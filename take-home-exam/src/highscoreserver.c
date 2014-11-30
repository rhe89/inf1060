#include "highscoreserver.h"
#include "ipportchecks.h"
#include "netcom.h"
#include "memory.h"
#include "server_handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h> 
#include <string.h>

char* port_number;
char* file_name;
/*
If the file is new, it should be declared with "w", if it is
passed as an argument, it should be declared with "a" for appending at
the end of the file
*/
int append;
int multi;

int main(int argc, char* argv[], char* envp[]) 
{
	check_endiannes();
	args_handle(argc, argv);
	signal(SIGINT, catch_signals);
	/*
	Checks if a filename was defined in the command line,
	and reads the file if it was, or sets the the filename
	for writing later if it was no. 
	*/
	if (file_name != NULL) {
		append = 0;
		read_file();
	} else {
		append = 1;
		set_blank();	
	}
	if (multi == 0) {
		printf("Single player server: Listening for connections on port '%s'\n", port_number);
		start_server(port_number, multi);
	}
	else if (multi == 1) {
		printf("Multi player server: Listening for connections on port '%s'\n", port_number);
		start_server(port_number, multi);
	}
	

	/*
	Starts a child process which task is to see if stdin is EOF, which
	means that CTRL-D has been pressed. If so, shutdown procedures are run
	*/
	pid_t pid = fork();
	if (pid == 0) 
	{
		if (getchar() == EOF) {
			save_to_file(file_name, append);
			print_file();
			free_mem_allocs();
			int parent = getppid();
           	kill(parent, SIGKILL);
           	exit(0);
		} 
	}
}
/*
Arguments are handled from option p, and optionally option f, to get desired
port number and desired highscore file to read from. Different error checks are used
to make sure nothing goes wrong in the process
*/ 	
void args_handle (int argc, char* argv[])
{
 	static struct option options[] =
 	{
 		{"port", required_argument, NULL, 'p'},
 		{"load", required_argument, NULL, 'l'},
 		{"multi", required_argument, NULL, 'm'},
		/* The last element must be all zeroes */
 		{0, 0, 0, 0}
 	};
	/* The index of the current option */
 	int option_index;
	/* The character for comparison */
 	int c = 0;
	/* We keep checking the arguments untill they run out (c == -1) */
 	while (c != -1)
 	{
 		c = getopt_long (argc, argv, "p:lm", options, &option_index);
 		switch (c)
 		{
 			case -1:
				// There were no '-' parameters passed
				// or all the parameters were processed
	 			break;
 			case 'p':
 				
 				port_number = optarg;
 				/*Checks if there were no port number passed as argument,
 				or if it is of the wrong format
				*/
 				int port_check = is_valid_port(port_number);
				if (port_check == -1) {
					printf("Not valid port number!\n");
					printf("Your port number is above 65535!\n");
			 		printf("Usage: %s\n", port_number);
			 		printf("Do: ... [--p] [1025-65535]\n");
					exit(EXIT_SUCCESS);
				} else if (port_check == -2) {
					printf("Not valid port number!\n");
					printf("All port numbers below 1024 are reserved!\n");
			 		printf("Usage: %s\n", port_number);
			 		printf("Do: ... [--p] [1025-65535]\n");
					exit(EXIT_SUCCESS);
				} else if (port_check == -3) {
					printf("Not valid port number!\n");
			 		printf("Usage: %s\n", port_number);
			 		printf("Do: ... [--p] [1025-65535]\n");
					exit(EXIT_SUCCESS);
				}
 				break;
 			case 'l':
 			/*
 			Sets filename from the argument. Tests on this will
 			be done later
 			*/
 				file_name = optarg;
 				break; 	
 			/*
 			If argument is passed, the multiplayer server will start
 			*/
 			case 'm':
 				multi = 1;
			default:
				break;			
		}
	}
}
/*
Catches the CTRL-C signal, to be able
to save the file before exiting
*/
void catch_signals()
{ 		
	signal(SIGINT, catch_signals);
	print_file();
	save_to_file(file_name, append);
	printf("Saving file\n");
	free_mem_allocs();
	exit(0);
}
/*
Reads a file from the command line
*/
void read_file() 
{
	/*
	For each line. Sets size to 50 to be on the safe side! 
	*/
	char line[50];
	/*
	Declares the file-pointer, opens it for reading only
	*/
	FILE* file = fopen(file_name, "r");
	/*
	Checks for existance to prevent segmentation fault
	*/
	if (!file) {
		printf("The highscore-file doesn't exists. Please type a valid file name or ");
		printf("don't pass it as an argument!\n");
		exit(EXIT_SUCCESS);
	}
	short msg_size = 0;
	short game_type = 0;
	short points = 0;
	short teammates = 0;
	short oppositions = 0;
	short score = 0;
	short killed = 0;
	short survivors = 0;
	short reserved = 0;
	short free_space = 0;
	char name[8];
	/* 
	Reads one and one line, and gets info that line
	and sends that to the server-handler for creating
	a point_package
	My file format looks like this:
		message size, game type, points, teammates, oppositions, score, killed, survivors, name
		message size, game type, points, teammates, oppositions, score, killed, survivors, name
	    ............, ........., ......, ........., ..........., ....., ......, ........., ....
	    ............, ........., ......, ........., ..........., ....., ......, ........., ....
	*/
	while (fgets(line, sizeof(line), file) != NULL) {
		sscanf(line, "%hu %hu %hu %hu %hu %hu %hu %hu %s\n", &msg_size, &game_type, &points, &teammates, &oppositions, &score, &killed, &survivors, name);
		set_values_from_file(msg_size, game_type, points, teammates, oppositions, score, killed, survivors, reserved, free_space, name);
	}
	fclose(file);
}

/*
Starts up a highscore server with no saved highscores
Declares the filename to be used when saving
*/
void set_blank() 
{
	file_name = "highscore.sav";
}
