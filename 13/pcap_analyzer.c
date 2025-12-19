#include <pcap.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#include <netinet/ip_icmp.h>
#include <time.h>

void packet_handler(u_char *args,
                    const struct pcap_pkthdr *header,
                    const u_char *packet)
{
    struct ether_header *eth;
    struct ip *ip_hdr;
    struct icmphdr *icmp;

    eth = (struct ether_header *) packet;

    printf("Time: %ld.%06ld  ",
           header->ts.tv_sec,
           header->ts.tv_usec);

    /* L2 */
    if (ntohs(eth->ether_type) == ETHERTYPE_ARP) {
        printf("L2: ARP\n");
        return;
    }

    if (ntohs(eth->ether_type) == ETHERTYPE_IP) {
        printf("L2: Ethernet  ");

        ip_hdr = (struct ip *)(packet + sizeof(struct ether_header));

        /* L3 */
        if (ip_hdr->ip_p == IPPROTO_ICMP) {
            printf("L3: IP  L4: ICMP\n");
        } else if (ip_hdr->ip_p == IPPROTO_TCP) {
            printf("L3: IP  L4: TCP\n");
        } else if (ip_hdr->ip_p == IPPROTO_UDP) {
            printf("L3: IP  L4: UDP\n");
        } else {
            printf("L3: IP  L4: Other\n");
        }
    }
}

int main() {
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;

    handle = pcap_open_offline("ping_capture.pcap", errbuf);
    if (handle == NULL) {
        printf("Error opening file: %s\n", errbuf);
        return 1;
    }

    printf("Time Diagram (Protocol Occurrence)\n");
    printf("---------------------------------\n");

    pcap_loop(handle, 0, packet_handler, NULL);

    pcap_close(handle);
    return 0;
}
