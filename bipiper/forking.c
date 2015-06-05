#include <unistd.h>
#include <sys/types.h>
#include "helpers.h" 
#include "bufio.h"
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 8192

int get_socket(char* port, struct sockaddr* cli, socklen_t* sz)
{
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == -1)
        perror("socket");
    int one = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int)) == -1)
        perror("setsockopt");
    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(atoi(port)),
        .sin_addr = {.s_addr = INADDR_ANY}};
    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) == -1)
        perror("bind");
    if (listen(sock, 1) == -1)
        perror("listen");
    struct sockaddr_in client;
    memcpy(cli, &client, sizeof(client));
    *sz = sizeof(client);
    return sock;
}
void do_child_job(int fd_from, int fd_to)
{
    buf_t* buffer = buf_new(BUFFER_SIZE); 
    int readed;
    while((readed = buf_fill(fd_from, buffer, 1))>0)
    {
        buf_flush(fd_to, buffer, 1);
    }
    if(readed == -1) perror("read");
    if(shutdown(fd_from, SHUT_RD)==-1) perror("Shutdown");
    buf_flush(fd_to, buffer, buffer->size);
    if(shutdown(fd_to, SHUT_WR)==-1) perror("Shutdown");
    if(close(fd_to)==-1) perror("close");
    if(close(fd_from)==-1) perror("close");
    //printf("close %d %d", fd_from, fd_to);
    exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[])
{
    struct sockaddr client;
    struct sockaddr client2;
    socklen_t sz;
    socklen_t sz2;
    int sock = get_socket(argv[1], &client, &sz);
    int sock2 = get_socket(argv[2], &client2, &sz2);
    while(1)
    {
        int fd1 = accept(sock, &client, &sz);
        if (fd1 == -1)
            perror("accept");
        int fd2 = accept(sock2, &client2, &sz2);
        if (fd2 == -1)
            perror("accept");
        int pid = fork();
        if(pid == -1)
            perror("fork");
        if(pid){//in parent
            int pid2 = fork();
            if(pid2 == -1)
                perror("fork");
            if(!pid2){//another child
                do_child_job(fd1, fd2);
            }
        } else {//child
            do_child_job(fd2, fd1);
        }
    }
    return 0;
}
