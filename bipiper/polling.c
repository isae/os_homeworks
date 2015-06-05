#include <unistd.h>
#include <sys/types.h>
#include "helpers.h" 
#include "bufio.h"
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <poll.h>

#include <errno.h>
#define BUFFER_SIZE 8192

typedef struct{
    buf_t* buf_from;
    buf_t* buf_to;
} buf_pair;

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
void do_some_job(struct pollfd* pollfds, buf_pair* buf_pairs, int i)
{
    struct pollfd* fd1 = &pollfds[2+2*i];
    struct pollfd* fd2 = &pollfds[2+2*i+1];
    buf_pair* pair = &buf_pairs[i];

    printf("some_job %d %d\n", fd1->fd, fd2->fd);
    buf_t* buffer1 = pair->buf_from;
    buf_t* buffer2 = pair->buf_to;
    int readed;
    readed = buf_fill(fd1->fd, buffer1, 1);
    if(readed == -1){
        if(errno == EAGAIN || errno == EWOULDBLOCK){
            
        } else {
            perror("read1");
            exit(EXIT_FAILURE);
        }
    }
    if(readed==0){
        printf("readed zero bytes!!!!1\n");
        exit(EXIT_FAILURE);
    } else {
    }
    buf_flush(fd2->fd, buffer1, 1);
    //buf_flush(fd2->fd, buffer1, buffer1->size);

    readed = buf_fill(fd2->fd, buffer2, 1);
    if(readed == -1){
        if(errno == EAGAIN || errno == EWOULDBLOCK){
        } else {
            perror("read2");
            exit(EXIT_FAILURE);
        }
    }
    if(readed==0){
        printf("readed zero bytes!!!!1\n");
        exit(EXIT_FAILURE);
    }
    buf_flush(fd1->fd, buffer2, 1);
    //buf_flush(fd1->fd, buffer2, buffer2->size);

    //if(shutdown(fd_to, SHUT_WR)==-1) perror("Shutdown");
    //if(close(fd_to)==-1) perror("close");
    //printf("close %d %d", fd_from, fd_to);
    //exit(EXIT_SUCCESS);
    printf("some_job ended \n");
}

void add_accept_socket(struct pollfd* pollfds, int* poll_size, int sock_fd)
{
    struct pollfd fd = {
        .fd = sock_fd,
        .events = POLLIN,
        .revents = 0
    };
    pollfds[*poll_size] = fd;
    ++*poll_size;
}

void add_fd(struct pollfd* pollfds, int* poll_size, int read_fd)
{
    struct pollfd fd = {
        .fd = read_fd,
        .events = POLLIN,
        .revents = 0
    };
    pollfds[*poll_size] = fd;
    ++*poll_size;
}


int rb_push(int fd, int* rb, int* h, int* t, int* size)
{
    printf("fd is %d\n", fd);
    if(*t == (( *h - 1 + *size) % *size)){
        return -1; /* Queue Full*/
    }
    rb[*t] = fd;
    *t = (*t + 1) % *size;
    return 0; // No errors
}

int rb_pop(int* rb, int* h, int* t, int* size)
{
    if(*t == *h){
        return -1; /* Queue Empty - nothing to get*/
    }
    int result = rb[*h];
    *h = (*h + 1) % *size;
    return result; // No errors
}

int rb_peek(int* rb, int* h, int* t, int* size)
{
    if(*t == *h){
        return -1; /* Queue Empty - nothing to get*/
    }
    int result = rb[*h];
    return result; // No errors
}

#define rb1_peek() rb_peek(connections, &h, &t, &qs)
#define rb1_pop() rb_pop(connections, &h, &t, &qs)
#define rb2_peek() rb_peek(connections2, &h2, &t2, &qs2)
#define rb2_pop() rb_pop(connections2, &h2, &t2, &qs2)

int main(int argc, char* argv[])
{
    struct sockaddr client;
    struct sockaddr client2;
    socklen_t sz;
    socklen_t sz2;
    int sock = get_socket(argv[1], &client, &sz);
    int sock2 = get_socket(argv[2], &client2, &sz2);
    struct pollfd pollfds[256];
    int poll_size = 0;
    int pairs_size = 0;
    buf_pair buf_pairs[127];
    add_accept_socket(pollfds, &poll_size, sock);
    add_accept_socket(pollfds, &poll_size, sock2);
    int qs0 = BUFFER_SIZE +1; 
    int connections[qs0];
    int connections2[qs0];
    int h=0, t=0, h2=0, t2=0, qs=qs0, qs2 = qs0;
    while(1)
    {
        int poll_res = poll(pollfds, poll_size, -1);
        printf("poll fired! %d\n", poll_res);
        if((pollfds[0].revents&POLLIN)!=0){
            int fd1 = accept4(pollfds[0].fd, &client, &sz, SOCK_NONBLOCK);
            if (fd1 == -1)
                perror("accept");
            rb_push(fd1, connections, &h, &t, &qs);
        }
        if((pollfds[1].revents&POLLIN)!=0){
            int fd2 = accept4(pollfds[1].fd, &client2, &sz2, SOCK_NONBLOCK);
            if (fd2 == -1)
                perror("accept");
            rb_push(fd2, connections2, &h2, &t2, &qs2);
        }
        int fst, snd;
        if((fst = rb1_peek()) != -1 && (snd = rb2_peek())!=-1){
            printf("ok, time to create pipe! %d %d \n", fst, snd);
            int num = poll_size;
            add_fd(pollfds, &poll_size, rb1_pop());
            add_fd(pollfds, &poll_size, rb2_pop());
            buf_pairs[pairs_size].buf_from = buf_new(BUFFER_SIZE);  
            buf_pairs[pairs_size].buf_to = buf_new(BUFFER_SIZE);  
            ++pairs_size;
        }
        printf("poll_size is %d\n", poll_size);
        for(int i = 0; i<pairs_size;++i){
            do_some_job(pollfds,buf_pairs,i);
        }
    }
    return 0;
}
