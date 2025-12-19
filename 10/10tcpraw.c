#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

#define SRC_IP  "10.0.0.1"
#define DEST_IP "10.0.0.2"

/* Checksum calculation */
unsigned short checksum(unsigned short *ptr, int nbytes) {
    long sum = 0;
    unsigned short oddbyte;
    short answer;

    while (nbytes > 1) {
        sum += *ptr++;
        nbytes -= 2;
    }
    if (nbytes == 1) {
        oddbyte = 0;
        *((u_char*)&oddbyte) = *(u_char*)ptr;
        sum += oddbyte;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = (short)~sum;
    return answer;
}

int main() {
    int sockfd;
    char packet[4096];

    struct iphdr *ip = (struct iphdr*)packet;
    struct tcphdr *tcp = (struct tcphdr*)(packet + sizeof(struct iphdr));
    char *data = packet + sizeof(struct iphdr) + sizeof(struct tcphdr);

    char payload[] = "ROLL: CSM2018";
    int payload_len = strlen(payload);
    strcpy(data, payload);

    /* IP header */
    ip->ihl = 5;
    ip->version = 4;
    ip->tos = 0;
    ip->tot_len = htons(sizeof(struct iphdr) + sizeof(struct tcphdr) + payload_len);
    ip->id = htons(54321);
    ip->frag_off = 0;
    ip->ttl = 64;
    ip->protocol = IPPROTO_TCP;
    ip->saddr = inet_addr(SRC_IP);
    ip->daddr = inet_addr(DEST_IP);
    ip->check = checksum((unsigned short*)ip, sizeof(struct iphdr));

    /* TCP header */
    tcp->source = htons(4444);
    tcp->dest = htons(80);
    tcp->seq = htonl(1);
    tcp->ack_seq = 0;
    tcp->doff = 5;
    tcp->syn = 1;
    tcp->window = htons(65535);
    tcp->check = 0;
    tcp->urg_ptr = 0;

    /* Create RAW socket */
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);

    struct sockaddr_in dest;
    dest.sin_family = AF_INET;
    dest.sin_addr.s_addr = inet_addr(DEST_IP);

    sendto(sockfd, packet,
           sizeof(struct iphdr) + sizeof(struct tcphdr) + payload_len,
           0, (struct sockaddr*)&dest, sizeof(dest));

    printf("Raw TCP packet sent\n");
    close(sockfd);
    return 0;
}
