#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

#define PORT 8888
#define MAX_CLIENTS 50
#define BUFFER 2048

int clients[MAX_CLIENTS];
pthread_mutex_t lock;

FILE *logfile;

void timestamp(char *buf) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buf, 40, "[%d-%m-%Y %H:%M:%S]", t);
}

void broadcast(char *msg, int sender) {
    pthread_mutex_lock(&lock);
    for(int i=0; i<MAX_CLIENTS; i++) {
        if(clients[i] != 0 && clients[i] != sender) {
            send(clients[i], msg, strlen(msg), 0);
        }
    }
    pthread_mutex_unlock(&lock);
}

void *client_handler(void *socket_desc) {
    int sock = *(int*)socket_desc;
    char msg[BUFFER], final_msg[BUFFER + 60], timebuf[60];

    while(1) {
        memset(msg, 0, sizeof(msg));
        int r = recv(sock, msg, sizeof(msg), 0);
        if(r <= 0) {
            printf("Client disconnected\n");
            close(sock);
            for(int i=0; i<MAX_CLIENTS; i++)
                if(clients[i] == sock) clients[i] = 0;
            pthread_exit(NULL);
        }

        timestamp(timebuf);
        sprintf(final_msg, "%s %s", timebuf, msg);
        printf("%s", final_msg);

        fprintf(logfile, "%s", final_msg);
        fflush(logfile);

        broadcast(final_msg, sock);
    }

    return 0;
}

int main() {
    int server_fd, newfd;
    struct sockaddr_in server, client;
    socklen_t c = sizeof(client);

    logfile = fopen("log.txt", "a");
    pthread_mutex_init(&lock, NULL);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr*)&server, sizeof(server));
    listen(server_fd, 5);

    printf("Chat Server started... waiting for clients\n");

    while((newfd = accept(server_fd, (struct sockaddr*)&client, &c))) {
        printf("Client connected\n");

        pthread_mutex_lock(&lock);
        for(int i=0; i<MAX_CLIENTS; i++)
            if(clients[i] == 0) { clients[i] = newfd; break; }
        pthread_mutex_unlock(&lock);

        pthread_t thread_id;
        pthread_create(&thread_id, NULL, client_handler, (void*)&newfd);
        pthread_detach(thread_id);
    }
    return 0;
}
