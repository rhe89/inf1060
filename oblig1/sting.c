/*
Av: Roar Hoksnes Eriksen (roarher)
Obligatorisk Oppgave 1 - INF1060, Høst 2013

Kommentar til oppgavene:
Har fjernet alle kommentarer fra precoden for å få bedre oversikt over min egen kode.
All kode er kommentert med forklaring. 
*/

//Dette er header-filen med alle funksjon- og struct-deklarasjoner som jeg trenger
//i denne oppgaven
#include "header.h"

char vowels[] = {'a', 'e', 'i', 'o', 'u', 'y'};
char line[50];
FILE *inp_f;
FILE *out_f;

//Første argument er antall argumenter fra kommandolinjen
int main(int argc, char *argv[]) 
{
	
	FILE *tmp_f = NULL;
	/*
	Lager rot-noden som er peker til første element i listen
	*/
	Elem *head = malloc(sizeof(Elem));

	/*
	Sjekker om tredje argument er en fil, hvis det er det opprettes denne
	og filens innhold leses inn. Hvis ikke (- eller tom) leses det som skrives
	i terminal
	*/
	inp_f = stdin;
	if ((tmp_f = test_argument(argv[2], 0)) != NULL) {
		inp_f = tmp_f;
	} 
	/*
	Hvis tredje argument ikke er en fil, altså at det skal leses fra tastatur
	gis det beskjed om at CTRL-D må trykkes når man er ferdig med
	teksten og vil utføre parameter-kommandoen
	*/
	if (!argv[2] || *argv[2] == '-') 
	{
		printf("Finish typing by pressing CTRL-D\n");
	}

	
	/*Sjekker om fjerde argument er en fil, hvis det er det opprettes denne
	og kommandoen skrives til fil. Hvis det ikke er en fil (- eller tom) skrives
	kommandoen ut i terminal
	*/
	out_f = stdout;
	if ((tmp_f = test_argument(argv[3], 1)) != NULL) {
		out_f = tmp_f;
	} 

	/*
	For hver gang bruker trykker enter legges en ny linje inn i den lenkede listen.
	Når ctrl-d blir trykket, om bruker har tastet en kommando som andre parameter,
	blir kommandoen utført. Det samme gjelder for tekstfilen om den er tredje parameter.
	Da legges hver linje inn i en lenket liste. For at begge deler skal fungere
	må det sjekkes om antall argumenter er under eller lik 3 for å lese inn en fil, eller om
	stdin er blitt avsluttet (ved CTRL-D) og at størrelsen dermed er 0.
	*/
	while ((fgets(line, sizeof(line), inp_f) != NULL && argc <= 4) || ((!(sizeof(inp_f))))) 
	{	
		put(line, head);
	} 
	//Gir feilmelding for å unngå programkrasj om en kommando ikke er gitt som andre argument
	if (argc == 1) 
	{
		printf("Remember to type [command] as second argument if you want to read from keyboard!\n");
		exit(1);
	}
	//Sjekker hvilken kommando som er gitt, og utfører funksjonen
	//for valgte kommando
	if (strcmp(argv[1], "print") == 0) 
	{
		print_text(head);
	} 
	else if (strcmp(argv[1], "random") == 0) 
	{
		print_random_line(head);
	}
	else if (strcmp(argv[1], "remove") == 0) 
	{
		print_text_without_vowels(head);
	}
	else if (strcmp(argv[1], "replace") == 0) 
	{
		replace_vowels_in_text(head);
	}
	else if (strcmp(argv[1], "len") == 0) 
	{
		print_length_of_file(head);
	} else {
		printf("UNKOWN COMMAND\n");
		exit(1);
	}

	clear_memory(head);
	fclose(inp_f);
	fclose(out_f);
	return 0;
}

