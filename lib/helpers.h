#define _POSIX_SOURCE
#ifndef HELPERS_H
#define HELPERS_H

#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

ssize_t read_(int fd, void *buf, size_t count);

ssize_t write_(int fd, const void *buf, size_t count);

ssize_t read_until(int fd, void *buf, size_t count, char delimiter);

int spawn(const char * file, char * const argv []);

typedef struct execargs_t
{
    char* command;
    char** argv;
} execargs_t;

execargs_t* execargs_new(char* command, char** argv);

int exec(execargs_t* args);

int runpiped(execargs_t** programs, size_t n);

int _runpiped(execargs_t** programs);
#endif
