#include "helpers.h" 
#include <string.h>

const int BUFFER_SIZE = 8192; 
const char DELIM = '\n';
const int ARG_SIZE = 256;
const int LAST_ARG_SIZE = 8192;


int main(int argc, char* argv[])
{
    char buffer[BUFFER_SIZE];
    char* args[ARG_SIZE];
    char last_arg[LAST_ARG_SIZE];
    ssize_t result;
    size_t prev = 0;
    argc-=1;
    memcpy(args, argv+1, argc*sizeof(char*));
    args[argc]=last_arg;
    args[argc+1]=NULL;
    while((result = read_(
                    STDIN_FILENO,
                    buffer+prev,
                    BUFFER_SIZE-prev))>0)
    {
        //printf("Got %d symbols\n", (int)result);
        result+=prev;
        int pos=0;
        int prev_delim = 0;//position after previous delimiter
        while(pos<result)
        {
            if(buffer[pos] == DELIM){
                int length = pos-prev_delim+1;
                memcpy(last_arg, buffer+prev_delim, length*sizeof(char));
                last_arg[length]=0;
                last_arg[length-1]=0;//removed \n
                if(spawn(argv[1],args)==0){
                    last_arg[length-1]='\n';//added back
                    if(write_(STDOUT_FILENO,last_arg,length)==-1){
                        perror("Write failed"); 
                        exit(EXIT_FAILURE);
                    }
                }
                prev_delim=pos+1;
            } 
            ++pos;
        }
        memmove(buffer, buffer+prev_delim, (result-prev_delim)*sizeof(char));
        prev = result-prev_delim;
    }
    
    ++prev;
    memcpy(last_arg, buffer, prev*sizeof(char));
    last_arg[prev]=0;
    last_arg[prev-1]=0;//removed \n
    if(spawn(argv[1],args)==0){
        last_arg[prev-1]='\n';//added back
        if(write_(STDOUT_FILENO,last_arg,prev)==-1){
            perror("Write failed"); 
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
