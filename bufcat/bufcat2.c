#include "bufio.h"
const int BUFFER_SIZE = 512;
int main()
{
    buf_t* buffer = buf_new(BUFFER_SIZE);
    ssize_t result;
    while((result = buf_fill(STDIN_FILENO,buffer,buf_size(buffer)))>0)
    {
        if(result==-1) perror("Read failed"); 
        if(buf_flush(STDOUT_FILENO,buffer,buf_size(buffer))==-1)
            perror("Write failed"); 
    }
    return 0;
}
