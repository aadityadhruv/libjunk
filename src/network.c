#include "junk/network.h"
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <net/ethernet.h> /* the L2 protocols */
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>

#define TAG "network"

int tcp_ipv4_send(char* ip, char* port, char* data) {

  int sock;
  struct addrinfo hints, *p, *res;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  if (getaddrinfo(ip, port, &hints, &res) != 0) {
    fprintf(stderr, "%s: getaddrinfo (%d): %s\n", TAG, errno, gai_strerror(errno));
    return errno;
  }

  if ((sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
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


/* eth_bind
 * Bind to a L2 Ethernet address.
 *
 */
int eth_bind(char address[]) {

  int sock;
  struct sockaddr_ll addrinfo;

  if ((sock = socket(AF_PACKET, SOCK_DGRAM, htons(ETH_P_ARP))) == -1) {
    fprintf(stderr, "%s: socket (%d): %s\n", TAG, errno, strerror(errno));
    return errno;
  }
  fprintf(stderr, "%s: socket fd: (%d)\n", TAG, sock);

  memset(&addrinfo, 0, sizeof addrinfo);
  addrinfo.sll_family = AF_PACKET;
  addrinfo.sll_protocol = htons(ETH_P_ARP);
  addrinfo.sll_ifindex = 0;
  addrinfo.sll_pkttype = PACKET_BROADCAST;
  sscanf((char*)address, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
    &addrinfo.sll_addr[0],
    &addrinfo.sll_addr[1],
    &addrinfo.sll_addr[2],
    &addrinfo.sll_addr[3],
    &addrinfo.sll_addr[4],
    &addrinfo.sll_addr[5]
  );
  addrinfo.sll_halen = 6;

  fprintf(stderr, "%s: target mac (%x:%x:%x:%x:%x:%x)\n", TAG, addrinfo.sll_addr[0], addrinfo.sll_addr[1], addrinfo.sll_addr[2], addrinfo.sll_addr[3], addrinfo.sll_addr[4], addrinfo.sll_addr[5]);

  int status = 0;
  if ((status = bind(sock, (struct sockaddr*) &addrinfo, sizeof(addrinfo))) != 0) {
    fprintf(stderr, "%s: bind (%d): %s\n", TAG, errno, strerror(errno));
    return status;
  }

  return sock;
}

/* eth_recv
 * Recv on a L2 Ethernet address.
 *
 */
int eth_recv(int sockfd) {

  return sock;
}
