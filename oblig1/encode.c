#include "encode.h"
/*Denne er lik for alle oppgaver, derfor er det deklarert i en egen
header-fil slik at jeg slipper de ekstra (unøvendige) linjene i hver c.-fil
*/
#include "test_argument.h"

FILE *inp_f;
FILE *out_f;

int main(int argc, char *argv[], char *env[]) {
	FILE *tmp_f = NULL;

	//Sjekker at filnavn, input og output fil er med, og ingenting
	//mer eller mindre
	if (argc > 3) {
		printf("Error: [file] [input_file] [output_file]\n");
		exit(0);
	}

	inp_f = stdin;
	if (argc > 1 && (tmp_f = test_argument(argv[1], 0)) != NULL) {
		printf("Leser fra fil: %s\n", argv[1]);
		inp_f = tmp_f;
	} else {
		printf("Leser fra stdin\n");
	}
	out_f = stdout;

	if (argc > 2 && (tmp_f = test_argument(argv[2], 1)) != NULL) {
		printf("Skriver til fil: %s\n", argv[2]);
		out_f = tmp_f;
	} else {
		printf("Skriver til stdout\n");
	}

	read_input_file(inp_f);

	fclose(inp_f);
	fclose(out_f);
	return 0;
}

/*Leser in-filen char for char, til man er på slutten av filen. 
Lager en char-array med plass til 4 chars, som skal sendes
til komprimeringsfunksjonen. Naar arrayen er fyllt opp, og teller er lik
4, skrives den komprimerte charen (som gjøres i funksjonen under denne)
til fil, og telleren nullstilles slik at arrayen kan fylles opp på nytt med fire nye
chars fra fila. 
*/
void read_input_file(FILE* file) {
	int current_char;
	//Char array som skal inneholde fire chars som skal sendes til komprimering
	char char_collection[4];
	int counter = 0;

	while ((current_char = fgetc(file)) != EOF) 
	{
		if (counter == 4) {
			//Skriver ut den komprimerte charen
			encode_input_file(char_collection);
			counter = 0;
		}
		char_collection[counter] = current_char;
		counter++;
	}
}
/*
Mallocer plass til en char med størrelsen til 8 chars. Dette fordi
denne skal "adderes" opp med flere chars i strcat()-funksjonen. For-løkken
går igjennom char-arrayen som kommer som parameter, og converterer hver char
i arrayen ( , :, @ eller \n) til binærverdien i forhold til 
"ASCII-tabell versjon INF1060". Denne binærverdien fylles så opp i den mallocerte
charen, til den til slutt er fyllt opp med 2*4=8 tall. Ved hjel av strtol, som komprimerer
denne tallrekken på 8 til en ny charverdi, returneres den komprimerte charen. 
*/
void encode_input_file(char current_chars[]) {
	int i;
	//Mallocer en char som skal inneholde 8 binærtall
	char* binary = malloc(sizeof(char));
	for (i = 0; i < 4; i++) {
		/*Converterer charen på plass i til et tosifret binærtall
		Legger den konverterte charen sammen med den mallocerte charen
		slik at den til slutt består av 8 tall når løkken er ferdig.
		*/
		strcat(binary, convert_to_binary(current_chars[i]));
	}
	//Komprimerer den mallocerte charen (bestående av åtte tall) til en ny char
   	char a = strtol(binary, 0, 2);
	fprintf(out_f, "%c", a);
	/*Sjekker om den mallocerte charen er full, i så fall free'es denne
	for å unngå memory leaks
	*/
	if (strlen(binary) == 8) {
		free(binary);
	}
}
/*
Får inn en char, sjekker denne opp mot "ASCII-tabell versjon INF1060", og returnerer
binærverdien oppført i tabellen
*/
char* convert_to_binary(char current_char) {
	if (current_char == ' ') 
	{
		return "00";
	} 
	else if (current_char == ':') 
	{
		return "01";
	} 
	else if (current_char == '\n') 
	{
		return "11";
	} 
	else if (current_char == '@') {
		return "10";
	}
	return NULL;
}