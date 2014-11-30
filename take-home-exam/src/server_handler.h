/*
Header for the server_handler.c-file
*/

#ifndef SERVER_HANDLER
#define SERVER_HANDLER
char fill_buf[300];

void set_values_from_file(short msg_size, short game_type, short points, short teammates, 
	short oppositions, short score, short killed, short survivors, short reserved, short free_space, char name[8]);
int validate_package(char* package, char msg_type);
int decide_package_type(char *package); 
void decrypt_point_package(char* buf);
void decrypt_error_package(char* buf);
void decrypt_update_package(char* buf);
void decrypt_start_package(char* buf);
int decrypt_setup_package(char* buf);
void decrypt_end_package(char* buf);
void make_highscore_package(char* buf); 
void make_error_package(char* msg);
void check_endiannes();
#endif