#ifndef HELPERS_H
#define HELPERS_H

#include <unistd.h>

extern ssize_t read_(int fd, void *buf, size_t count);

extern ssize_t write_(int fd, const void *buf, size_t count);

#endif
