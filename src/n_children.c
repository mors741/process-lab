/*
 ============================================================================
 Name        : n_children.c
 Author      : Evgeny Kharitonov
 ============================================================================
 */

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>

int w = 1;
int count;

void handler_child(int sig_no)
{
	signal(sig_no, SIG_IGN);
	if (sig_no == 14)
	{
		printf("Child-%d caught signal SIGALRM\n",getpid());
		kill(getppid(),SIGINT);
		exit(0);
	} else {
		printf("Child-%d caught signal %d\n",getpid(), sig_no);
		exit(sig_no);
	}
	signal(sig_no, &handler_child);
}

void handler_parent(int sig_no)
{
	signal(sig_no, SIG_IGN);
	if (sig_no == 2)
		printf("Parent caught signal SIGINT (%d signals left)\n", --count);
	else
		printf("Parent caught signal %d\n", sig_no);
	if (count==0){
		printf("Parent caught ALL THE SIGNALS. EXIT\n");
		exit(0);
	}
	signal(sig_no, &handler_parent);
}

void handler_parent_alrm(int sig_no)
{
	printf("10 seconds passed\n");
	w = 0;
}

int main(int argc, char *argv[])
{
	if (argc == 1)
	{
		printf("Usage:\n   n_children N\n   N - amount of the child processes\n");
		exit(1);
	}
	int i;
	int n=atoi(argv[1]);
	printf("%d processes will be created\n\n",n);
	if (n == 0){
		exit (0);
	}
	count = n;
	pid_t pids[n];

	// Start children.
	for (i = 0; i < n; ++i) {
		if ((pids[i] = fork()) < 0) {
			perror("fork");
			abort();
		} else if (pids[i] == 0) {
			printf("Child-%d process created\n",getpid());
			signal( SIGALRM, &handler_child);
			while(1){
				sleep(1);
			}
		}
	}
	signal( SIGINT, &handler_parent);
	for (i = 0; i < n; ++i) {
		sleep(1);
		kill(pids[i], SIGALRM);
	}

	// Wait for children to exit.
	signal(SIGALRM, &handler_parent_alrm);
	alarm(10);
	while (w)
	{
		pause();
	}
	printf("Parent is going to kill all his children :(\n");
	for (i = 0; i < n; ++i) {
		kill(pids[i], SIGKILL);
	}
	int status;
	pid_t pid;
	int max_status=0;
	while (n > 0) {
	  pid = wait(&status);
	  printf("Child with PID %ld exited with status %d\n", (long)pid, WEXITSTATUS(status));
	  if (WEXITSTATUS(status) > max_status)
		  max_status = WEXITSTATUS(status);
	  n--;
	}
	printf("exit status = %d\n",max_status);
	exit(max_status);
}
