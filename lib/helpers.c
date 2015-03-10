#include "helpers.h"

ssize_t read_(int fd, void *buf, size_t count)
{
    ssize_t actually_read;
    ssize_t overall = 0;
    while((actually_read = read(fd, buf, count)) > 0)
    {
        overall+=actually_read;
        count-=actually_read;
        buf+=actually_read;
    }
    if(actually_read==0)
    {
        return overall;
    } else {
        return -1;
    }
}


ssize_t write_(int fd, const void *buf, size_t count)
{
    ssize_t cnt = count;
    ssize_t actually_written;
    while(count!=0)
    {
        actually_written = write(fd, buf, count);
        if(actually_written==-1) return -1;
        count-=actually_written;
        buf+=actually_written;
    }
    return cnt;
}

