/*
Header for the netcom.c-file
*/
#ifndef NETCOM
#define NETCOM

#include <netinet/in.h> 
#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

int send_pkg(char* port, char* ip_address, char *package, int size);
void start_server(char* port, int check);
void start_multi_server(char* port, int check);
void verify_send(int socket, char *package, int pack_bytes);
#endif