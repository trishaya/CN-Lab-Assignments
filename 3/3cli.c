#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define S_IP "10.0.0.1"
#define P 55550
#define B 1024

int main() {
    int c_fd;
    struct sockaddr_in s_a;
    char buf[B];
    char in[50];
    
    c_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(c_fd < 0) {
        perror("socket fail");
        exit(1);
    }
    
    s_a.sin_family = AF_INET;
    s_a.sin_port = htons(P);
    s_a.sin_addr.s_addr = inet_addr(S_IP);
    
    printf("UDP Client\n");
    printf("Server: %s:%d\n", S_IP, P);
    printf("LIST - see fruits\nfruit - buy\nquit - exit\n\n");
    
    while(1) {
        printf("> ");
        fgets(in, sizeof(in), stdin);
        
        if(in[strlen(in)-1] == '\n') in[strlen(in)-1] = '\0';
        
        if(strcmp(in, "quit") == 0) break;
        
        sendto(c_fd, in, strlen(in), 0, (struct sockaddr*)&s_a, sizeof(s_a));
        
        memset(buf, 0, B);
        socklen_t a_l = sizeof(s_a);
        int b_r = recvfrom(c_fd, buf, B-1, 0, (struct sockaddr*)&s_a, &a_l);
        
        if(b_r > 0) {
            buf[b_r] = '\0';
            printf("Server:\n%s\n\n", buf);
        }
    }
    
    close(c_fd);
    printf("Done\n");
    
    return 0;
}
