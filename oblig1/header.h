#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/*Lager et alias for structen Elem, som inneholder en peker til neste linje
samt innholdet på nodens linje.
*/
typedef struct Elem
{
	char line[50];
	struct Elem *next;
} Elem;

/*
Får et argument 1 eller 0 i tillegg til navnet (eller mangel på) på filen
som tilsier om filen skal leses fra eller skrives til
*/
FILE* test_argument(char *name, int checkState) {
	if (!name || (strcmp(name, "-") == 0)) {
		return NULL;
	}
	FILE* file;
	/*Klargjor filen for lesing eller skriving
	Returnerer NULL om filen ikke finnes, og gjør dermed tastaturet/terminalen
	klar for input/output
	*/
	if (checkState == 0) {
		file = fopen(name, "r");
	} else if (checkState == 1) {
		file = fopen(name, "w");
	}
	
	if (!file) {
		return NULL;
	}
	return file;
}

//Funksjoner som brukes i .c-filen
void put(char line[50], Elem* n);
void print_text(Elem* n);
void print_random_line(Elem* n);
unsigned int make_random_int(unsigned int min, unsigned int max);
void print_text_without_vowels(Elem* n);
int vowel_check(char c);
void replace_vowels_in_text(Elem* n);
void print_length_of_file(Elem* n);
void clear_memory(Elem* n);