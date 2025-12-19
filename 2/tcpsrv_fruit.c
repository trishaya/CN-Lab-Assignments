#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 55550
#define MAX_CLIENTS 5

struct Fruit {
    char name[20];
    int quantity;
};

struct Fruit fruits[] = {
    {"apple", 10},
    {"banana", 5}, 
    {"orange", 8},
    {"grape", 1}
};
int fruit_count = 4;

struct Client {
    char ip[16];
    int port;
};
struct Client clients[100];
int client_count = 0;

void add_client(char* ip, int port) {
    for(int i = 0; i < client_count; i++) {
        if(strcmp(clients[i].ip, ip) == 0 && clients[i].port == port) {
            return;
        }
    }
    
    // Add new client
    strcpy(clients[client_count].ip, ip);
    clients[client_count].port = port;
    client_count++;
    printf("New customer: %s:%d\n", ip, port);
}

void show_clients() {
    printf("\n=== Customer List ===\n");
    printf("Total unique customers: %d\n", client_count);
    for(int i = 0; i < client_count; i++) {
        printf("Customer %d: %s:%d\n", i+1, clients[i].ip, clients[i].port);
    }
    printf("\n\n");
}

int main() {
    int server, client;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[1024];
    
    server = socket(AF_INET, SOCK_STREAM, 0);
    if(server < 0) {
        perror("Socket failed");
        exit(1);
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    if(bind(server, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }
    
    if(listen(server, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        exit(1);
    }
    
    printf("Fruit Store Server started on port %d\n", PORT);
    printf("Available fruits:\n");
    for(int i = 0; i < fruit_count; i++) {
        printf("- %s: %d\n", fruits[i].name, fruits[i].quantity);
    }
    printf("\n");
    
    while(1) {
        client = accept(server, (struct sockaddr*)&client_addr, &addr_len);
        if(client < 0) {
            perror("Accept failed");
            continue;
        }
        
        char client_ip[16];
        int client_port;
        strcpy(client_ip, inet_ntoa(client_addr.sin_addr));
        client_port = ntohs(client_addr.sin_port);
        
        printf("Client connected: %s:%d\n", client_ip, client_port);
        
        add_client(client_ip, client_port);
        
        char welcome[] = "Welcome to Fruit Store! Available: apple, banana, orange, grape\n";
        send(client, welcome, strlen(welcome), 0);
        
        memset(buffer, 0, sizeof(buffer));
        int bytes = recv(client, buffer, sizeof(buffer)-1, 0);
        if(bytes > 0) {
            buffer[bytes] = '\0';
            printf("Client %s:%d wants: %s\n", client_ip, client_port, buffer);
            
            if(buffer[strlen(buffer)-1] == '\n') {
                buffer[strlen(buffer)-1] = '\0';
            }
            
            int found = 0;
            char response[100];
            
            for(int i = 0; i < fruit_count; i++) {
                if(strcmp(fruits[i].name, buffer) == 0) {
                    found = 1;
                    if(fruits[i].quantity > 0) {
                        fruits[i].quantity--;
                        sprintf(response, "Purchase successful! %s remaining: %d\nTotal customers: %d", 
                                fruits[i].name, fruits[i].quantity, client_count);
                        printf("Sold 1 %s to %s:%d. Remaining: %d\n", 
                               fruits[i].name, client_ip, client_port, fruits[i].quantity);
                    } else {
                        strcpy(response, "Sorry, this fruit is out of stock!");
                        printf("Client %s:%d tried to buy out-of-stock %s\n", 
                               client_ip, client_port, fruits[i].name);
                    }
                    break;
                }
            }
            
            if(!found) {
                strcpy(response, "Fruit not available in store!");
                printf("Client %s:%d requested unknown fruit: %s\n", 
                       client_ip, client_port, buffer);
            }
            
            send(client, response, strlen(response), 0);

            show_clients();
        }
        
        close(client);
        printf("Client %s:%d disconnected\n\n", client_ip, client_port);
    }
    
    close(server);
    return 0;
}
