#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define P 55550
#define B 1024

struct F {
    char n[20];
    int q;
};

struct F fs[] = {
    {"apple", 10},
    {"banana", 5}, 
    {"orange", 8},
    {"grape", 3}
};
int f_c = 4;

struct C {
    char ip[16];
    int p;
};
struct C cs[100];
int c_c = 0;

void add_c(char* ip, int p) {
    for(int i = 0; i < c_c; i++) {
        if(strcmp(cs[i].ip, ip) == 0 && cs[i].p == p) return;
    }
    strcpy(cs[c_c].ip, ip);
    cs[c_c].p = p;
    c_c++;
    printf("New: %s:%d\n", ip, p);
}

void show_c() {
    printf("\n=== Customers ===\n");
    printf("Total: %d\n", c_c);
    for(int i = 0; i < c_c; i++) {
        printf("%d: %s:%d\n", i+1, cs[i].ip, cs[i].p);
    }
    printf("=================\n\n");
}

int main() {
    int s_fd;
    struct sockaddr_in s_a, c_a;
    socklen_t a_l = sizeof(c_a);
    char buf[B];
    
    s_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(s_fd < 0) {
        perror("socket fail");
        exit(1);
    }
    
    s_a.sin_family = AF_INET;
    s_a.sin_addr.s_addr = INADDR_ANY;
    s_a.sin_port = htons(P);
    
    if(bind(s_fd, (struct sockaddr*)&s_a, sizeof(s_a)) < 0) {
        perror("bind fail");
        exit(1);
    }
    
    printf("UDP Server on %d\n", P);
    printf("Fruits:\n");
    for(int i = 0; i < f_c; i++) {
        printf("- %s: %d\n", fs[i].n, fs[i].q);
    }
    printf("Waiting...\n\n");
    
    while(1) {
        memset(buf, 0, B);
        int b_r = recvfrom(s_fd, buf, B-1, 0, (struct sockaddr*)&c_a, &a_l);
        
        if(b_r > 0) {
            buf[b_r] = '\0';
            
            char c_ip[16];
            int c_p;
            strcpy(c_ip, inet_ntoa(c_a.sin_addr));
            c_p = ntohs(c_a.sin_port);
            
            printf("From %s:%d: %s\n", c_ip, c_p, buf);
            
            if(buf[strlen(buf)-1] == '\n') buf[strlen(buf)-1] = '\0';
            
            char res[B];
            
            if(strcmp(buf, "LIST") == 0) {
                strcpy(res, "Fruits:\n");
                for(int i = 0; i < f_c; i++) {
                    char f_i[50];
                    sprintf(f_i, "- %s: %d\n", fs[i].n, fs[i].q);
                    strcat(res, f_i);
                }
            }
            else {
                add_c(c_ip, c_p);
                
                int found = 0;
                for(int i = 0; i < f_c; i++) {
                    if(strcmp(fs[i].n, buf) == 0) {
                        found = 1;
                        if(fs[i].q > 0) {
                            fs[i].q--;
                            sprintf(res, "OK! %s left: %d\nCustomers: %d", 
                                    fs[i].n, fs[i].q, c_c);
                            printf("Sold %s to %s:%d. Left: %d\n", 
                                   fs[i].n, c_ip, c_p, fs[i].q);
                        } else {
                            strcpy(res, "Out of stock!");
                            printf("%s:%d tried %s (out)\n", c_ip, c_p, fs[i].n);
                        }
                        break;
                    }
                }
                
                if(!found) {
                    strcpy(res, "No such fruit! Try: apple, banana, orange, grape");
                    printf("%s:%d bad req: %s\n", c_ip, c_p, buf);
                }
                
                show_c();
            }
            
            sendto(s_fd, res, strlen(res), 0, (struct sockaddr*)&c_a, a_l);
        }
    }
    
    close(s_fd);
    return 0;
}
