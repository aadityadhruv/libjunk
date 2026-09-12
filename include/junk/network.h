#pragma once


//Layer 4, TCP
#include <linux/if_ether.h>
#include <linux/types.h>
/*
 * Connect to an IP and port. This function creates a socket and connect() to the provided
 * IP and Port
 * @param ip Destination IP
 * @param port Destination port
 * @return The socket with an open connection, -1 if we failed to connect
 */
int junk_tcp_ipv4_connect(char *ip, char* port);
/*
 * Send data over a connected socket. Usually used after calling junk_tcp_ipv4_connect
 * The function will attempt to send the whole block of data
 * @param data Data buffer to send
 * @param size Size of the data buffer
 * @return 0 on success, -1 on error
 */
int junk_tcp_ipv4_send(int sock, char *data, int size);
/*
 * Bind an IP address and port to a socket. The function will return a socket
 * if the bind was successful
 * @param ip IP address to bind to
 * @param port Port to bind to
 * @return socket fd if successful, -1 if error
 */
int junk_tcp_ipv4_bind(char* ip, char* port);
/*
 * Receive data over a connected socket.
 * The function will attempt to recv the whole block of data
 * @param data Data buffer to write output to
 * @param size Amount of bytes to write
 * @return 0 on success, -1 on error
 */
int junk_tcp_ipv4_recv(int sock, char *data, int size);


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

int junk_eth_arp_bind(char address[]);
int junk_eth_arp_send(int sockfd, arp_packet* data);
int junk_eth_arp_recv(int sockfd, arp_packet* packet);
