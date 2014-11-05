#ifndef COMPRESS_TEST_INF1060
#define COMPRESS_TEST_INF1060

// stdin, stdout, stderr, fprintf, etc.
#include <stdio.h>

// memset, etc.
#include <string.h>

// kill
#include <sys/types.h>
// + lstst, etc.
#include <sys/stat.h>
// + dup2, execvp
#include <unistd.h>

// pthread_create, pthread_join, etc.
#include <pthread.h>

// +kill
#include <signal.h>

// errno
#include <errno.h>

struct thread_arg {
	int fd;
	int result;
};

void* writer(void *arg);
void* reader(void *arg);

#endif
