#pragma once


//Layer 4, TCP
#include <linux/if_ether.h>
#include <linux/types.h>
int tcp_ipv4_send(char* ip, char* port, char* data);
int tcp_ipv4_recv(char* ip, char* port, char* data);
int ipv4_bind(char* ip, char* port, char* data);


//Layer 2, Ethernet
typedef struct ethhdr ethhdr;

typedef struct arp_packet {
  __u16 hardware_type;
  __u16 protocol_type;
  __u8 hardware_length;
  __u8 protocol_length;
  __u16 operation;
  unsigned char sender_hardware_address[6];
  unsigned char sender_protocol_address[4];
  unsigned char target_hardware_address[6];
  unsigned char target_protocol_address[4];
  } arp_packet;

int eth_arp_bind(char address[]);
int eth_arp_send(int sockfd, arp_packet* data);
int eth_arp_recv(int sockfd, arp_packet* packet);
