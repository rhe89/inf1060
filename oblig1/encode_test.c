#ifndef XXCODE_TEST_INF1060
#define XXCODE_TEST_INF1060
#include "compress_test.h"

void* writer(void *arg) {
	struct thread_arg *wt = arg;
	char wrt[] = {' ', ':', '@', '\n', '\n'};
	ssize_t now = 0, total = 0;
	while ((ssize_t)sizeof(wrt) > total + (now =
		write(wt->fd, wrt + total, sizeof(wrt) - total))) {
		if (now < 0) {
			close(wt->fd);
			wt->result = -1;
			return NULL;
		}
		total += now;
		usleep(200);
	}
	wt->result = 0;
	close(wt->fd);
	return NULL;
}

void* reader(void *arg) {
	struct thread_arg *rd = arg;
	unsigned char res[2];
	memset(res, 0, sizeof(res));
	ssize_t now = 0, total = 0;
	while ((ssize_t)sizeof(res) > total + (now =
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
	if (res[0] != 0x1b) {
		if (res[0] & 0xc0) {
			fprintf(stderr, "Missmatch for  ' ' (0x%x) got 0x%x\n", 0x0, res[0] >> 6);
			rd->result |= 0x1;
		}
		if ((res[0] & 0x30) != 0x10) {
			fprintf(stderr, "Missmatch for  ':' (0x%x) got 0x%x\n", 0x1, (0x3f & res[0]) >> 4);
			rd->result |= 0x2;
		}
		if ((res[0] & 0x0c) != 0x08) {
			fprintf(stderr, "Missmatch for  '@' (0x%x) got 0x%x\n", 0x2, (0xc & res[0]) >> 2);
			rd->result |= 0x4;
		}
		if ((res[0] & 0x03) != 0x03) {
			fprintf(stderr, "Missmatch for '\\n' (0x%x) got 0x%x\n", 0x3, 0x3 & res[0]);
			rd->result |= 0x8;
		}
	}
	if (res[1] != 0xc0) {
		fprintf(stderr, "Incorrect packing when length is not a "
			"multiplum of 4.\n");
		rd->result |= 0x10;
	}
	close(rd->fd);
	return NULL;
}
#else
#warning Test functions allready defined, unexspected results are now exspected!
#endif
