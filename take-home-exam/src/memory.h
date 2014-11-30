/*
Header for the memory.c file
*/

#ifndef MEMORY
#define MEMORY

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h> 
#include <string.h>
#include <signal.h>

#include "structs.h"

typedef struct memory_area 
{
	int game_type;
	points_package max_score;
	int min; 
	int max;
	int numb_packages;
	points_package min_score;
} mem_area;

/*
Array for all the memory areas available for storing packages
of different game types
*/
mem_area *memory_areas[31];

void 	print_file();
int 	add_to_memory(char *buf, int file_check);
int 	check_if_package_fits(mem_area *tmp, char *candidate);
void 	check_lowest(mem_area* area, char* candidate);
void 	check_highest(mem_area* area, char* candidate);
void 	replace_lowest_score(mem_area *tmp, char* candidate);
void 	free_mem_allocs();
points_package get_pkg_from_index(int i);
int 	get_filled_areas();
void 	save_to_file(char* file_name, int append);
#endif
