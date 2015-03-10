#include "helpers.h"
#include <stdio.h>

const int BUFFER_SIZE = 512;

int main(){
    char buffer[BUFFER_SIZE];
    while((ssize_t result = read_(STDIN_FILENO,buffer,BUFFER_SIZE))>0){
        if(result==-1) perror("Read failed"); 
        if(write_(STDOUT_FILENO,buffer,result)==-1)
            perror("Write failed"); 
    }
    return 0;
}
