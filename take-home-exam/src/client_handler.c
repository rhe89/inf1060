#include "client_handler.h"
#include "structs.h"
#include "bit_handling.h"
#include "netcom.h"

short TEAMMATES   	=  0;
short OPPOSITIONS 	=  0;
short KILLED 	  	=  0;
short SURVIVORS	  	=  0;
short FREE_SPACE  	=  0;
short RESERVED 	  	=  0;

points_package 		buffer;
highscore_package 	highscore, info;
char 				generic_point_buffer[sizeof(points_package)];
char 				generic_highscore_buffer[sizeof(highscore_package)];
char 				generic_error_buffer[sizeof(error_package)];
char 				generic_start_buffer[sizeof(start_package)];
char 				generic_update_buffer[sizeof(update_package)];
char 				generic_setup_buffer[sizeof(setup_package)];
char 				generic_end_buffer[sizeof(end_package)];
int 				endianess;
#define 			BIGENDIAN 2
#define 			LITTLEENDIAN 1

/*
Check to see which endianess the computer is. For later checks,
if bit length is smaller than 8, no check is needed. For higher
bits, the endianess is needed to determine if packages should
be htons'd (host-to-network-order, big endian) before sending to server, 
and if they should be ntohs'd (network-to-host-order, little endian), when
decrypting the package to get the right values for this computer from the variables.
*/
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
Checks wether a package is an error package (0x10) or an highscore package. Uses the second field
in the char array for checking, which always is msg_type in each of the structs representing the different
protocols
*/
int decide_package_type(char* package) 
{
	if (package[2] == 0x00) {
		decrypt_highscore_package(package);	
		print_in_game("Recieved highscore list from server!");
		return 1;
	} else if (package[2] == 0x10) {
		decrypt_error_package(package);
		return 2;
	} else if (package[2] == 0x02) { 
		decrypt_setup_package(package);
		print_in_game("Recieved setup package from server! Ready to play!");
	}
	return 0;
}
int decrypt_setup_package(char* package) 
{
	setup_package pkg;
	memcpy(&pkg, package, sizeof(setup_package));
	if (endianess == LITTLEENDIAN) {
		playerID = ntohs(pkg.playerID);
	}
	return 0;
}

