#pragma once


//Layer 4, TCP
int tcp_ipv4_send(char* ip, char* port, char* data);
int tcp_ipv4_recv(char* ip, char* port, char* data);
int ipv4_bind(char* ip, char* port, char* data);


//Layer 2, Ethernet
int eth_bind(char address[]);
int eth_send(int sockfd, char* data);
int eth_recv(int sockfd);
