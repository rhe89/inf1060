#ifndef XXCODE_TEST_INF1060
#define XXCODE_TEST_INF1060
#include "compress_test.h"

void* writer(void *arg) {
	struct thread_arg *wt = arg;
	char test = 0x1b; // 0X1b = 0001 1011
	ssize_t res = write(wt->fd, &test, 1);
	close(wt->fd);
	if (res == 1) {
		wt->result = 0;
	} else if (res == -1) {
		wt->result = -1;
 	} else {
		// 4FUN: Do a "man 2 write" and try to figure out what the correct
		//         action in this if branch is.
	}
	return NULL;
}

void* reader(void *arg) {
	struct thread_arg *rd = arg;
	char res[4];
	memset(res, 0, sizeof(res));
	ssize_t now = 0, total = 0;
	while (sizeof(res) > total + (now =
		read(rd->fd, res + total, sizeof(res) - total))) {
		if (now < 0) {
			close(rd->fd);
			rd->result = -1;
			fprintf(stderr, "Could not read required amount\n");
			return NULL;
		}
		total += now;
		usleep(200);
	}
	rd->result = 0;
	if (res[0] != ' ') {
		fprintf(stderr, "Missmatch, for  ' ' (0x%2x), got 0x%x\n", ' ', res[0]);
		rd->result |= 0x1;
	}
	if (res[1] != ':') {
		fprintf(stderr, "Missmatch, for  ':' (0x%2x), got 0x%x\n", ':', res[1]);
		rd->result |= 0x2;
	}
	if (res[2] != '@') {
		fprintf(stderr, "Missmatch, for  '@' (0x%2x), got 0x%x\n", '@', res[2]);
		rd->result |= 0x4;
	}
	if (res[3] != '\n') {
		fprintf(stderr, "Missmatch, for '\\n' (0x%02x), got 0x%x\n", '\n', res[3]);
		rd->result |= 0x8;
	}
	close(rd->fd);
	return NULL;
}
#else
#warning Test functions allready defined, unexspected results are now exspected!
#endif