void decrypt_error_package(char* package) 
{
	error_package tmp;
	memset(&tmp, 0, sizeof(error_package));
	memcpy(&tmp, package, sizeof(error_package));
	if (endianess == LITTLEENDIAN) {
		tmp.msg_size = ntohs(tmp.msg_size);
	}
	print_in_game(tmp.error);
}
void decrypt_highscore_package(char* rec_pack)
{
	memset(&info, 0, sizeof(highscore_package));
	memset(&to_game, 0, sizeof(to_game));
	memcpy(&info, &rec_pack[0], sizeof(highscore_package));
	if (endianess == LITTLEENDIAN) {
		info.msg_size = ntohs(info.msg_size);
	}
	size = info.numb_packages;
	int i;
	for (i = 0; i < info.numb_packages; i++) {
		points_package pkg;
		memcpy(&pkg, &rec_pack[sizeof(highscore_package) + (i*sizeof(points_package))], sizeof(points_package));
		if (endianess == LITTLEENDIAN) {
			pkg.msg_size = ntohs(pkg.msg_size);
			pkg.points = ntohs(pkg.points);
			pkg.line_3 = ntohs(pkg.line_3);
		}
		memcpy(&to_game[(i*sizeof(points_package))], &pkg, sizeof(points_package));
	}
}
/*
Recieves a struct with game info, and sets the values from this struct in the point package.
If the computer running the program is using little endian values, they need to be converted
to big endian values before sending, as well as all bit-values larger than the required field
in the package protocol need to be compressed so they can be put in the package. Error packages 
is sent if something is wrong.
Mesmets both the buffer and the generic_point_buffer, since they are global variables and may
contain values from another game. In the end, the buffer is copied to the adress of the generic_point_buffer,
and sent to the netcom for network processing.
*/
int declare_point_package(struct game_t game_info, char* ip, char* port) 
{
	memset(&buffer, 0, sizeof(buffer));
	memset(&generic_point_buffer, 0, sizeof(generic_point_buffer));

	buffer.game_type = define_game_type(game_info.level);
	buffer.teammates = get_8bit_from_16bit(TEAMMATES);
	if (buffer.teammates == -1) {
		send_error_package("The previous game returned an illegal value! Score couldn't be saved", ip, port);
		return -1;
	}
    buffer.oppositions = get_8bit_from_16bit(OPPOSITIONS);
    if (buffer.oppositions == -1) {
		send_error_package("The previous game returned an illegal value! Score couldn't be saved", ip, port);
    	return -1;
    }
	short msg_size = sizeof(points_package);
	if (endianess == LITTLEENDIAN) {
		buffer.points = htons(game_info.points);
		buffer.msg_size = htons(msg_size);
		buffer.line_3 = htons(compress_to_32bit(game_info.result, KILLED, SURVIVORS, FREE_SPACE, RESERVED));
	} else {
		buffer.points = game_info.points;
		buffer.msg_size = msg_size;
		buffer.line_3 = compress_to_32bit(game_info.result, KILLED, SURVIVORS, FREE_SPACE, RESERVED);
	}
 	strcpy(buffer.name, game_info.name);
 	generic_point_buffer[msg_size-1] = '\0'; 
 	buffer.msg_type = 0x01;
   	memcpy(&generic_point_buffer, &buffer, sizeof(generic_point_buffer));
   	return 0;
}
/*
Recives a number from the game saying how many point packages it wants from the server.
Memsets the highscore and generic_highscore_buffer, since they are global variables and may
contain values from another game. Sets the message type and numb_packackes in the highscore struct
(which should be a negative number, or else an error package is sent to the server)
Then memcopies it to the generic_highscore_buffer, making it ready for sending
*/
int declare_highscore_package(char* port, char* ip, unsigned char numb_packages) 
{
	memset(&highscore, 0, sizeof(highscore_package));
	memset(&generic_highscore_buffer, 0, sizeof(generic_highscore_buffer));
	if (endianess == LITTLEENDIAN) {
		highscore.msg_size = htons(sizeof(highscore_package));
	}
	highscore.msg_type = 0x00;
	highscore.numb_packages = numb_packages;
	if (highscore.numb_packages >> 7 != -1) {
		send_error_package("This package can not be sent! No highscores will be shown in the game!", ip, port);
		return -1;
	}
   	memcpy(&generic_highscore_buffer, &highscore, sizeof(generic_highscore_buffer)); 
	return 0;
}
/*
For multiplayer. Recieves a playerID for identification, and a direction variable for the
direction change. This is sent to the server, so that the server can send this update package
back to the other player to update their screen with this player's direction change.
*/
int declare_update_package(short playerID, short direction)
{
	update_package pkg;
	pkg.msg_type = 0x04;
	if (endianess == LITTLEENDIAN) {
		pkg.direction = htons(direction);
		pkg.playerID = htons(playerID);
		pkg.msg_size = htons(sizeof(update_package));
	} else {
		pkg.direction = direction;
		pkg.playerID = playerID;
		pkg.msg_size = sizeof(update_package);
	}
	memcpy(generic_update_buffer, &pkg, sizeof(update_package));
	return 0;
}
/*
For multiplayer. Recieves a playerID for identification. This package's message type
indicates that a player has pressed 'M' in the game and has started a game
*/
int declare_start_package(short playerID)
{
	start_package pkg;
	pkg.msg_type = 0x03;
	if (endianess == LITTLEENDIAN) {
		pkg.playerID = htons(playerID);
		pkg.msg_size = htons(sizeof(start_package));
	} else {
		pkg.playerID = playerID;
		pkg.msg_size = sizeof(setup_package);
	}
	memcpy(generic_start_buffer, &pkg, sizeof(start_package));
	return 0;
}
/*
For multiplayer. Sets a playerID for identification to send to the server. This
package indicates that a player has connected and is ready to start playing.
*/
int declare_setup_package()
{
	setup_package pkg;
	pkg.msg_type = 0x02;
	if (endianess == LITTLEENDIAN) {
		pkg.playerID = htons(playerID);
		pkg.msg_size = htons(sizeof(setup_package));
	} else {
		pkg.playerID = playerID;
		pkg.msg_size = sizeof(setup_package);
	}
	memcpy(generic_setup_buffer, &pkg, sizeof(setup_package));
	return 0;
}
/*
Package indicating that a player has stopped playing, so that the server can
free the slot and let another player take this players space
*/
int declare_end_package(short playerID) 
{
	end_package pkg;
	pkg.msg_type = 0x05;
	if (endianess == LITTLEENDIAN) {
		pkg.playerID = htons(playerID);
		pkg.msg_size = htons(sizeof(end_package));
	} else {
		pkg.playerID = playerID;
		pkg.msg_size = sizeof(end_package);
	}
	memcpy(generic_end_buffer, &pkg, sizeof(end_package));
	return 0;
}
/*
Calls the function in netcom who is responsible for sending a package over the network
Different return values is used for error checking and preventing unwanted crashes
*/
int send_package(char* port, char* ip_address, int pack_type)
{
	if (pack_type == 1) {
		int send = send_pkg(port, ip_address, generic_point_buffer, sizeof(generic_point_buffer));
		if (send == -1) {
			return -1;
		} else if (send == -2) {
			return -2;
		} else if (send == -3) {
			return -3;
		}
	} else if (pack_type == 2) {
		int send = send_pkg(port, ip_address, generic_highscore_buffer, sizeof(generic_highscore_buffer));
		if (send == -1) {
			return -1;
		} else if (send == -2) {
			return -2;
		} else if (send == -3) {
			return -3;
		}
	} else if (pack_type == 3) {
		int send = send_pkg(port, ip_address, generic_setup_buffer, sizeof(generic_setup_buffer));
		if (send == -1) {
			return -1;
		} else if (send == -2) {
			return -2;
		} else if (send == -3) {
			return -3;
		}
	} else if (pack_type == 4) {
		int send = send_pkg(port, ip_address, generic_start_buffer, sizeof(generic_start_buffer));
		if (send == -1) {
			return -1;
		} else if (send == -2) {
			return -2;
		} else if (send == -3) {
			return -3;
		}
	} else if (pack_type == 5) {
		int send = send_pkg(port, ip_address, generic_update_buffer, sizeof(generic_update_buffer));
		if (send == -1) {
			return -1;
		} else if (send == -2) {
			return -2;
		} else if (send == -3) {
			return -3;
		}
	} else if (pack_type == 6) {
		int send = send_pkg(port, ip_address, generic_end_buffer, sizeof(generic_end_buffer));
		if (send == -1) {
			return -1;
		} else if (send == -2) {
			return -2;
		} else if (send == -3) {
			return -3;
		}
	}
	return 0;
}
/*
Recieves an error message to be sent to the server. Sets this message in the error_package 
struct, and memcopies the struct to the generic_error_buffer, and calls the function in
netcom who is responsible for sending a package over the network
*/
int send_error_package(char* error_msg, char* ip, char* port) 
{
	error_package tmp;
	memset(&generic_error_buffer, 0, sizeof(generic_error_buffer));
	tmp.msg_type = 0x10;
	tmp.free = 0;
	strcpy(tmp.error, error_msg);
	if (endianess == LITTLEENDIAN) {
		tmp.msg_size = htons(sizeof(error_package));
	}
	memcpy(&generic_error_buffer, &tmp, sizeof(generic_error_buffer));
	int send = send_pkg(port, ip, generic_error_buffer, sizeof(generic_error_buffer));
	if (send == -1) {
		return -1;
	} else if (send == -2) {
		return -2;
	} else if (send == -3) {
		return -3;
	}
	return 0;
}