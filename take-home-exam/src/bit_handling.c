#ifndef BITHANDLING

#define BITHANDLING

#include <stdio.h>
#include <stdlib.h>

/*
Defines all the game types
*/
#define SINGLE_TEST 0x00
#define SINGLE1 	0x01
#define SINGLE2 	0x02
#define SINGLE3 	0x03
#define SINGLE4 	0x04
#define SINGLE5 	0x05
#define SINGLE6 	0x06
#define SINGLE7 	0x07
#define SINGLE8 	0x08
#define SINGLE9 	0x09
#define SINGLE_VAR  0x0A
#define MULTI1 		0x11
#define MULTI2 		0x12
#define MULTI3 		0x13
#define MULTI4 		0x14
#define MULTI5 		0x15
#define MULTI6 		0x16
#define MULTI7 		0x17
#define MULTI8 		0x18
#define MULTI9 		0x19
#define MULTI_VAR 	0x1A
#define TEAM1 		0xA1
#define TEAM2 		0xA2
#define TEAM3 		0xA3
#define TEAM4 		0xA4
#define TEAM5 		0xA5
#define TEAM6 		0xA6
#define TEAM7 		0xA7
#define TEAM8 		0xA8
#define TEAM9 		0xA9
#define TEAM_VAR 	0xA

/*
Checks if the value isn't 0 when 8 bits of the 16 is
shifted to the right, and returns -1 if so.
If else, returns a char (8 bit) of the 16 bit (the first 8 from 
right to left)
*/
char get_8bit_from_16bit(short convert) 
{
	if (convert >> 8 != 0) {
		return -1;
	}
	return convert;
}
/*
Checks the game type, and returns the correct hexa
*/
char define_game_type(short value) {
	if (value == 0) {
		return SINGLE_TEST;
	} else if (value == 1) {
		return SINGLE1;
	} else if (value == 2) {
		return SINGLE2;
	} else if (value == 3) {
		return SINGLE3;
	} else if (value == 4) {
		return SINGLE4;
	} else if (value == 5) {
		return SINGLE5;
	} else if (value == 6) {
		return SINGLE6;
	} else if (value == 7) {
		return SINGLE7;
	} else if (value == 8) {
		return SINGLE8;
	} else if (value == 9) {
		return SINGLE9;
	} else if (value == 10) {
		return MULTI_VAR;
	}
	return -1;
}
/*
For the third line in the point package. Bitshifts x numbers of field for each requred bits:
Score will be sat at the left side with 4 bits, then killed, survivors and free_space with 8 bits, and
at last reserved with the remaining 4 bits 
*/
int compress_to_32bit(short score, short killed, short survivors, short free_space, short reserved) 
{
	return (score << 28) | killed << 20 | survivors << 12 | free_space << 4 | reserved;
}
/*
Returns the value on the 32 bit integer that represents killed players in the points package protocol
*/
int get_killed(int line) 
{
	int killed = line << 4;
 	killed = killed >> 24;
 	return killed;
}
/*
Returns the value on the 32 bit integer that represents the score in the points package protocol
*/
int get_result(int line) 
{
	int score = line >> 28;
	return score;
}
/*
Returns the value on the 32 bit integer that represents the survivors in the points package protocol
*/
int get_survivors(int line) 
{
 	int survivors = line << 12;
 	survivors = survivors >> 24;
 	return survivors;
}

#endif