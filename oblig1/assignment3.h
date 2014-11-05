#ifndef INF1060_ASSIGNMENT3
#define INF1060_ASSIGNMENT3
// ssize_t
#include <sys/types.h>
// NULL
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

//Setter buffer-størrelsen til en vilkårlig størrelse
#define CONTAINER_SIZE 512
struct text_storage {
	//Denne lagrer størrelsen på bufferen
	int container_size;
	//Denne inneholder pekere til tekststrenger bortover i minneområdet
	char* container[20];
	//Denne inneholder endepunktet på adressen hvor noe skal lagres
	int max_adress;
	//Denne inneholder max index hvor noe kan lagres i minneområdet
	int max_index;
	//Denne inneholder hvor neste "lagringsplass" i minneområdet er
	int next_storage_point;
	//Denne inneholder den indeksen hvor noe skal lagres i minneområdet
	int storage_index;
} typedef text_t;
/** text_init
 * Arguments:
 *  Pointer to unused memory and its size.
 * Returns:
 *  Initiated instance of a text storage container.
 * OR
 *  NULL on errors (insufisient space to create a container).
 */
text_t* text_init(void *memory, size_t size);
/** text_store_entry
 * Arguments:
 *  Instance of text storage container to use.
 *  Pointer to data/text and its size.
 * Returns:
 *  The index used to later fetch the entry.
 * OR
 *  -1 (negative) on error (out of space, no container, etc.).
 */
int text_store_entry(text_t *txt, const char *input, size_t input_sz);
/** text_fetch_entry
 * Arguments:
 *  Instance of text storage container to use.
 *  Index of text/data entry we would like to fetch.
 *  Output buffer we would like the data written into, and the buffer size.
 * Returns:
 *  Number of bytes written to the buffer.
 * OR
 *  -1 (negative) on error (no such index, etc.)
 */
ssize_t text_fetch_entry(text_t *txt, int index, char *output, size_t output_sz);
/** text_delete_entry
 * Arguments:
 *  Instance of text storage container to use.
 *  Index of text/data to remove from the storage.
 */
void text_delete_entry(text_t *txt, int index);
#endif