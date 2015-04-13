#ifndef BUFIO_H
#define BUFIO_H

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

typedef int fd_t;

typedef struct buf_t
{
    void* mem;
    size_t capacity;
    size_t size;
} buf_t;

buf_t *buf_new(size_t capacity);

void buf_free(buf_t *);

size_t buf_capacity(buf_t *);

size_t buf_size(buf_t *);

ssize_t buf_fill(fd_t fd, buf_t *buf, size_t required);

ssize_t buf_flush(fd_t fd, buf_t *buf, size_t required);

ssize_t buf_getline(fd_t fd, buf_t *buf, char* dest);

ssize_t buf_write(fd_t fd, buf_t *buf, char* src, size_t len);

#endif
