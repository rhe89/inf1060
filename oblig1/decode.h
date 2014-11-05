#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//Funksjoner som brukes i .c-filen
void read_input_file(FILE* file);
void decode_input_file(char current_char);
char* convert_to_char(int i[]);