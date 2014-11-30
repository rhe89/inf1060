#include "server_handler.h"
#include "memory.h"
#include "bit_handling.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h> 
#include <string.h>
#include <signal.h>

points_package 		buffer;
highscore_package 	highscore;
char 				generic_point_buffer[sizeof(points_package)];
char 				generic_highscore_buffer[sizeof(highscore_package)];
char 				generic_error_buffer[sizeof(error_package)];
char 				generic_update_buffer[sizeof(update_package)];
int 				endianess;
int 				playerID;
#define 			BIGENDIAN 2
#define 			LITTLEENDIAN 1

void check_endiannes()
{
	int check = 2000000;
	if(*(char *)&check == 1) {
		endianess = BIGENDIAN;
	} else {
		endianess = LITTLEENDIAN;
	}
}
/*
Sets values for a points package, then fills a char buffer with a point package using memcpy,
preparing it to be stored in memory
*/
void set_values_from_file(short msg_size, short game_type, short points, short teammates, 
	short oppositions, short score, short killed, short survivors, short reserved, short free_space, char name[8]) 
{
	buffer.msg_size = msg_size;
	buffer.game_type = define_game_type(game_type);
	buffer.points = points;
	/*
	Needs to convert these fields to 8 bits
	*/
	buffer.teammates = get_8bit_from_16bit(teammates);
	buffer.oppositions = get_8bit_from_16bit(oppositions);
	buffer.line_3 = compress_to_32bit(score, killed, survivors, reserved, free_space);
	strcpy(buffer.name, name);
	/*
	Sets the message type so it can be interpreted correctly
	*/
	buffer.msg_type = 0x01;
   	memcpy(&generic_point_buffer, &buffer, sizeof(generic_point_buffer));
   	add_to_memory(generic_point_buffer, 1);
}
/*
Error checking for recieved packages
*/
int decide_package_type(char *package) 
{
	/*
	Checks the message type field in each package 
	*/
	//Highscore list package
	if (package[2] == 0x00) {
		int validation = validate_package(package, package[2]);
		/*
		The package was of the right size
		*/
		if (validation == 0) { 
			printf("Recieved highscore list request!\n");
			make_highscore_package(package);
			return 1;
			/*
			The package was too large, and an error package is sent back to the client
			who sent the original package
			*/
		} else if (validation == -1)  {
			make_error_package("The package you sent was too large!");
			return -1;
		}
	//Point package
	} else if (package[2] == 0x01) {
		int validation = validate_package(package, package[2]);
		if (validation == 0) { 
			printf("Recieved point package!\n");
			decrypt_point_package(package);
			return 2;
		} else if (validation == -1)  {
			make_error_package("The package you sent was too large!");
			return -1;
		}
	//Error package
	} else if (package[2] == 0x10) {
		int validation = validate_package(package, package[2]);
		if (validation == 0) { 
			printf("Recieved error package!\n");
			decrypt_error_package(package);
			return 3;
		} else if (validation == -1)  {
			return -1;
		}
	//Setup package
	} else if (package[2] == 0x02) {
		int validation = validate_package(package, package[2]);
		if (validation == 0) { 
			printf("Recieved setup package!\n");
			if (decrypt_setup_package(package) == -1) {
				make_error_package("No more players allowed! Maximum amount of players: 2!");
				return -3;
			}
			return 4;
		} else if (validation == -1)  {
			return -1;
		}
	//Start package
	} else if (package[2] == 0x03) {
		int validation = validate_package(package, package[2]);
		if (validation == 0) { 
			printf("Recieved start of game package!\n");
			decrypt_start_package(package);
			return 5;
		} else if (validation == -1)  {
			return -1;
		}
	//Update package
	} else if (package[2] == 0x04) {
		int validation = validate_package(package, package[2]);
		if (validation == 0) { 
			printf("Recieved update package!\n");
			decrypt_update_package(package);
			return 6;
		} else if (validation == -1)  {
			return -1;
		}
	//End package
	} else if (package[2] == 0x05) {
		int validation = validate_package(package, package[2]);
		if (validation == 0) { 
			printf("Recieved end of game package!\n");
			decrypt_end_package(package);
			return 7;
		} else if (validation == -1)  {
			return -1;
		}
		/*
		Comes here if none of the message types were recognized, and sends an error
		package back to the client
		*/
	}  else {
		make_error_package("The message type was not recognized by the server!");
		return -2;
	}
	return 0;
}
/*
Checks the size of the buffer to determine if it is too large to be decrypted
*/
int validate_package(char* package, char msg_type) 
{
	if (sizeof(package) > 300) {
		return -1;
	}
	return 0;
}
void make_highscore_package(char* buf) 
{
	/*
	Declares two highscore packages, one for the buffer who was recieved,
	and one to be sent back to the client with the point packages
	*/
	highscore_package recv_pkg;
	highscore_package sent_pkg;
	int i, k = 0, query;
	int counter = get_filled_areas();

	points_package ret_pkg[counter];
	/*
	Decrypts the highscore package recieved from the client to determine how
	many point packages to be sent back, by reading the negative number.
	*/
	memset(&recv_pkg, 0, sizeof(highscore_package));
	memcpy(&recv_pkg, &buf, sizeof(highscore_package));
	
	query = recv_pkg.numb_packages;
	if (query > get_filled_areas()) {
		query = get_filled_areas();
	} 
	/*
	Gets the max score for each game type from memory, and adds them to a buffer
	to be sent back to the client
	*/
	for(i = 0; i < 31; i++) {
		points_package tmp = get_pkg_from_index(i);
		if (tmp.points != 0) {
			if (k < query) {
				if (endianess == LITTLEENDIAN) {
					tmp.msg_size = htons(tmp.msg_size);
					tmp.points = htons(tmp.points);
					tmp.line_3 = htons(tmp.line_3);
				}
				memcpy(&ret_pkg[k], &tmp, sizeof(points_package));
				/*
				Counter to keep track of the number of point packages in the buffer
				*/
				k++;
			}
		}
	}
	/*
	Sets the message size for the highscore package to be sent back to the client, by
	adding the highscore package, and number of point packages
	*/
	if (endianess == LITTLEENDIAN) {
		sent_pkg.msg_size = htons(sizeof(highscore_package) + (counter * sizeof(points_package)));
	} else {
		sent_pkg.msg_size = sizeof(highscore_package) + (counter * sizeof(points_package));
	}


	/*
	Sets the message type so it can be interpreted correctly
	*/	
	sent_pkg.msg_type = 0x00;
	/*
	Sets the number of packages to a positive number
	*/
	sent_pkg.numb_packages = query;
	memset(&fill_buf, 0, sizeof(fill_buf));
	/*
	Sets the first part of the buffer with the highscore package, then sets the point packages
	after
	*/
	memcpy(&fill_buf[0], &sent_pkg, sizeof(highscore_package));
	for (i = 0; i < k; i++) {
		memcpy(&fill_buf[sizeof(highscore_package) + (i*sizeof(points_package))], &ret_pkg[i], sizeof(points_package)); 
	}
	fill_buf[2] = 0x00;
}
/*
Makes an error package containing the error message recieved as a parameter
*/
void make_error_package(char* msg) 
{
	memset(&fill_buf, 0, sizeof(fill_buf));
	error_package error_pkg;
	memset(&error_pkg, 0, sizeof(error_package));
	/*
	Sets the message type so it can be interpreted correctly
	*/
	error_pkg.msg_type = 0x10;
	strcpy(error_pkg.error, msg);
	if (endianess == LITTLEENDIAN) {
		error_pkg.msg_size = htons(sizeof(error_package));
	} else {
		error_pkg.msg_size = sizeof(error_package);
	}
	memcpy(&fill_buf, &error_pkg, sizeof(fill_buf));
}
/*
Adds a recieved point package to memory
*/
void decrypt_point_package(char* buf) 
{
	char tmp_buf[sizeof(points_package)];
	points_package rec;
	memcpy(&rec, buf, sizeof(points_package));
	if (endianess == LITTLEENDIAN) {
		rec.msg_size = ntohs(rec.msg_size);
		rec.points = ntohs(rec.points);
		rec.line_3 = ntohs(rec.line_3);
	}
	memcpy(tmp_buf, &rec, sizeof(tmp_buf));
	add_to_memory(tmp_buf, 0);
}
/*
Interprets the error message in the error package, and prints it
*/
void decrypt_error_package(char* buf) 
{
	error_package tmp;
	memcpy(&tmp, buf, sizeof(error_package));
	printf("ERROR: %s\n", tmp.error);
	memset(&fill_buf, 0, sizeof(fill_buf));
	memcpy(&fill_buf, &tmp, sizeof(fill_buf));
}
/*
Interprets an update package from a client, which indicates
a new direction from a player playing multiplayer, and sets the 
endianess accordingly to the machine type.
*/
void decrypt_update_package(char* buf)
{
	update_package pkg;
	memcpy(&pkg, buf, sizeof(update_package));
	if (endianess == LITTLEENDIAN) {
		pkg.direction = ntohs(pkg.direction);
		pkg.playerID = ntohs(pkg.playerID);
		pkg.msg_size = ntohs(sizeof(update_package));
	}
}
/*
Interprets a start package from a client, which indicates
a new player has started playing multiplayer, and sets the 
endianess accordingly to the machine type.
*/
void decrypt_start_package(char* buf)
{
	start_package pkg;
	memcpy(&pkg, buf, sizeof(start_package));
	pkg.msg_type = 0x03;
	if (endianess == LITTLEENDIAN) {
		pkg.playerID = ntohs(pkg.playerID);
		pkg.msg_size = ntohs(sizeof(setup_package));
	}
}
/*
Interprets a setup package from a client, which indicates
a new player has connected, and increases the number of players connected.
If two players already is connected, an error package is sent back to 
the client (by returning -1) saying that the server is full.
*/
int decrypt_setup_package(char* buf)
{
	if (playerID == 2) {
		return -1;
	}
	setup_package pkg;
	memcpy(&pkg, buf, sizeof(setup_package));
	if (endianess == LITTLEENDIAN) {
		pkg.playerID = ntohs(++playerID);
		pkg.msg_size = ntohs(sizeof(setup_package));
	} else {
		pkg.playerID = ++playerID;
	}
	memcpy(fill_buf, &pkg, sizeof(setup_package));
	return 0;
}
/*
Interprets an end package from a client, which indicates that a player
has stopped playing. The numbers of players on the server is then decreased,
so that a new player can connect to the multiplayer server
*/
void decrypt_end_package(char* buf) 
{
	end_package pkg;
	memcpy(&pkg, buf, sizeof(end_package));
	pkg.msg_type = 0x05;
	if (endianess == LITTLEENDIAN) {
		pkg.playerID = ntohs(pkg.playerID);
		pkg.msg_size = ntohs(sizeof(end_package));
	} 
	playerID--;
}