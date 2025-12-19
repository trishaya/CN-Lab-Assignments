#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    struct sockaddr_in serv, cli;
    char buffer[100];
    socklen_t len;
    
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        exit(1);
    }

    serv.sin_family = AF_INET;
    serv.sin_port = htons(9090);
    serv.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&serv, sizeof(serv)) < 0) {
        perror("bind failed");
        exit(1);
    }

    printf("UDP Scientific Calculator Server Running...\n");
    len = sizeof(cli);

    while (1) {
        recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&cli, &len);
        printf("\nClient expression: %s\n", buffer);

        double a, b, result = 0;
        char op[10];

        if (sscanf(buffer, "%lf%[^0-9]%lf", &a, op, &b) == 3) {
            if (strcmp(op, "+") == 0) result = a + b;
            else if (strcmp(op, "-") == 0) result = a - b;
            else if (strcmp(op, "*") == 0) result = a * b;
            else if (strcmp(op, "/") == 0) result = a / b;
        }
        else if (sscanf(buffer, "%[^ (](%lf)", op, &a) == 2) {
            if (strcmp(op, "sin") == 0) result = sin(a * M_PI / 180);
            else if (strcmp(op, "cos") == 0) result = cos(a * M_PI / 180);
            else if (strcmp(op, "tan") == 0) result = tan(a * M_PI / 180);
            else if (strcmp(op, "inv") == 0) result = 1 / a;
        } else {
            sprintf(buffer, "Invalid Expression");
            sendto(sockfd, buffer, strlen(buffer) + 1, 0, (struct sockaddr *)&cli, len);
            continue;
        }

        sprintf(buffer, "%.4lf", result);
        sendto(sockfd, buffer, strlen(buffer) + 1, 0, (struct sockaddr *)&cli, len);
    }

    close(sockfd);
    return 0;
}
