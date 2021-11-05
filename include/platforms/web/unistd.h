#pragma once

enum {
	STDIN_FILENO,
	STDOUT_FILENO,
	STDERR_FILENO,
};

extern int write(const int fd, const char * const data, const unsigned length);
