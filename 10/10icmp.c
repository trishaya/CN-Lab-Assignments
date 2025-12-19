#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip_icmp.h>
#include <time.h>

unsigned short checksum(void *b, int len) {
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;
    if (len == 1)
        sum += *(unsigned char*)buf;

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

int main() {
    int sockfd;
    char packet[64];

    struct icmphdr *icmp = (struct icmphdr*)packet;

    icmp->type = ICMP_TIMESTAMP;   // Type 13
    icmp->code = 0;
    icmp->un.echo.id = htons(1234);
    icmp->un.echo.sequence = htons(1);
    icmp->checksum = 0;

    icmp->checksum = checksum(icmp, sizeof(struct icmphdr));

    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

    struct sockaddr_in dest;
    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = inet_addr("10.0.0.2");

    sendto(sockfd, packet, sizeof(struct icmphdr), 0,
           (struct sockaddr*)&dest, sizeof(dest));

    printf("ICMP Timestamp request sent\n");
    close(sockfd);
    return 0;
}
