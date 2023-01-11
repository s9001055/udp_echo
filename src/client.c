/*
 * udp echo packet client
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
#include <time.h>
#include <math.h>

int waitFromServer(int sock_fd);
void *recvFuncThread(void *sock_fd);
void sendToServer(int sock_fd);
int myPow(int base, int exp);

#define MULTIPLIER          2
#define TIME_BASE           500  //ms
#define MAX_WAIT_INTERVAL   8000 //ms
#define MSG_LEN             128

char buf[MSG_LEN];
int isRecv = -1;
struct sockaddr_in gServer_addr;

int main(int argc, char **argv) {
    if (argc < 3) {
        printf("pls input Server IP & Server Port \n");
        return -1;
    }
    
    int sock_fd;
    int addr_len = sizeof(struct sockaddr_in);
    pthread_t recvThread;

    gServer_addr.sin_family = AF_INET;
    gServer_addr.sin_addr.s_addr = inet_addr(argv[1]);
    gServer_addr.sin_port = htons(atoi(argv[2]));
    
    sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_fd < 0) {
        printf("socket errer \n");
        return -1;
    }

    printf("input msg: \n");
    fgets(buf, MSG_LEN, stdin);
    sendToServer(sock_fd);
    pthread_create(&recvThread, NULL, recvFuncThread, &sock_fd);
    printf("send success, wait server echo\n");

    if (waitFromServer(sock_fd)) {
        printf("No echo from server \n");
        pthread_cancel(recvThread);
    }

    return 0;
}

void sendToServer(int sock_fd) {
    int addr_len = sizeof(struct sockaddr_in);
    if ( sendto(sock_fd, buf, strlen(buf), 0, (struct sockaddr*)&gServer_addr, addr_len) < 0) {
        printf("send errer \n");
        exit(0);
    }
}

int waitFromServer(int sock_fd) {
    int addr_len = sizeof(struct sockaddr_in);
    clock_t start, end;
    int maxRetry = (log(MAX_WAIT_INTERVAL/TIME_BASE) / log(MULTIPLIER));
    int diffTime;

    int time;
    for (time = 0; time <= maxRetry && isRecv == -1; time++) {
        start = clock();
        printf("no echo from server, after %dms #%d retry \n", TIME_BASE * myPow(MULTIPLIER, time), time+1);
        do {
            end = clock();
            diffTime = (end - start) / (double)(1000);
        }while (isRecv == -1 && diffTime < (TIME_BASE * myPow(MULTIPLIER, time)) );
        sendToServer(sock_fd); // retry send
    }

    return (isRecv == -1) ? 1 : 0;
}

void *recvFuncThread(void *sock_fd) {
    int serv_fd = *(int *) sock_fd;
    int addr_len = sizeof(struct sockaddr_in);
    char recvBuf[MSG_LEN] = {0};

    if (recvfrom(serv_fd, recvBuf, sizeof(recvBuf), 0, (struct sockaddr*)&gServer_addr, &addr_len) < 0) {
        printf("recv thread error \n");
        pthread_exit(0);
    }
    isRecv = 1;
    printf("recv success, server echo: %s \n", recvBuf);
}

int myPow(int base, int exp) {
    if (exp == 0) {
        return 1;
    }

    int ret = base;
    exp -= 1;
    while (exp) {
        ret *= base;
        exp -= 1;
    }

    return ret;
}