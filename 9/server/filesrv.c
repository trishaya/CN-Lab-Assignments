#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <pthread.h>

#define S_IP "10.0.0.1"
#define P 55550
#define B 1024

int s_fd;

void* receive_files(void* arg) {
    char buf[B];
    
    while(1) {
        memset(buf, 0, B);
        int bytes = recv(s_fd, buf, B, 0);
        if(bytes <= 0) break;
        
        // If we receive "SEND" command, get filename and receive file
        if(strncmp(buf, "SEND", 4) == 0) {
            // Get filename
            memset(buf, 0, B);
            recv(s_fd, buf, B, 0);
            char f_name[50];
            strcpy(f_name, buf);
            
            printf("Receiving %s from server...\n", f_name);
            clock_t start = clock();
            
            FILE* f = fopen(f_name, "wb");
            if(f) {
                int file_bytes;
                while((file_bytes = recv(s_fd, buf, B, 0)) > 0) {
                    fwrite(buf, 1, file_bytes, f);
                }
                fclose(f);
                
                clock_t end = clock();
                double t = ((double)(end - start)) / CLOCKS_PER_SEC;
                printf("File received in %.3f seconds\n", t);
            }
        }
    }
    return NULL;
}

int main() {
    struct sockaddr_in s_a;
    char buf[B];
    
    s_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    s_a.sin_family = AF_INET;
    s_a.sin_port = htons(P);
    s_a.sin_addr.s_addr = inet_addr(S_IP);
    
    connect(s_fd, (struct sockaddr*)&s_a, sizeof(s_a));
    printf("Connected to server\n");
    
    // Start thread to receive files
    pthread_t t;
    pthread_create(&t, NULL, receive_files, NULL);
    
    while(1) {
        printf("client> ");
        fgets(buf, B, stdin);
        
        if(buf[strlen(buf)-1] == '\n') buf[strlen(buf)-1] = '\0';
        
        if(strncmp(buf, "send ", 5) == 0) {
            char f_name[50];
            sscanf(buf + 5, "%s", f_name);
            
            FILE* f = fopen(f_name, "rb");
            if(f) {
                // Send command to server
                send(s_fd, "SEND", 4, 0);
                usleep(10000);
                // Send filename
                send(s_fd, f_name, strlen(f_name), 0);
                usleep(10000);
                
                printf("Sending %s to server...\n", f_name);
                clock_t start = clock();
                
                int bytes;
                while((bytes = fread(buf, 1, B, f)) > 0) {
                    send(s_fd, buf, bytes, 0);
                }
                
                clock_t end = clock();
                double t = ((double)(end - start)) / CLOCKS_PER_SEC;
                fclose(f);
                printf("File sent in %.3f seconds\n", t);
            } else {
                printf("File not found\n");
            }
        }
        else if(strcmp(buf, "exit") == 0) {
            break;
        }
    }
    
    close(s_fd);
    return 0;
}
