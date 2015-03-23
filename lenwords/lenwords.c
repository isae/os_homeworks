#include "helpers.h"
#include <stdio.h>
#include <string.h>

#define RESULT_BUFFER_SIZE 8

const int BUFFER_SIZE = 8192; 
const char DELIM = ' ';

char output_buffer[RESULT_BUFFER_SIZE];

void clean_output_buffer(){
    for(int i=0;i<RESULT_BUFFER_SIZE;++i)
    {
        output_buffer[i]=0;
    }
    return;
}

int main()
{
    char buffer[BUFFER_SIZE];
    ssize_t result;
    size_t prev = 0;
    while((result = read_until(
                    STDIN_FILENO,
                    buffer+prev,
                    BUFFER_SIZE-prev,
                    DELIM))>0)
    {
        result+=prev;
        int pos=0;
        int prev_delim = 0;//position after previous delimiter
        while(pos<result)
        {
            if(buffer[pos] == DELIM){
                clean_output_buffer();
                int length = sprintf(output_buffer,"%d\n", pos-prev_delim);
                if(write_(STDOUT_FILENO,output_buffer,length+1)==-1)
                    perror("Write failed"); 
                prev_delim=pos+1;
            } 
            ++pos;
        }
        memmove(buffer, buffer+prev_delim, result-prev_delim);
        prev = result-prev_delim;
    }
    clean_output_buffer();
    int length = sprintf(output_buffer,"%d\n", (int)prev);
    if(write_(STDOUT_FILENO,output_buffer,length)==-1)
        perror("Write failed");
    return 0;
}
