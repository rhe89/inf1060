/*
 Struct for point packages
 The third line containing amongst others two 4 bit fields
 is set as an int, and will be bitshifted when set and interpreted
 to get the actual values stored in it.
 The other fields are variables of the rights bit-size required in the message protocol
 */
typedef struct points {
	short msg_size;
	char msg_type;
	char game_type;
	short points;
	char teammates;
	char oppositions;
	int line_3;
	char name[8];
} points_package;
/*
 Struct for highscore list packages to be sent back and forth from client and server
 */
typedef struct highscore {
	short msg_size;
	char msg_type;
	char numb_packages;
} highscore_package;
/*
 Struct for error message packages to be sent back and forth from client and server
 */
typedef struct error {
	short msg_size;
	char msg_type;
	char error[200];
	char free;
} error_package;

/*
For players that are ready to play. I have set this package type's message type
to be 0x02
*/
typedef struct setup {
	short msg_size;
	char msg_type;
	short playerID;
} setup_package;

/*
For players that have startet to play. Message type 0x03
*/
typedef struct start {
	short msg_size;
	char msg_type;
	short playerID;
} start_package;

/*
For players that have made a direction change. Message type 0x04
*/
typedef struct update {
	short msg_size;
	char msg_type;
	short direction;
	short playerID;
} update_package;

/*
For players that have ended a game. Message type 0x05
*/
typedef struct end {
	short msg_size;
	char msg_type;
	short playerID;
} end_package;
