#include "memory.h"
#include "netcom.h"
#include "bit_handling.h"

/*
NOTICE: I have chosen to use memcpy quite often to add values to the memory area
declared. When using pointers, i cast the pointer to each memory area to a long (
to get the start of the memory area), and then casts this long to a void to avoid
warnings when compiling. I then add the size of the memory area to get to the point
where i can start storing point packages, and then adds the number of packages (x * point_packages)
to get to the point where the next free spot in the current memory area is for storing a new
highscore. Example:

To get the next insertion point:
((void*)(long)point to the_area)+size of the area+(number of packages*size of point packages))

When copying to an adress, this is the first parameter in memcpy(), when copying from
an adress and to a point_package or char* this is the second parameter
Since i do this quite often, i comment it here, so it doesn't have to be 
repeated every time i do it. 
When using point_package variables (e.g. max_score and min_score), 
i memcpy directly to the variable. When using other variables, i use the = operator.

The memory_area struct is set up as follows:
I use a point_package variable and an integer variable for storing the highest and lowest score in
memory. These variables are set at insertion of a new package.
I also have an integer variable keeping track of the number of point_packages in memory, as well as
a variable for storing the mem_areas game type.

At insertion, as mentioned, i use the number of packages variable to see where i can put a new point_package.
*/


/*
Array for storing all the different kinds of games. This is used
to find out if a memory area is already assigned to a specific 
game type
*/
int MAX_SIZE = 12;
int MEM_COUNT;
int file_check = 0;

