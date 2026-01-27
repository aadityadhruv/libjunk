#ifdef __linux__
#include "junk/network.h"
#include <arpa/inet.h>
#include <linux/if_link.h>
#include <net/if.h>
#include <errno.h>
#include <linux/if_arp.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <net/ethernet.h> /* the L2 protocols */
#include <netdb.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <ifaddrs.h>

#define TAG "network"

int tcp_ipv4_send(char *ip, char *port, char *data) {

  int sock;
  struct addrinfo hints, *p, *res;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  if (getaddrinfo(ip, port, &hints, &res) != 0) {
    fprintf(stderr, "%s: getaddrinfo (%d): %s\n", TAG, errno,
            gai_strerror(errno));
    return errno;
  }

  if ((sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) ==
      -1) {
    fprintf(stderr, "%s: socket (%d): %s\n", TAG, errno, strerror(errno));
    return errno;
  }

  if (connect(sock, res->ai_addr, res->ai_addrlen) == -1) {
    fprintf(stderr, "%s: connect (%d): %s\n", TAG, errno, strerror(errno));
    return errno;
  }

  int len = strlen(data);
  int bytes_sent = 0;
  int total_bytes_sent = 0;

  while (1) {
    if ((bytes_sent = send(sock, data, len, 0)) == -1) {
      fprintf(stderr, "%s: send (%d): %s\n", TAG, errno, strerror(errno));
      return errno;
    }
    total_bytes_sent += bytes_sent;
    if (bytes_sent == len) {
      break;
    }
  }
  close(sock);
  freeaddrinfo(res);
  return 0;
}

/* eth_arp_bind
 * Bind to a L2 Ethernet address.
 *
 */
int eth_arp_bind(char address[]) {

  int sock;
  struct sockaddr_ll addrinfo;

  if ((sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP))) == -1) {
    fprintf(stderr, "%s: socket (%d): %s\n", TAG, errno, strerror(errno));
    return errno;
  }
  fprintf(stderr, "%s: socket fd: (%d)\n", TAG, sock);

  memset(&addrinfo, 0, sizeof addrinfo);
  addrinfo.sll_family = AF_PACKET;
  addrinfo.sll_protocol = htons(ETH_P_ARP);
  addrinfo.sll_ifindex = 0;
  addrinfo.sll_pkttype = PACKET_BROADCAST;
  sscanf((char *)address, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
         &addrinfo.sll_addr[0], &addrinfo.sll_addr[1], &addrinfo.sll_addr[2],
         &addrinfo.sll_addr[3], &addrinfo.sll_addr[4], &addrinfo.sll_addr[5]);
  addrinfo.sll_halen = 6;

  fprintf(stderr, "%s: target mac (%02x:%02x:%02x:%02x:%02x:%02x)\n", TAG,
          addrinfo.sll_addr[0], addrinfo.sll_addr[1], addrinfo.sll_addr[2],
          addrinfo.sll_addr[3], addrinfo.sll_addr[4], addrinfo.sll_addr[5]);

  int status = 0;
  if ((status = bind(sock, (struct sockaddr *)&addrinfo, sizeof(addrinfo))) !=
      0) {
    fprintf(stderr, "%s: bind (%d): %s\n", TAG, errno, strerror(errno));
    return status;
  }

  return sock;
}

/* eth_arp_recv
 * Recv on a L2 Ethernet address.
 *
 */
