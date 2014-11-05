#include "compress_test.h"

int main(int argc, char *argv[], char *env[]) {
	if (argc < 2) {
		fprintf(stderr, "Usage:\n\t%s <cmd [arguments]>\n", argv[0]);
		return -1;
	}
	{
		struct stat info;
		if (lstat(argv[1], &info)) {
			perror("lstat failed");
			return -1;
		}
		if (!(info.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH))) {
			fprintf(stderr, "Not executable: %s\n", argv[1]);
			return -1;
		}
	}
	int fds[4];
	if (pipe(fds + 0) || pipe(fds + 2)) {
		perror("Readying pipes for testing failed");
	}
	pid_t has_child = fork();
	if (has_child == -1) {
		fprintf(stderr, "Failed spawing a process to test with.\n");
		return -1;
	} else if (has_child) {
		close(fds[0]);
		close(fds[3]);
		pthread_t t_writer, t_reader;
		struct thread_arg a_writer, a_reader;
		a_writer.fd = fds[1];
		a_writer.result = -10;
		a_reader.fd = fds[2];
		a_reader.result = -10;
		// Note: writer/reader will be included on compiletime, based
		//         on the test we would like to run.
		if (errno = pthread_create(&t_writer, NULL, writer, &a_writer)) {
			perror("Could not start writer thread");
		}
		if (errno = pthread_create(&t_reader, NULL, reader, &a_reader)) {
			perror("Cound not start reader thread");
		}
		pthread_join(t_writer, NULL);
		pthread_join(t_reader, NULL);
		usleep(5000);
		if (kill(has_child, 9)) {
			perror("Could not signal termination");
		}
		int status;
		if (0 > waitpid(has_child, &status, 0)) {
			perror("Could not wait for child");
		}
		if (!a_writer.result && !a_reader.result) {
			fprintf(stdout, "Test passed!\n");
		} else {
			fprintf(stdout, "Test failed!\n");
		}
		return 0;
	} else {
		close(fds[1]);
		close(fds[2]);
		if (dup2(fds[0], fileno(stdin)) < 0 || dup2(fds[3], fileno(stdout)) < 0) {
			perror("Failed replacing input/output for process.");
			close(fds[0]);
			close(fds[3]);
			return -1;
		}
		fprintf(stderr, "Now starting program to test: ");
		int argl;
		for (argl = 1; argl < argc; argl++)
			fprintf(stderr, " %s", argv[argl]);
		fprintf(stderr, "\n");
		execvp(argv[1], argv + 1);
		perror("Starting program to test failed");
		return -1;
	}
}
