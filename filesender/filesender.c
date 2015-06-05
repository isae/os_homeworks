#include <unistd.h>
#include <sys/types.h>
#include "helpers.h" 
#include "bufio.h"
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>

#define BUFFER_SIZE 8192
int main(int argc, char* argv[])
{
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == -1)
        perror("socket");
    int one = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int)) == -1)
        perror("setsockopt");
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(atoi(argv[1])),
        .sin_addr = {.s_addr = INADDR_ANY}};
    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1)
        perror("bind");
    if (listen(sock, 1) == -1)
        perror("listen");
    struct sockaddr_in client;
    socklen_t sz = sizeof(client);
    while(1)
    {
        int fd = accept(sock, (struct sockaddr*)&client, &sz);
        if (fd == -1)
            perror("accept");
        int pid = fork();
        if(pid == -1)
            perror("fork");
        if(!pid){//in child
            buf_t* buffer = buf_new(BUFFER_SIZE); 
            int file = open(argv[2], O_RDONLY);
            if(file==-1) perror("open");
            int readed;
            while((readed = buf_fill(file, buffer, 1))>0)
            {
                buf_flush(fd, buffer, 1);
            }
            if(readed == -1) perror("read");
            buf_flush(fd, buffer, buffer->size);
            if(shutdown(fd, SHUT_WR)==-1) perror("Shutdown");
            if(close(fd)==-1) perror("close");
            if(close(file)==-1) perror("close");
            exit(EXIT_SUCCESS);
        }
    }
    return 0;
}
