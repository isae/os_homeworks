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

ssize_t buf_getline(fd_t fd, buf_t *buf, char* dest)
{
    char* mem = buf->mem;
    ssize_t global_pos = 0;
    while(1)
    {
        //printf("buf_size is %d\n", buf->size);
        int pos = -1;
        for(int i=0;i<buf->size;++i)
        {
            if(mem[i]=='\n'){
                pos = i;
                global_pos+=pos;
                break;
            }
        }
        if(pos!=-1){
            memcpy(dest, mem, pos);
            memmove(mem, mem+pos+1, buf->size-pos-1);
            buf->size-= (pos+1);
            //printf("Now buf_size is %d\n", buf->size);
            return global_pos;
        }
        memcpy(dest, mem, buf->size);
        dest+=buf->size;
        global_pos+=buf->size;
        buf->size = 0; 
        if(buf_fill(fd, buf, 1)<=0) return global_pos;
    }
    return -1;
}

ssize_t buf_write(fd_t fd, buf_t *buf, char* src, size_t len)
{
    char* mem = (char*) buf->mem;
    int result = 0;
    while(len>0)
    {
        if(buf->size==buf->capacity)
        {
            int written = buf_flush(fd, buf, 1);
            if(written<=0)
            {
                return result;
            }
        }
        int remaining = buf->capacity - buf->size;
        int shallMove = len>=remaining?remaining:len;
        result+=shallMove;
        memcpy(mem+buf->size, src, shallMove);
        buf->size+=shallMove;
        src+=shallMove;
        len-=shallMove;
    }
    return result;
}
