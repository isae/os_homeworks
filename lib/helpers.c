#include "helpers.h"
#include <stdbool.h>

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

const size_t BUFFER_SIZE = 512;

ssize_t read_until(int fd, void *buf, size_t count, char delimiter)
{
    ssize_t actually_read;
    ssize_t overall = 0;
    while((actually_read = read(fd, buf, count)) > 0)
    {
        bool finded=false;
        for(int i=0;i<actually_read;++i){
            if(*((char*) buf +i)==delimiter){
                finded=true;
            }
        }
        overall+=actually_read;
        count-=actually_read;
        buf+=actually_read;
        if(finded) break;
    }
    if(actually_read==-1)
    {
        return -1;
    } else {
        return overall;
    }
}

int spawn(const char * file, char * const argv [])
{
    pid_t pid = fork();
    
    if(pid){
        if(pid==-1){
            perror("Fork failed");
            exit(EXIT_FAILURE);
            return -1;
        }
        int result;
        if(waitpid(pid,&result,0)==-1){
            perror("Wait failed");
            exit(EXIT_FAILURE);
        }
        return result;        
   } else{
        int fd = open("/dev/null", O_WRONLY);
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);
        execvp(file, argv);
        close(fd);
        exit(EXIT_FAILURE);
        return -1;
   } 
}

