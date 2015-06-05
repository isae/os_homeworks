#define _POSIX_SOURCE
#include "helpers.h" 
#include "bufio.h"
#include <string.h>
#include <sys/types.h>
#include <signal.h>


const int BUFFER_SIZE = 8192; 
const char DELIM = '|';
const char SPACE = ' ';
const int COMMANDS_SIZE = 256;

void die(int signum){
    printf("closing\n");
    //kill(getpid(), SIGTERM); 
}

void setActions(){
    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = die;
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGQUIT);
    act.sa_mask = set;
    if(sigaction(SIGQUIT, &act, 0)==-1){
        perror("sigaction failed");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char* argv[])
{
    const char* greetings = "$";
    buf_t * in_buf = buf_new(BUFFER_SIZE);//input buffer
    if(in_buf==NULL)
    {
        perror("Malloc failed");
        exit(EXIT_FAILURE);
    }
    while(1){
        write_(STDOUT_FILENO, greetings, 1);
        char* cur_line = malloc(BUFFER_SIZE);
        char* commands[COMMANDS_SIZE][COMMANDS_SIZE];//currently line, parsed 
        memset(commands, 0, sizeof(commands));
        int comm_size = 0;//how many commands
        int comm_arg_size = 0;//how many args in current command
        ssize_t read = _buf_getline(STDIN_FILENO, in_buf, cur_line, 1);
        if(read<0){
            perror("Read failed");
            exit(EXIT_FAILURE);
        }
        int prev = 0;//position of beginning of current command
        int i=0;
        while(i<read){
            if(cur_line[i]==DELIM){
                if(i>prev){
                    cur_line[i]=0;//lets put end of line instead of delim
                    commands[comm_size][comm_arg_size] = cur_line+prev;
                } 
                commands[comm_size][comm_arg_size+1] = 0;//null-terminate
                ++comm_size;
                comm_arg_size = 0;//new command
                ++i;
                prev = i;
            } else 
            if(cur_line[i]==SPACE){
                if(i>prev){
                    cur_line[i]=0;//eol
                    commands[comm_size][comm_arg_size] = cur_line+prev;
                    ++comm_arg_size;
                } 
                ++i;
                prev = i;
            } else {
                ++i;
            }
        }
        if(i>prev){//row doesnt have trailing spaces
            cur_line[i]=0;
            commands[comm_size][comm_arg_size] = cur_line+prev;
        }
        commands[comm_size][comm_arg_size+1] = 0;
        execargs_t* to_run[comm_size+2];
        to_run[comm_size+1] = 0;
                
        for(int j=0;j<=comm_size;++j){
            char** arr = commands[j];
            execargs_t* tmp = execargs_new(arr[0], arr);
            to_run[j] = tmp;
        }
        _runpiped(to_run);

        

    }
    
    buf_free(in_buf);
}
