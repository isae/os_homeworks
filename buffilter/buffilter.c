#include "helpers.h" 
#include "bufio.h"
#include <string.h>


const int BUFFER_SIZE = 8192; 
const char DELIM = '\n';
const int ARG_SIZE = 256;
const int LAST_ARG_SIZE = 8192;


int main(int argc, char* argv[])
{
    buf_t * in_buf = buf_new(BUFFER_SIZE);
    buf_t * out_buf = buf_new(BUFFER_SIZE);
    char* args[ARG_SIZE];
    char last_arg[LAST_ARG_SIZE];
    argc-=1;
    memcpy(args, argv+1, argc*sizeof(char*));
    args[argc]=last_arg;
    args[argc+1]=NULL;
    int length = 0;
    while((length = buf_getline(STDIN_FILENO,in_buf,last_arg))>0)
    {
        last_arg[length]=0;
       // printf("Last arg is $%s$\n", last_arg);
       // printf("length is %d\n", length);
        if(spawn(argv[1],args)==0){
            last_arg[length++]='\n';//added back
            int written = buf_write(STDOUT_FILENO,out_buf,last_arg,length);
            if(written==-1){
                perror("Write failed"); 
                exit(EXIT_FAILURE);
            }
        //    printf("Written: %d\n", written);
        }
    }
    buf_flush(STDOUT_FILENO, out_buf, out_buf->size);
    buf_free(in_buf);
    buf_free(out_buf);
}
