/**
UNIX Shell Project

Sistemas Operativos
Grados I. Informatica, Computadores & Software
Dept. Arquitectura de Computadores - UMA

Some code adapted from "Fundamentos de Sistemas Operativos", Silberschatz et al.

To compile and run the program:
   $ gcc Shell_project.c job_control.c -o Shell
   $ ./Shell          
	(then type ^D to exit program)

**/

#include "job_control.h"   // remember to compile with module job_control.c 

#define MAX_LINE 256 /* 256 chars per line, per command, should be enough. */

// -----------------------------------------------------------------------
//                            MAIN          
// -----------------------------------------------------------------------

int main(void)
{
	char inputBuffer[MAX_LINE]; /* buffer to hold the command entered */
	int background;             /* equals 1 if a command is followed by '&' */
	char *args[MAX_LINE/2];     /* command line (of 256) has max of 128 arguments */
	// probably useful variables:
	int pid_fork, pid_wait; /* pid for created and waited process */
	int status;             /* status returned by wait */
	enum status status_res; /* status processed by analyze_status() */
	int info;				/* info processed by analyze_status() */

	while (1)   /* Program terminates normally inside get_command() after ^D is typed*/
	{   		
		printf("COMMAND->");
		fflush(stdout);
		get_command(inputBuffer, MAX_LINE, args, &background);  /* get next command */
		
		if(args[0]==NULL) continue;   // if empty command

		/* the steps are:
			 (1) fork a child process using fork()
			 (2) the child process will invoke execvp()
			 (3) if background == 0, the parent will wait, otherwise continue 
			 (4) Shell shows a status message for processed command 
			 (5) loop returns to get_commnad() function
		*/
		pid_fork = fork(); //creamos un nuevo proceso hijo (return: ID de dicho proceso) [T1]
		new_process_group(pid_fork)  // crea un nuevo grupo [T2]
		
		switch(pid-fork){
			case(-1): // [ERROR]
				perror("no se pudo crear el proceso hijo");
				break;

			case 0 : // [PROCESO HIJO]
				if(execvp(inputBuffer, args) == -1){//si su ejecucion falla, mensaje de error [T1] 
					printf("Error, command not found: %s \n", inputBuffer);
				}//ejecutamos el proceso (2)*
				break;

			default: // [PROCESO PADRE]
				if(background == 0){			        //si el background es 0 (3) [T1]
				// ---FOREGROUND--- 
 					waitpid(pid_fork, &status, 0);		//el padre espera [T1]
					status_res = analyze_status( status, &info ); 	//analizo el status [T1]
										 	                        //y se modifica info (nose si esto va aqui)

					printf("Foreground pid: %d, command: %s , %s, info: %i \n", getpid()
											    , *args
											    , status_strings[status_res]
											    , info);
				}else{ 
				// ---BACKGROUND---
					printf("Background job running... pid: %d, command: %s \n", getpid(),*args);
				}//end else
				
				break;
		

	} // end while
}
