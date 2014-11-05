/*
Header for the bit_handling.c file
*/

#ifndef BITHANDLING
#define BITHANDLING
char get_8bit_from_16bit(short convert);
char define_game_type(short value);
int compress_to_32bit(short score, short killed, short survivors, short free_space, short reserved);
int get_killed(int line);
int get_result(int line);
int get_survivors(int line);
#endif