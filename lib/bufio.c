#include "bufio.h"

buf_t *buf_new(size_t capacity)
{
    buf_t* result = (buf_t*) malloc(sizeof(buf_t));
    result->capacity = capacity;
    result->size = 0;
    result->mem = malloc(capacity);
    return result;
}

void abort_if_debug()
{
    #ifdef DEBUG
            abort();
    #endif
}

void buf_free(buf_t * buf)
{

    if(buf==NULL){
        abort_if_debug();
    }
    free(buf->mem);
    free(buf);
    return;
}

size_t buf_capacity(buf_t * buf)
{
    return buf->capacity;
}

size_t buf_size(buf_t * buf)
{
    return buf->size;
}

ssize_t buf_fill(fd_t fd, buf_t *buf, size_t required)
{
    ssize_t actually_read;
    ssize_t size = 0;
    char* buffer = (char*) buf->mem + buf->size;
    size_t free_size = buf->capacity - buf->size;

    if(free_size<required) abort_if_debug();

    while((actually_read = read(fd, buffer, free_size)) > 0)
    {
        size+=actually_read;
        free_size-=actually_read;
        buffer+=actually_read;
        if(size>=required) {
            break;
        }
    }
    if(actually_read>=0)
    {
        buf->size += size;
        return buf->size;
    } else {
        return -1;
    }
}

ssize_t buf_flush(fd_t fd, buf_t *buf, size_t required)
{
    size_t total_written = 0;
    size_t actually_written = 0;
    char* buffer = (char*) buf->mem;

    while(buf->size!=0)
    {
        actually_written = write(fd, buffer, buf->size);
        if(actually_written==-1) return -1;
        total_written+=actually_written;
        buf->size-=actually_written;
        buffer+=actually_written;

        if(total_written>=required){
           memmove(buf->mem, buffer, buf->size);     
           return total_written; 
        }
    }
    return total_written;
}

