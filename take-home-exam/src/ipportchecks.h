#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h> 
#include <string.h>
//Checks if an ip-adress is of the right format
int is_valid_ip(char* ip_adress) {
	struct sockaddr_in sa;
    if (inet_pton(AF_INET, ip_adress, &(sa.sin_addr)) <= 0) {
    	return -1;
    }
    return 0;
}
//Checks if a portnumber is of the right format
int is_valid_port(char* port) {
 	if (port != NULL) {
 		if (atoi(port) < 1024) {
 			return -2;
 		}
 		if (strlen(port) > 5|| atoi(port) > 65535) {
 			return -1;
 		}
 	} else {
 		return -3;
 	}
 	return 0;
}