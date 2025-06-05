#include "junk/network.h"
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

//TCP SEND
int tcp_ipv4_send_packet(char* ip, char* port, char* data) {

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


// TCP BIND
int tcp_ipv4_b
