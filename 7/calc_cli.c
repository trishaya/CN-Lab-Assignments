#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    struct sockaddr_in serv;
    char expr[100], result[100];
    socklen_t len;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    serv.sin_family = AF_INET;
    serv.sin_port = htons(9090);
    serv.sin_addr.s_addr = inet_addr("10.0.0.1");  // server IP

    len = sizeof(serv);

    while (1) {
        printf("Enter expression (ex: 10+5, sin(30), 8/2, inv(5)): ");
        scanf("%s", expr);

        sendto(sockfd, expr, strlen(expr) + 1, 0, (struct sockaddr *)&serv, len);

        recvfrom(sockfd, result, sizeof(result), 0, (struct sockaddr *)&serv, &len);
        printf("Result from server = %s\n\n", result);
    }

    close(sockfd);
    return 0;
}
