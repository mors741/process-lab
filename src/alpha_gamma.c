/*
 ============================================================================
 Name        : alpha_gamma.c
 Author      : Evgeny Kharitonov
 ============================================================================
 */
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void go_next(int sig)
{
	char c='X';
	signal(sig,&go_next);
	switch (sig)
	{
		case 6:
			c='0';
			break;
		case 7:
			c='1';
			break;
		case 8:
			c='2';
			break;
		case 17:
			c='3';
			break;
		case 11:
			c='4';
			break;
		case 12:
			c='5';
			break;
		case 13:
			c='6';		// SIGPIPE
			break;
		case 14:
			c='7';		// SIGALRM
			break;
		case 15:
			c='8';		// SIGTERM
			break;
		case 16:
			c='9';
			break;
	}
	printf("%c",c);
}
/* ===== Termination procedure ===== */
void stop_go(int sig)
{
	printf("\nProcess %d is stopped by Parent: %d\n",getpid(),getppid());
	kill(getppid(), SIGKILL);
	exit(1);
}
/* ===== ALPHA_GAMMA process body ====== */
int main()
{
	setvbuf(stdout,(char*)NULL,_IONBF,0);
	signal(6, &go_next);
	signal(7, &go_next);
	signal(8, &go_next);
	signal(17, &go_next);
	signal(11, &go_next);
	signal(12, &go_next);
	signal(13, &go_next);
	signal(14, &go_next);
	signal(15, &go_next);
	signal(16, &go_next);
	signal(SIGINT ,&stop_go);
	printf("Alpha_Gamma: %d\n personal number:\n", getpid());
	while (1)
	{
		pause();
	}
	printf("This message should not be printed\n");
	exit(3);
}
