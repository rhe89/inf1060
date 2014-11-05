#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

//Funksjoner som brukes i .c-filen
FILE* test_argument(char *name, int checkState);
void read_input_file(FILE* file);
void encode_input_file(char current_chars[]);
char* convert_to_binary(char current_char);
