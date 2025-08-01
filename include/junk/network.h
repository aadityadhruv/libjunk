#pragma once


//Layer 4, TCP
#include <linux/if_ether.h>
int tcp_ipv4_send(char* ip, char* port, char* data);
int tcp_ipv4_recv(char* ip, char* port, char* data);
int ipv4_bind(char* ip, char* port, char* data);


//Layer 2, Ethernet
typedef struct ethhdr ethhdr;

typedef struct arp_packet {
  unsigned char hardware_type[2];
  unsigned char protocol_type[2];
  unsigned char hardware_length[1];
  unsigned char protocol_length[1];
  unsigned char operation[2];
  unsigned char sender_hardware_address[6];
  unsigned char sender_protocol_address[4];
  unsigned char target_hardware_address[6];
  unsigned char target_protocol_address[4];
  } arp_packet;

int eth_bind(char address[]);
int eth_send(int sockfd, char* data);
int eth_recv(int sockfd, arp_packet* packet);
