#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>  /* For getopt_long() */
#include <string.h> 
 
#include "engine.h"
#include "player.h"
#include "ipportchecks.h" //For checkin the ip/port-arguments
#include "client_handler.h"
#include "structs.h"

char* args_handle (int argc, char* argv[]);
void print_help();

char* user;
char* ip;
char* port;
char* single_ip;
char* single_port;
char* multi_ip;
char* multi_port;
int error = 0, multi = 0;
/** Handles all the commandline arguments.
 *
 * This is the man function of this module. It processes
 * an argv[] array of argc elements based on the options
 * specified at the struct option.
 *
 * If you want to add another argument, follow these steps:
 * # Add an option on the struct option.
 * # Add the short equivalent (if it exists) on the getopt_long() call.
 * # Add a case on the switch specifying the action of that option.
 */
char* args_handle (int argc, char* argv[])
{
 	static struct option options[] =
 	{
 		{"help",	no_argument, NULL, 'h'},
 		{"user",		required_argument, NULL, 'u'},
 		{"hs",		required_argument, NULL, '1'},
 		{"hp",		required_argument, NULL, '2'},
 		{"ms",		required_argument, NULL, '3'},
 		{"mp",		required_argument, NULL, '4'},
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
 		c = getopt_long (argc, argv, "h:u:1:2:3:4", options, &option_index);
 		switch (c)
 		{
 			case -1:
				// There were no '-' parameters passed
				// or all the parameters were processed
	 			break;
 			case 'h':
 				/*Set default so any other letter passed to getopt, even with wrong syntax
				will print help.
				*/
	 			print_help();
	 			exit(EXIT_SUCCESS);
	 			break;
 			case 'u':
 				//Gets username
 				if (optarg == NULL) {
 					printf("Wrong syntax of option o!\n");
					printf("Usage: %s %s\n", argv[0], argv[1]);
					printf("Do: [file] [--o] [username]\n");
				 	exit(EXIT_SUCCESS);
 				}
 				user = optarg;
 				break;
 			case '1':
 				/*
 				Gets ip-adress for single player, prints error message if it is of wrong format
 				*/
 				single_ip = optarg;
 				if (is_valid_ip(single_ip) != 0) {
 					printf("Not valid IP-adress!\n");
 					printf("Usage: %s\n", single_ip);
 					printf("Do: ... [--hs] [123.123.123.12]\n");
				 	exit(EXIT_SUCCESS);
 				}
 				ip = single_ip;
 				break;
 			case '2': 
 				/*
 				Gets port number for single player, prints error message if it is of wrong format
 				*/
 				single_port = optarg;
 				int port_check = is_valid_port(single_port);
				if (port_check == -1) {
					printf("Not valid port number!\n");
					printf("Your port number is above 65535!\n");
			 		printf("Usage: %s\n", single_port);
			 		printf("Do: ... [--p] [1025-65535]\n");
					exit(EXIT_SUCCESS);
				} else if (port_check == -2) {
					printf("Not valid port number!\n");
					printf("All port numbers below 1024 are reserved!\n");
			 		printf("Usage: %s\n", single_port);
			 		printf("Do: ... [--p] [1025-65535]\n");
					exit(EXIT_SUCCESS);
				} else if (port_check == -3) {
					printf("Not valid port number!\n");
			 		printf("Usage: %s\n", single_port);
			 		printf("Do: ... [--p] [1025-65535]\n");
					exit(EXIT_SUCCESS);
				}
 				port = single_port;
 				break;
 			case '3': 
 				/*
 				Gets ip-adress for multiplayer, prints error message if it is of wrong format
 				*/
 				multi_ip = optarg;
 				if (is_valid_ip(multi_ip) != 0) {
 					printf("Not valid IP-adress!\n");
 					printf("Usage: %s\n", multi_ip);
 					printf("Do: ... [--ms] [123.123.123.12]\n");
				 	exit(EXIT_SUCCESS);
 				}
 				ip = multi_ip;
 				multi = 1;
 				break;
 			case '4': 
 				/*
 				Gets port number for multiplayer, prints error message if it is of wrong format
 				*/
 				multi_port = optarg;
 				port_check = is_valid_port(multi_port);
				if (port_check == -1) {
					printf("Not valid port number!\n");
					printf("Your port number is above 65535!\n");
			 		printf("Usage: %s\n", multi_port);
			 		printf("Do: ... [--p] [1025-65535]\n");
					exit(EXIT_SUCCESS);
				} else if (port_check == -2) {
					printf("Not valid port number!\n");
					printf("All port numbers below 1024 are reserved!\n");
			 		printf("Usage: %s\n", multi_port);
			 		printf("Do: ... [--p] [1025-65535]\n");
					exit(EXIT_SUCCESS);
				} else if (port_check == -3) {
					printf("Not valid port number!\n");
			 		printf("Usage: %s\n", multi_port);
			 		printf("Do: ... [--p] [1025-65535]\n");
					exit(EXIT_SUCCESS);
				}
 				port = multi_port;
 				multi = 1;
 				break;
 		}
 	}

	/* Just in case the user specified more arguments (not options)
	 * than needed, you decide what to do. Here, we just ignore them */
 	while (optind < argc) optind++;
 	return NULL;
}
/**	Prints Help instructions on standard output.
*/
void print_help(void)
{
 	printf("Synopsis:\n");
 	printf("\tThe classic TRON game.\n\n");

 	printf("Controls:\n");
 	printf("\tNumbers (1 ~ 9)   Changes the game speed at the main menu\n");
 	printf("\tArrow Keys        Control the directions\n");
 	printf("\tq                 Quits the game at any time\n");
 	printf("\n\n");

 	printf("Usage:\n");
 	printf("\tifitron [-h]\n");
 	printf("Commandline arguments:\n\n");
 	printf("\t-h, --help         Displays the help guidelines.\n");
 	printf("\n");
}
// The main loop of the game.
int main (int argc, char* argv[], char* envp[])
{
	check_endiannes();
 	args_handle(argc, argv);
 	if (user == NULL) 
 	{
 		/*If two or more arguments, args_handle will get username or return NULL
 		so that username can be fetched from the environment*/
 		user = getenv("USER");
	}
	if (single_ip != NULL && multi_ip != NULL) {
		printf("You cant play both single- and multiplayer!\n");
		exit(EXIT_SUCCESS);
	}
	if ( !((single_ip != NULL && single_port != NULL) || (multi_ip != NULL && multi_port != NULL)) ) {
		printf("You must have ip-adress and port-number for either single- or multiplayer!\n");
		exit(EXIT_SUCCESS);
	}
	/*
	If multiplayer port and ip-adress is set, a setup package
	is sent to the server to indicate that a player has 
	connected to the multiplayer server
	*/
	if (multi == 1) {
		if (declare_setup_package() == 0) {
			if (send_package(port, ip, 3) == -3) {
				error = 1;
			}
		}
	}
	/*
	Sets a predefined number of packages
	*/
	signed char numb_packages = -1;
	/*
	Declares a highscore_package to be sent to the server
	*/
	if (declare_highscore_package(port, ip, numb_packages) == 0) {
		int send = send_package(port, ip, 2);
		if (send == -1) {
			print_in_game("Server not running! The scores will not be saved!");
		} else if (send == -2) {
			print_in_game("Something is wrong with the socket! The scores will not be saved");
		} else if (send == -3) {
			error = 1;
		}
 	}
 	/*
 	Starts the game, and sends the username so it can be shown on screen
 	*/
	engine_init(user, port, ip);
	/*
	Sets the highscore list for the first time in the game, filled 
	with highscores recieved from the server
	*/
	is_multi = multi;
	engine_show_main_menu();
	ifitron_init ();
	while (TRUE)
 	{
 		if (tron.is_alive == FALSE) 
 		{
 			/*If a game is over, gets the game info and sets it to the global
 			struct that holds the info, so it can be decrypted and copied
 			to the point_package
 			*/
 			set_game_info();
 			/*
 			Sends the point_package with info from the recently terminated game
 			*/
 			if (declare_point_package(game, ip, port) == 0) {
 				int send = send_package(port, ip, 1);
 				if (send == -1) {
					print_in_game("Server not running! The scores will not be saved!");
				} else if (send == -2) {
					print_in_game("Something is wrong with the socket! The scores will not be saved");
				} else if (send == -3) {
					error = 1;
				}
 			}
 			/*
 			Requests a new highscore package with updated highscores
 			*/
 			if (error == 0) {
 				if (declare_highscore_package(port, ip, numb_packages) == 0) {
 				int send = send_package(port, ip, 2);
					if (send == -1) {
						print_in_game("Server not running! The scores will not be saved!");
					} else if (send == -2) {
						print_in_game("Something is wrong with the socket! The scores will not be saved");
					} else if (send == -3) {
						error = 0;
					}
				}
 			}
 			ifitron_game_over();
		}
		/*
		Error checking. Checks if multiplayer is choosen in-game.
		If not, multi is 0, and the player cant start a game 
		unless he chooses multiplayer ('M'). If singleplayer mode is choosen
		the 'M'-option is not reachable. If an error has occured in the packages, the player can't
		start a game.
		*/
		is_multi = multi;
		engine_get_game_input(multi);
		if (is_multi == 0 && is_multi_choosen == 1) {
			engine_show_main_menu();
			engine_show_screen ();
			print_in_game("You can't play multiplayer player on a single player server!");
		} else if (is_multi == 1 && is_multi_choosen == 0) {
			engine_show_main_menu();
			engine_show_screen ();
			print_in_game("You can't play single player on a multiplayer server!");
		} else if (error != 1) {
	 		player_update();
	 		player_increase_size (1);
	 		player_increase_score (game.level);
	 		if (tron.score % 50 == 0 && game.level < 9) game.level++;

	 		if (player_hit_self() == TRUE  || player_hit_borders() == TRUE)
	 			tron.is_alive = FALSE;
 			engine_show_screen ();
		} else if (error == 1) {
			engine_show_main_menu();
			engine_show_screen ();
		}
		
 		
 	}
 	return 0;
}
