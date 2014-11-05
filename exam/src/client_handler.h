/*
Header for the client_handler.c-file
*/

#include <stdio.h>
#include <stdlib.h>
#include "engine.h"

/*
Global variables for the game to access
*/
char 			to_game[300];
signed int 		size;
int				is_multi;
int 			playerID;

void check_endiannes();
int decide_package_type(char* package);
void decrypt_error_package(char* package);
void decrypt_highscore_package(char* rec_pack);
int decrypt_setup_package(char* package);
int declare_point_package(struct game_t game_info, char* ip, char* port);
int declare_highscore_package(char* port, char* ip, unsigned char numb_packages);
int declare_update_package(short playerID, short direction);
int declare_setup_package();
int declare_start_package(short playerID);
int declare_end_package(short playerID); 
int send_package(char* port, char* ip_address, int pack_type);
int send_error_package(char* error_msg, char* ip, char* port);