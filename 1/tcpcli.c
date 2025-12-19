#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int sockfd;
    struct sockaddr_in serv;
    char buff[100];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    serv.sin_family = AF_INET;
    serv.sin_port = htons(8080);
    serv.sin_addr.s_addr = inet_addr("10.0.0.1"); 

    connect(sockfd, (struct sockaddr*)&serv, sizeof(serv));

    send(sockfd, "Hi", strlen("Hi"), 0);          
    recv(sockfd, buff, sizeof(buff), 0);          
    printf("Server: %s\n", buff);

    close(sockfd);
    return 0;
}
