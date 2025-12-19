#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT 8888
#define BUFFER 2048

int sock;

void *recv_msg(void *arg) {
    char msg[BUFFER];
    while(1) {
        memset(msg, 0, sizeof(msg));
        int r = recv(sock, msg, sizeof(msg), 0);
        if(r <= 0) exit(0);
        printf("%s", msg);
    }
}

int main() {
    struct sockaddr_in server;
    char msg[BUFFER];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("10.0.0.1"); // server IP

    connect(sock, (struct sockaddr*)&server, sizeof(server));

    printf("Connected to chat server...\n");

    pthread_t t;
    pthread_create(&t, NULL, recv_msg, NULL);

    while(1) {
        fgets(msg, sizeof(msg), stdin);
        send(sock, msg, strlen(msg), 0);
    }
}
