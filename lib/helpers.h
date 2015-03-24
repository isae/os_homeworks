#ifndef HELPERS_H
#define HELPERS_H

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

ssize_t read_(int fd, void *buf, size_t count);

ssize_t write_(int fd, const void *buf, size_t count);

ssize_t read_until(int fd, void *buf, size_t count, char delimiter);

int spawn(const char * file, char * const argv []);
#endif
