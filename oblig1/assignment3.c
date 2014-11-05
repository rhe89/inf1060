#include "assignment3.h"
int main (void){
	/*Test
	Oppretter et nytt minneområdet (char* buffer). Deretter oppretter jeg en text_t-peker
	som skal peke på hvor i minnet tekst skal begynne å lagres. Teksten vil ikke
	lagres i denne structen, men strukten er en peker til den neste plassen i
	det opprettede minneområdet hvor noe kan lagres. Structen blir på en måte
	en guide i minnet så det som kan lagres kan finne ut hvor neste lagringsplass er.
	Oppretter deretter en tekst som jeg lagrer i minneområdet
	*/
	char *buffer = malloc(CONTAINER_SIZE);
	text_t *ptr = text_init(buffer, CONTAINER_SIZE);
	text_store_entry(ptr, "hello", 6);
	char *fetch_text;
	ssize_t ret_v = text_fetch_entry(ptr, 0, fetch_text, 6);
	//Sjekker at størrelsen returnert er like stor
	assert(ret_v==6);
	free(buffer);
	return 0;
}
/*
Her initialiseres minneområdet. Det sjekkes først om størrelsen er større enn
området det skal lagres på, isåfall returneres -1. Deretter settes indeksene
på hvor noe skal lagres ved å trekke fra størrelsen på minneområdet og størrelsen
på den første plassen i minneområdet. Så initialiseres alle plassene, som settes
til 0 slik at det indikirer at det ikke er noe lagret tekst der enda.
Minneområdet sin index for hvor noe skal lagres settes til 0, siden ingen tekst er 
lagret enda. Adressen til den siste plassen i settes, og adressen til der noe
skal lagres neste gang settes også.
*/
text_t* text_init(void *memory, size_t size) {
	if (&memory<size){
		return -1;
	}
	text_t* the_struct = memory;
	the_struct->container_size = size;
	
	the_struct->max_index = (sizeof(the_struct->container)/sizeof(the_struct->container[0]) );
	
	int i;
	for (i = 0; i<10; i++){
		the_struct->container[i] = 0;
	}
	the_struct->storage_index = 0;
	the_struct->max_adress = the_struct+sizeof(memory);
	the_struct->next_storage_point = the_struct;
	return the_struct;
}
/*
Først sjekkes det om størrelsen på det som skal lagres er større enn det det er plass til
i minneområdet. Deretter sjekkes det om indeksen i minneområdet er større enn max-indeksen, 
det vili såfall bety at området er fullt og det kan ikke legges inn noe mer.
Så brukes memcpy til å kopiere det som er på adressen til input-parameteren til
det som står på adressen til nåværende lagringspunkt i minneområdet.
Indeksen returneres så, og økes med en for å gjøre klart til at noe kan lagres på
neste indeks i minneområdet.
*/
int text_store_entry(text_t *txt, const char *input, size_t input_sz) {
	if ( input_sz > (txt->max_adress - txt->storage_index) ){
		return -1;
	}
	if (txt->storage_index >= txt->max_index){
		return -1;
	}	
	memcpy(&txt->container[txt->storage_index] , &input, input_sz);
	printf("Text stored: %s\n", txt->container[txt->storage_index]);
	return (txt->storage_index)++;
}
/*
Her skal noe hentes fra en gitt plass i minneområdet mitt. Den angitte indexen sjekkes opp mot
gjeldende peker til minneområdets index, og hvis sjekken ikke returnerer funksjonen kopieres
det som er lagret på den adressen over i adressen til parameter-pekeren. Den skrives så ut for 
å sjekke at riktig ble hentet, før størrelsen på det som ble lagret blir returnert.
*/
ssize_t text_fetch_entry(text_t *txt, int index, char *output, size_t output_sz) {
	if (index < txt->storage_index && index < 0){
		return -1;
	}
	memcpy(&output, &txt->container[index], output_sz);
	printf("Text fetched: %s\n", output);
	return output_sz;
}
/*Det som er lagret på den gitte plassen slettes slik at det kan lagres nye
ting der
*/
void text_delete_entry(text_t *txt, int index) {
	txt->container[index] = NULL;
	return;
}