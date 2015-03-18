#include "helpers.h" 
#include <stdio.h> 
#include <string.h>

const int BUFFER_SIZE = 8192; 
const char DELIM = ' ';

void reverse(char* buffer, int n)
{
    for(int i=0;i<n/2;++i){
        char tmp = buffer[i];
        buffer[i] = buffer[n-i-1]; 
        buffer[n-i-1]=tmp;
    }
    
    return;
}

int main()
{
    char buffer[BUFFER_SIZE];
    ssize_t result;
    size_t prev = 0;
    while((result = read_until(STDIN_FILENO,buffer+prev,BUFFER_SIZE-prev, DELIM))>0)
    {
        result+=prev;
        int pos=0;
        int prev_delim = 0;//position after previous delimiter
        while(pos<result)
        {
            if(buffer[pos] == DELIM){
                reverse(buffer, pos-prev_delim);
                if(write_(STDOUT_FILENO,buffer+prev_delim,pos-prev_delim)==-1)
                    perror("Write failed"); 
                prev_delim=pos+1;
            } 
            ++pos;
        }
        memmove(buffer, buffer+prev_delim, result-prev_delim);
        prev = result-prev_delim;
    }
    reverse(buffer, prev);
    if(write_(STDOUT_FILENO,buffer,prev)==-1)
        perror("Write failed"); 
    return 0;
}
