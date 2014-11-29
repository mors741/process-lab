/*
 * RB-number.c
 *
 *  Created on: Nov 26, 2014
 *      Author: Evgeny Kharitonov 
 */

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>

int main()
{
	pid_t pid;
	setvbuf (stdout, (char*) NULL, _IONBF, 0);
	switch (pid = fork())
	{
		case -1:
			perror("Unsuccessful fork");
			_exit(1);
			break;
		case 0:
			//execl("/home/mors/workspace/Process-lab/alpha_gamma", "alpha_gamma", NULL);
			execl("alpha_gamma", "alpha_gamma", NULL);	
			break;
		default:
			sleep(1);
			kill(pid,7);
			sleep(1);
			kill(pid,8);
			sleep(1);
			kill(pid,17);
			sleep(1);
			kill(pid,17);
			sleep(1);
			kill(pid,12);
			sleep(1);
			kill(pid,13);
			sleep(1);

			kill(pid, SIGINT);
			while (1){
			}
			break;
	}
	return 0;
}
