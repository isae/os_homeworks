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

execargs_t* execargs_new(char* command, char** argv)
{
    execargs_t* result = (execargs_t*) malloc(sizeof(execargs_t));
    result->command = command;
    result->argv = argv;
    return result;
}

int exec(execargs_t* args)
{   
    execvp(args->command, args->argv);
    return -1;
}

int running_process = 0;
void die(int signum){
    if(running_process!=0) kill(running_process, SIGKILL);
    running_process=0;
}

int runpiped(execargs_t** programs, size_t n)
{
    int parent_pid = fork();
    if(parent_pid==-1){
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    if(parent_pid){
        struct sigaction act;
        memset(&act, 0, sizeof(act));
        act.sa_handler = die;
        sigset_t set;
        sigemptyset(&set);
        sigaddset(&set, SIGINT);
        sigaddset(&set, SIGPIPE);
        act.sa_mask = set;
        if(sigaction(SIGINT, &act, 0)==-1){
            perror("sigaction failed");
            exit(EXIT_FAILURE);
        }
        if(sigaction(SIGPIPE, &act, 0)==-1){
            perror("sigaction failed");
            exit(EXIT_FAILURE);
        }
        running_process = parent_pid;
        int st;
        if(waitpid(parent_pid, &st, 0)==-1){
            //interrupted by a signal
            return 0;
        }
        if(WIFEXITED(st) && WEXITSTATUS(st) == EXIT_SUCCESS) 
        {
            return 0;
        } else { 
            return -1;
        }
    } else {
        int pipes[n-1][2];
        for(int i=0;i<n-1;++i)
        {
            pipe(pipes[i]);
        }
        for(int i=0;i<n;++i)
        {
            pid_t pid = fork();
            if(pid==-1){
                perror("Fork failed");
                exit(EXIT_FAILURE);
            }
            if(pid){
            } else {//exec
                if(i<n-1){
                    dup2(pipes[i][1], STDOUT_FILENO);
                    close(pipes[i][0]);
                }
                if(i>0){
                    dup2(pipes[i-1][0], STDIN_FILENO);  
                    close(pipes[i-1][1]);
                }
                exec(programs[i]);
                exit(EXIT_FAILURE);
                return -1;
            }
        }
        int st;
        while(1){
            if(wait(&st)==-1){
                exit(EXIT_FAILURE);
            }
            if(WIFEXITED(st) && WEXITSTATUS(st) == EXIT_SUCCESS)
            {
                exit(EXIT_SUCCESS);
            } else { 
                exit(EXIT_FAILURE);
            }
        }
        exit(EXIT_SUCCESS);
        return 0;
    }
}

int _runpiped(execargs_t** programs)//array must be null terminated
{
    int n = 0;
    while(programs[n]!=0) ++n;   
    return runpiped(programs, n); 
}

