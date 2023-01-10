/*
 * udp chat room
 *
 * by Arthur Chang
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>
#include <arpa/inet.h>

struct sockaddr_in gServer_addr;
struct sockaddr_in gClient_addr;

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("pls input Server IP & Server Port \n");
        return 1;
    }

    int sock_fd;
    pthread_t recvThread;

    gServer_addr.sin_family = AF_INET;
    gServer_addr.sin_addr.s_addr = inet_addr(argv[1]);
    gServer_addr.sin_port = htons(atoi(argv[2]));
    
    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0) {
        printf("socket errer \n");
        return 1;
    }
    
    if ( bind(sock_fd, (struct sockaddr*) &gServer_addr, sizeof(gServer_addr)) < 0 ) {
        printf("bind errer \n");
        return 1;
    }

    printf("Server open \n");
    pthread_create(&recvThread, NULL, recvFuncThread, &sock_fd);

    while (1) {
        char inputMsg[32];
        printf("input Server Commend \n");
        printf("1. exit \n");
        fgets(inputMsg, 32, stdin);
        if (strcmp(inputMsg, "exit\n") == 0) {
            break;
        }
    }

    pthread_cancel(recvThread);

    return 0;
}

void *recvFuncThread(void *sock_fd) {
    int serv_fd = *(int *) sock_fd;
    int addr_len = sizeof(struct sockaddr_in);
    char buf[128] = {0};
    while (1) {
        if (recvfrom(serv_fd, &buf, sizeof(buf), 0, (struct sockaddr*)&gClient_addr, &addr_len) < 0) {
            printf("recv thread error \n");
            pthread_exit(0);
        }

        
    }
}

