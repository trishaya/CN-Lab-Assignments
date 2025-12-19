#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_IP "10.0.0.1"
#define PORT 55550
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char fruit[20];
    
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(client_socket < 0) {
        perror("Socket creation failed");
        exit(1);
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    
    if(connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(1);
    }
    
    printf("Connected to Fruit Store Server at %s:%d\n", SERVER_IP, PORT);
    
    memset(buffer, 0, BUFFER_SIZE);
    recv(client_socket, buffer, BUFFER_SIZE-1, 0);
    printf("%s", buffer);
    
    printf("Enter fruit name: ");
    fgets(fruit, sizeof(fruit), stdin);
    
    send(client_socket, fruit, strlen(fruit), 0);
    
    memset(buffer, 0, BUFFER_SIZE);
    recv(client_socket, buffer, BUFFER_SIZE-1, 0);
    printf("Server response: %s\n", buffer);
    
    close(client_socket);
    printf("Disconnected from server\n");
    
    return 0;
}
