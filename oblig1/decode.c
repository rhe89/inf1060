#include "decode.h"
#include "test_argument.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

FILE *inp_f;
FILE *out_f;

int main(int argc, char *argv[], char *env[]) {
	FILE *tmp_f = NULL;

	//Sjekker at filnavn, input og output fil er med
	if (argc < 3) {
		printf("Error: [file] [input_file] [output_file]\n");
	}

	inp_f = stdin;
	if (argc > 1 && (tmp_f = test_argument(argv[1], 0)) != NULL) {
		printf("Reads from fil: %s\n", argv[1]);
		inp_f = tmp_f;
	} else {
		printf("Reads from stdin\n");
	}
	out_f = stdout;

	if (argc > 2 && (tmp_f = test_argument(argv[2], 1)) != NULL) {
		printf("Writes to file: %s\n", argv[2]);
		out_f = tmp_f;
	} else {
		printf("Writes to stdout\n");
	}

	//Initsierer lesing fra inp_p (filen som skal de-komprimeres):
	read_input_file(inp_f);

	//Lukker filene før programmet termineres
	fclose(inp_f);
	fclose(out_f);
	return 0;
}

/*Denne funksjonen leser en og en char i filen, deretter kaller den
på funksjonen som starter å decode charenes binær-verdi.
Den leser til den kommer til End of File.
*/
void read_input_file(FILE* file) {
	int current_char;

	while ((current_char = fgetc(file)) != EOF) 
	{
		decode_input_file(current_char);
	}
}

/*Her skjer selve decodingen. En int-array på str 2 lagrer int-verdier (binærverdiene 0 eller 1)
som tilsvarer 2*4 av de 8 bitsene char-parameteren består av. Deretter kjøres
en for-løkke for å gå gjennom en og en bit i charen. For å få tak
i binærverdien til hver av de 8 bitsene til charen, kjøres en bit-operasjon som
får tak i disse. Når arrayen  er fyllt opp med to tall (0 eller 1), kjøres 
funksjonen som returnerer de to binærverdienes char (utifra "ASCII-tabell versjon INF1060" i oppgaveteksten). 
Disse skrives så ut til out_f.
*/
void decode_input_file(char current_char) {
	int bin_values[2];
	int i = 0;
	int counter = 0;
	int bin_value;
	
	char *decoded;
	for (i = 7; i >= 0; i--)
	{
		if (counter == 2) {
			//Konverterer de to tallene i int-arrayen
			decoded = convert_to_char(bin_values);
			//Skriver den konverterte charen til fil
			fprintf(out_f, "%s", decoded);
			//Nullstiller telleren slik at den kan fylle opp nye binærtall
			counter = 0;
		}
		//Bit-operasjon
		bin_value = (current_char >> i) & 0x01;
		//Fyller int-arrayen med binærtall (0 eller 1)
		bin_values[counter] = bin_value;
		counter++;	
	}
	/*Kjorer decodingen en siste gang etter at for-løkka er ferdig
	for å fylle den siste indexen, som ikke blir fylt i for-løkka (kun 7/8 plasser
	blir fylt) og skriver ut denne
	*/
	decoded = convert_to_char(bin_values);
	fprintf(out_f, "%s", decoded);
}

/*Her sjekkes hver binærrekke (på 2) opp mot "ASCII-tabell versjon INF1060".
Den sjekker først om første tall er 0 eller 1, deretter om andre tall er 0 eller 1,
og returnerer så tilsvarende char for disse to tallene.
*/
char* convert_to_char(int i[]) {
	if (i[0] == 0) {
		if (i[1] == 0) {
			return " ";
		} else if (i[1] == 1) {
			return ":";
		}
	} else if (i[0] == 1) {
		if (i[1] == 0) {
			return "@";
		} else if (i[1] == 1) {
			return "\n";
		}
	}
	return NULL;
}