int eth_arp_recv(int sockfd, arp_packet *packet) {
  int buffer_len = sizeof(ethhdr) + sizeof(arp_packet);
  char buffer[buffer_len];
  memset(buffer, 0, buffer_len);
  int bytes_read = 0;
  int total_bytes_read = 0;

  while (1) {
    bytes_read = recv(sockfd, &buffer[total_bytes_read],
                      buffer_len - total_bytes_read, 0);
    if (bytes_read == -1) {
      fprintf(stderr, "%s: recv (%d): %s\n", TAG, errno, strerror(errno));
      return -1;
    } else if (bytes_read > 0) {
      fprintf(stderr, "%s: recv: read %d bytes, total %d\n", TAG, bytes_read,
              total_bytes_read);
      total_bytes_read += bytes_read;
    } else {

      ethhdr *recv_struct = (ethhdr *)buffer;
      fprintf(stderr, "%s: source mac (%02x:%02x:%02x:%02x:%02x:%02x)\n", TAG,
              recv_struct->h_source[0], recv_struct->h_source[1],
              recv_struct->h_source[2], recv_struct->h_source[3],
              recv_struct->h_source[4], recv_struct->h_source[5]);
      fprintf(stderr, "%s: dest mac (%02x:%02x:%02x:%02x:%02x:%02x)\n", TAG,
              recv_struct->h_dest[0], recv_struct->h_dest[1],
              recv_struct->h_dest[2], recv_struct->h_dest[3],
              recv_struct->h_dest[4], recv_struct->h_dest[5]);

      memcpy((char *)packet, (char *)(buffer + sizeof(ethhdr)),
             sizeof(arp_packet));
      return 0;
    }
  }
}

int eth_arp_send(int sockfd, arp_packet *packet) {
  char buffer[sizeof(ethhdr) + sizeof(arp_packet)];
  memset(buffer, 0, sizeof(buffer));
  ethhdr *eth_header = (ethhdr *)buffer;
  memcpy(eth_header->h_source, packet->sender_hardware_address,
         sizeof(packet->sender_hardware_address));
  memcpy(eth_header->h_dest, packet->target_hardware_address,
         sizeof(packet->target_hardware_address));
  eth_header->h_proto = htons(ETH_P_ARP);
  memcpy(buffer + sizeof(ethhdr), packet, sizeof(arp_packet));

  struct sockaddr_ll addrinfo;
  memset(&addrinfo, 0, sizeof addrinfo);
  addrinfo.sll_family = AF_PACKET;
  addrinfo.sll_protocol = htonl(ETH_P_ARP);
  struct ifaddrs *ifaddr;
  if (getifaddrs(&ifaddr) == -1) {
       perror("getifaddrs");
  }

  for (struct ifaddrs* ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
    if (ifa->ifa_addr == NULL) {
      continue;
    }
    int family = ifa->ifa_addr->sa_family;
    if (family == AF_PACKET) {
      struct sockaddr_ll* hwaddr = (struct sockaddr_ll*)ifa->ifa_addr;
      if (memcmp(hwaddr->sll_addr, packet->sender_hardware_address, 6) == 0) {
        fprintf(stderr, "Found intf ifindex %d\n", hwaddr->sll_ifindex);
        addrinfo.sll_ifindex = hwaddr->sll_ifindex;
      }
    }
  }


  addrinfo.sll_hatype = htonl(ARPHRD_ETHER);
  addrinfo.sll_pkttype = PACKET_BROADCAST;
  addrinfo.sll_halen = 6;
  memcpy(addrinfo.sll_addr, packet->target_hardware_address, sizeof(packet->target_hardware_address));

 fprintf(stderr, "network: send to %hhx:%hhx:%hhx:%hhx:%hhx:%hhx\n", addrinfo.sll_addr[0],addrinfo.sll_addr[1],addrinfo.sll_addr[2],addrinfo.sll_addr[3],addrinfo.sll_addr[4],addrinfo.sll_addr[5]);

  int bytes_sent = 0;
  int total_bytes_sent = 0;
  while (1) {
    bytes_sent = sendto(sockfd, buffer + total_bytes_sent,
                      sizeof(buffer) - total_bytes_sent, 0, (struct sockaddr*)&addrinfo, sizeof(addrinfo));

    if (bytes_sent == -1) {
      fprintf(stderr, "%s: send (%d): %s\n", TAG, errno, strerror(errno));
      return -1;
    } else if (bytes_sent < sizeof(buffer)) {
      fprintf(stderr, "%s: send: sent %d bytes, total %d\n", TAG, bytes_sent,
              total_bytes_sent);
      total_bytes_sent += bytes_sent;
    } else {
      fprintf(stderr, "%s: send: sent %d bytes, total %d\n", TAG, bytes_sent,
              total_bytes_sent);
      return 0;
    }
  }
}

#endif