/*
Printing a memory areas scores, for test purposes
*/
void print_file() {
	int i;
	int j;
	for (i = 0; i < MAX_SIZE; i++) {
		mem_area *tmp = memory_areas[i];
		if (tmp != NULL) {
			printf("\n\n-------------------------------\n");
			printf("SCORES FOR GAME TYPE %d\n", tmp->game_type);
			for (j = 0; j < tmp->numb_packages; j++) {
				int increase = sizeof(points_package) * j;
				points_package pkg;
				memcpy(&pkg, (void*)((long)tmp) + sizeof(mem_area)+increase, sizeof(points_package));
				printf("%d %s \n", pkg.points, pkg.name);
			}
		}
	} 
}
/*
Saves each memory_areas list of highscores. Checks the append-variable from the highscoreserver-file
to see if the file should be appended or not. Then iterates through each memory area,
and prints the point_package-info to file
*/
void save_to_file(char* file_name, int append) {
	FILE* file = NULL;
	if (append == 0) {
		file = fopen(file_name, "w");
	} else {
		file = fopen(file_name, "a");
	}
	int i;
	int j;
	for (i = 0; i < MAX_SIZE; i++) {
		mem_area *tmp = memory_areas[i];
		if (tmp != NULL) {
			for (j = 0; j < tmp->numb_packages; j++) {
				int increase = sizeof(points_package) * j;
				points_package pkg;
				memcpy(&pkg, (void*)((long)tmp) + sizeof(mem_area)+increase, sizeof(points_package));
				int score = get_result(pkg.line_3);
				int survivors = get_survivors(pkg.line_3);
				int killed = get_killed(pkg.line_3);
				fprintf(file, "%d %d %d %d %d %d %d %d %s \n", pkg.msg_size, pkg.game_type, pkg.points, pkg.teammates, pkg.oppositions, score, 
				killed, survivors, pkg.name);
			}
		}
	} 
	fclose(file);
}
/*
Recieves an integer from the server who queries the highest score from memory area[i]
*/
points_package get_pkg_from_index(int i) 
{
	points_package pkg;
	memset(&pkg, 0, sizeof(points_package));
	if (memory_areas[i] != NULL) {
		memcpy(&pkg, &memory_areas[i]->max_score, sizeof(points_package));
	}
	return pkg;
}
/*
Return a counter for the number of memory_areas in memory
*/
int get_filled_areas() {
	int counter = 0, i;
	for(i = 0; i < MAX_SIZE; i++) {
		if (memory_areas[i] != NULL) {
			counter++;
		}
	}
	return counter;
}
/*
Checks if the current memory_area[index] is not malloced. If so, I malloc it,
and sets the size of the address space to contain one mem_area and 25 point packages.
I then add the first score to the start of the memory area (right after the actual mem_area struct)
and sets this mem_areas' values. file_check is used to print info on the screen,
i don't want to print this when adding from file, therefore file_check is set to 1,
when adding from a client it is set to 0. I then check if this score is lowest or highest (which
it is, since it is the first). If the memory area already is malloced, I first
check to see if it is full. If so, a check to see if this point_package's score
is higher than the current lowest, and either adds it or "trashes" it. If not full, I set an 
integer "next" to represent the current size of number of point packages, and adds the incoming
point_package here. I then check if this score is higher or lower than the current highest 
and lowest score. If the memory area isn't full, i increase the number of packages in memory
*/
int add_to_memory(char *gen_buf, int fc)
{
	char gt = gen_buf[3];
	int game_type = gt;
	int increase = 0;
	file_check = fc;
	if (memory_areas[game_type] == NULL) {
		memory_areas[game_type] = malloc(sizeof(mem_area) + (sizeof(points_package)*25));
		mem_area *tmp = memory_areas[game_type];
		tmp->game_type = gt;
		tmp->numb_packages = 1;
		tmp->min = 0;
		tmp->max = 0;
		memcpy((void*)((long)tmp) + sizeof(mem_area), gen_buf, sizeof(points_package));
		if (file_check == 0) {
			points_package pkg;
			memcpy(&pkg, (void*)((long)tmp) + sizeof(mem_area) + increase, sizeof(points_package));
			printf("New score added!\n");
			printf("Player %s scored %d points in game level %d!\n", pkg.name, pkg.points, pkg.game_type);
		} 
		check_lowest(tmp, gen_buf);
		check_highest(tmp, gen_buf);
	} else {
		mem_area *tmp = memory_areas[game_type];
		increase = sizeof(points_package) * tmp->numb_packages;
		if (tmp->numb_packages == 25) {
			if (check_if_package_fits(tmp, gen_buf)==-1) {
				return -1;
			} 
		} else {
			memcpy((void*)((long)tmp) + sizeof(mem_area) + increase, gen_buf, sizeof(points_package));
			if (file_check == 0) {
				points_package pkg;
				memcpy(&pkg, (void*)((long)tmp) + sizeof(mem_area) + increase, sizeof(points_package));
				printf("New score added!\n");
				printf("Player %s scored %d points in game level %d!\n", pkg.name, pkg.points, pkg.game_type);
			} 
			check_lowest(tmp, gen_buf);
			check_highest(tmp, gen_buf);
			if(tmp->numb_packages < MAX_SIZE) {
				tmp->numb_packages++;
			}
		}
	}	
	
	return 0;
}
/*
Checks if the incoming point_package is larger than the current largest score in memory,
or higher than the current lowest. If not, it is not added to memory. If it is the highest,
I call check_highest to replace the current highest score variable and calls replace_lowest_score
to overwrite the current lowest point_package in memory with the incoming point_package.
*/
int check_if_package_fits(mem_area *tmp, char *candidate) 
{
	points_package test;
	memcpy(&test, candidate, sizeof(points_package));
	int score_challenger = test.points;
	if (score_challenger > tmp->max) {
		memcpy(&tmp->max_score, candidate, sizeof(points_package));
		tmp->max = score_challenger;
		replace_lowest_score(tmp, candidate);
		check_highest(tmp, candidate);
	} else if (score_challenger > tmp->min) {
		replace_lowest_score(tmp, candidate);
		check_highest(tmp, candidate);
		return 0;
	}
	return -1;
}
/*
This functions is for overwriting the lowest score in memory with an incoming point_package.
To to this, i iterate through the current memory areas package, and compares each score with the
current lowest score variable, to get the position in memory of that score. I then check the incoming
point_package's score to see if it is larger than the current lowest. If so, it takes the lowest score's
place in memory, and resets the current lowest score to 0.
I then iterate through the point_packages in memory to set a new lowest score variable.
*/
void replace_lowest_score(mem_area *area, char* candidate) 
{
	int j;
	for (j = 0; j < area->numb_packages; j++) {
		points_package curr;
		points_package tmp;
		int increase = j * sizeof(points_package);
		memcpy(&curr, (void*)((long)area) + sizeof(mem_area) + increase, sizeof(points_package));
		if (curr.points == area->min) {
			memcpy(&tmp, candidate, sizeof(points_package));
			if (tmp.points > curr.points) {
				memcpy((void*)((long)area) + sizeof(mem_area) + increase, candidate, sizeof(points_package));	
				area->min = 0;
				break;
			}
		}
	}
	for (j = 0; j < area->numb_packages; j++) {
		int increase = j * sizeof(points_package);
		char buf[sizeof(points_package)];
		memcpy(&buf, (void*)((long)area) + sizeof(mem_area) + increase, sizeof(buf));
		check_lowest(area, buf);
	}
}
/*
Checks an incoming point_package's score to see if it is lower than the current lowest score variable in the current
memory area.
If no lowest score is stored, the incoming package is the new lowest score, or if it is smaller than the current
lowest score the incoming score is set to the incoming point_package's score.
*/
void check_lowest(mem_area* area, char* candidate) 
{
	points_package test;
	memcpy(&test, candidate, sizeof(points_package));
	if (area->min == 0) {
		memcpy(&area->min_score, candidate, sizeof(points_package));
		area->min = test.points;
	} else if (area->min > test.points) {
		memcpy(&area->min_score, candidate, sizeof(points_package));
		area->min = test.points;
	}
}
/*
Same procedure as for the check_lowest-function, the only difference is that the check is for the highest score
in memory
*/
void check_highest(mem_area* area, char* candidate) 
{
	points_package test;
	memcpy(&test, candidate, sizeof(points_package));
	if (area->max == 0) {
		memcpy(&area->max_score, candidate, sizeof(points_package));
		area->max = test.points;
		if (file_check == 0) {
			printf("New highest score added!\n");
		}
	} else if (area->max < test.points) {
		memcpy(&area->max_score, candidate, sizeof(points_package));
		area->max = test.points;
		if (file_check == 0) {
			printf("New highest score added!\n");
		}
	}
}
/*
Iterates through all the memory areas, and every memory area that is malloced, is free'd.
*/
void free_mem_allocs()
{
	int i;
	for (i = 0; i < 31; i++) {
		if (memory_areas[i] != NULL) {
			free(memory_areas[i]);
		}
	}
}
