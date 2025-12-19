#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int sockfd, newsock;
    struct sockaddr_in serv, cli;
    char buff[100];
    socklen_t len;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    serv.sin_family = AF_INET;
    serv.sin_port = htons(8080);
    serv.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr*)&serv, sizeof(serv));
    listen(sockfd, 5);

    len = sizeof(cli);
    newsock = accept(sockfd, (struct sockaddr*)&cli, &len);

    recv(newsock, buff, sizeof(buff), 0);   
    printf("Client: %s\n", buff);

    send(newsock, "Hello", strlen("Hello"), 0); 

    close(newsock);
    close(sockfd);
    return 0;
}