/*
Legger til en ny linje i lista. Bruker to Elem-pekere
og mallocerer en peker av root-node, og setter elem-noden
lik parameter-noden (den første Elemn i lista ved første kall på funksjonen).
*/
void put(char line[50], Elem* n) {
	Elem *tmp;  
	tmp = n;

	while(tmp->next != NULL) {
		tmp = tmp->next; 
	}

	Elem *root = malloc(sizeof(Elem));  
	strcpy(root->line, line);  

	root->next = NULL;  
	tmp->next = root;
}
/*
Frigjor hver peker i lista.
Ved å alltid lagre noden som skal frigjores
i en tmp-peker, samt ha en annen (Elem *node) som oppdaterer 
neste-pekeren, vil det vaere mulig å frigjore hver node
etter hvert som lista traverseres. node blir tom når
adressen den peker på frigjores, men siden node da 
allerede er satt til å peke paa neste element i lista 
vil dette tillate løkka å gå selv om den "nåværende"
pekeren blir frigjort
*/
void clear_memory(Elem *n) {
	Elem *tmp;
	Elem *node;
	for (node = n; node != NULL; node = node->next) {
		tmp = node;
		strcmp(tmp->line, "");
		free(tmp);
	}
}
/*
Traverserer listen, og skriver ut hver node sin linje
ved å oppdatere nestepekeren for hver gang løkka går
*/
void print_text(Elem* n)  
{  
	Elem* tmp;
	fprintf(out_f, "PRINTED TEXT:\n");
	for (tmp = n; tmp != NULL; tmp = tmp->next) {
		fprintf(out_f, "%s", tmp->line);  
	}  
	fprintf(out_f, "\n");
}
//Skriver ut en random linje. Lagrer alle linjene paa en indeks
//i en array, skriver ut en vilkårlig indeks av arrayen
void print_random_line(Elem* n)  {  
	int counter = 0;
	Elem* tmp;

	//Teller antall linjer i lenkelisten
	for (tmp = n; tmp != NULL; tmp = tmp->next) {
		counter++;
	}
	//Setter en int random_nr som er mellom 0 og telleren
	int random_nr = make_random_int(0,counter);
	srand(time(NULL));

	//Setter telleren til 0 etter at random er generert
	
	counter = 0;	
	/*Traverserer listen, øker nestepekeren for hver gang.
	Om linjen ikke er tom og telleren er lik random_nr
	skrives linjen på plass counter ut
	*/
	tmp = NULL;
	for (tmp = n; tmp != NULL; tmp = tmp->next) {
		if (strcmp(tmp->line, "") != 0) {
			if (counter++ == random_nr) {
				fprintf(out_f, "%s\n", tmp->line);	
			}
		}
	}
}
/*Funksjon som bruker pcens klokke for å finne en random verdi mellom
min og max. Det nærmeste jeg kom i å finne en random int-generator
*/
unsigned int make_random_int(unsigned int min, unsigned int max)
{
  unsigned int iseed = (unsigned int)time(NULL);
  
  srand(iseed);
  
  return ((rand () % ((max-min))));
}

/*
Løper gjennom hver char i hver linje for å sjekke den opp
mot vokal-arrayen. Hvis char'en ikke er en vokal, skrives den ut
Neste-pekeren økes etter hver linje for å skrive ut den neste linje.
*/
void print_text_without_vowels(Elem* n)  
{  
	fprintf(out_f, "TEXT WITH NO VOWELS\n");
	int k = 0;
	char noVowels[50];
	Elem* tmp;
	for (tmp = n; tmp != NULL; tmp = tmp->next) {
		if (strlen(tmp->line) != 0) {
			for (k = 0; tmp->line[k] != 0; k++) {
				if (vowel_check(tmp->line[k]) == 1) {
					noVowels[k] = tmp->line[k];
					fprintf(out_f, "%c", noVowels[k]);
				}
			}
		}
	}
	fprintf(out_f, "\n");
}

/*
Starter ved å løpe gjennom hver char i vokal-arrayen.
Sjekker forst om noden er null, deretter sjekkes det om
linjen er tom, og gjennomløpningen av lista starter ved å sjekke
hver char i hver linje. Hvis denne ikke er en vokal, legges den direkte til
i noVowels-arrayen og skrives ut, hvis den er det, legges det til en 
vokal fra vowels-arrayen og skriver ut denne. Øker deretter neste-pekeren
for å sjekke neste linje.
*/
void replace_vowels_in_text(Elem* n) {  
	int k = 0;
	int i = 0;
	char noVowels[200];

	for (i = 0; i < 6; i++) {
		Elem* tmp;
		fprintf(out_f, "REPLACED VOWELS WITH %c:\n", vowels[i]);
		for (tmp = n; tmp != NULL; tmp = tmp->next) {
			if (strlen(tmp->line) != 0) {
				for (k = 0; tmp->line[k] != 0; k++) {
					if (vowel_check(tmp->line[k]) == 1) {
						noVowels[k] = tmp->line[k];
						fprintf(out_f, "%c", noVowels[k]);
					} else {
						noVowels[k] = vowels[i];
						fprintf(out_f, "%c", noVowels[k]);
					}
				}
			}
		}
		fprintf(out_f, "\n");
	}
}
/*Sjekker om parameter-charen er en vokal ved 
aa sammenligne verdien dens med verdiene
i vokal-arrayen. Returnerer 0 for false og
1 for true
*/
int vowel_check(char c) {
	int i;
	for (i = 0; vowels[i] != 0; i++)
	{
		if (c == vowels[i]) {
			return 0;
		}
	}
	return 1;
}
/*Løper gjennom listen, øker length for hver karakterer for hver linje
og øker pekeren for å gå gjennom neste linje
*/
void print_length_of_file(Elem* n)  
{  
	int k = 0;
	int length = 0;
	Elem* tmp;
	for (tmp = n; tmp != NULL; tmp = tmp->next) {
		for (k = 0; tmp->line[k] != 0; k++) {
			length++;
		}
	}
	fprintf(out_f, "Length of file is: %d\n", length);
}