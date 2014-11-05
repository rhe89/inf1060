#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <sys/types.h>
#include <stdlib.h>

struct text_storage typedef text_t;
text_t* text_init(void *memory, size_t size);
int text_store_entry(text_t *txt, const char *input, size_t input_sz);
ssize_t text_fetch_entry(text_t *txt, int index, char *output, size_t output_sz);
void text_delete_entry(text_t *txt, int index);

#define LAST_STORE (4)

void *mean_malloc(size_t sz, const void *caller) {
	return NULL;
}

int main(int argc, char *argv[]) {

	char *b1 = malloc(1024 * 12);
	char *b2 = malloc(1024 * 4);
	if (!b1 || !b2) {
		fprintf(stderr, "System is out of memory, can not run test!\n");
		return 0;
	}
	memset(b1, 0, (1024 * 12));
	memset(b2, 0, (1024 * 4));
	
	// Makes malloc return - NULL - 
	//__malloc_hook = mean_malloc;

	const char *t_st[5] = {
		"This is a string we'll use for testing",
		"This string is a bit longer, notice it is "
			"spaned over more than one line.",
		"And a short one.",
		"For some reason, no one likes to store "
			"long strings in c.\x0 And for"
			"some reason, the assignment "
			"creator is making your life "
			"really misarable!",
		"The cake is a lie!"};
	size_t t_sz[5] = {39, 74, 17, 140, 19};


	text_t *nr[2];
	nr[0] = text_init(b1, (1024 * 12));
	nr[1] = text_init(b2, (1024 * 4));


	int cnt;
	int store[(((1024 * 12) / 17) + 1) << 1];
	int did[2];
	int doing[2];
	size_t freed[2];
	size_t stored[2];
	for (cnt = 0; cnt < (sizeof(doing) / sizeof(*doing)); cnt++) {
		did[cnt] = 0;
		freed[cnt] = 0;
		stored[cnt] = 0;
		if (nr[cnt]) {
			doing[cnt] = 1;
		} else {
			fprintf(stderr, "Failed to initiate storage nr[%d]\n", cnt + 1);
		}
	}

	if (!nr[0] && !nr[1]) {
		fprintf(stderr, "Could not initiate any of the storages\n");
		return 0;
	}

	for (cnt = 0; cnt < (sizeof(store) / sizeof(*store)); cnt++) {
		if (doing[(cnt % 2)])
			store[cnt] = text_store_entry(nr[(cnt % 2)], t_st[(cnt % 5)], t_sz[(cnt % 5)]);
		else
			store[cnt] = -1;
		if (store[cnt] < 0 && doing[(cnt % 2)]) {
			doing[(cnt % 2)] = 0;
			fprintf(stderr, "Managed to store %d strings (%zu bytes) in nr[%d].\n",
				did[(cnt % 2)], stored[(cnt % 2)], (cnt % 2) + 1);
		} else if (doing[(cnt % 2)]) {
			did[(cnt % 2)]++;
			stored[(cnt % 2)] += t_sz[(cnt % 5)];
		}
	}
	for (cnt = 0; cnt < (sizeof(store) / sizeof(*store)); cnt++) {
		if (t_sz[(cnt % 5)] == 17 && store[cnt] >= 0) {
			text_delete_entry(nr[(cnt % 2)], store[cnt]);
			freed[(cnt % 2)] += t_sz[(cnt % 5)];
		}
	}
	for (cnt = 0; cnt < (sizeof(doing) / sizeof(*doing)); cnt++) {
		fprintf(stderr, "Freed %zu bytes from nr[%d]\n", freed[cnt], cnt + 1);
		if (0 > text_store_entry(nr[cnt], t_st[LAST_STORE], t_sz[LAST_STORE])) {
			fprintf(stderr, "But could not store %zu bytes (not failed).\n", t_sz[LAST_STORE]);
		}
	}
	
	return 0;
}
