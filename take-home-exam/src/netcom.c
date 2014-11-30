#include "netcom.h"
#include "client_handler.h"
#include "server_handler.h"
#include "engine.h"

int BUFSIZE = 300;
int send_pkg(char* port, char* ip_address, char *package, int size) 
{ 
   /*
   Package recieved from the server (if any)
   */
   char rec_pack[BUFSIZE];
   /*
   Client adress
   */
   struct sockaddr_in client;
   /*
   Socket for the client
   */
   int socket_desc;
   memset(&client, 0, sizeof(struct sockaddr_in));

   /*
   Sets the information to the client adress
   */
   client.sin_family = AF_INET;
   inet_pton(client.sin_family, ip_address, &client.sin_addr);  
   client.sin_port = htons(atoi(port));

   /*
   Makes a socket for the client with the TCP-protocol.
   */
   socket_desc = socket(client.sin_family, SOCK_STREAM, IPPROTO_TCP);
    
   if (socket_desc < 0) 
   {
      return -2;
   }
   /*
   Connects the socket with the server
   */
   if (connect(socket_desc, (struct sockaddr*)&client, sizeof(struct sockaddr_in)) < 0) 
   {
      return -1;
   }
   /*
   Package is being sent, and a call for recieving a package in return form the server 
   is made (if 0, no send from the server, if -1; error)
   */
   verify_send(socket_desc, package, BUFSIZE);
   int recieve = recv(socket_desc, rec_pack, BUFSIZE, 0);
   if (recieve <= 0) {
      if (recieve != 0) {
         perror("Error in recv()");
      } 
   } else {
      int send = decide_package_type(rec_pack);
      if (send == 2) {
         close(socket_desc);
         return -3;
      }
   }
   //Close the connection
   close(socket_desc);
   return 0;
}
/*
Makes sure no packages just partially gets sent
*/
void verify_send(int socket, char *package, int pack_bytes) 
{
   int sent = 0;
   int bytes_to_send = pack_bytes;
   int cnt;
   /*
   send() returns number of bytes sent (if not error), therefore a check to see if all bytes
   are sent (comparing the return value with the size of the package being sent) is done,
   and forcing another send() call if not all bytes were sent the first time. This way split
   packages are controlled.
   */
   for (cnt = send(socket, package+sent, bytes_to_send, 0); cnt < pack_bytes; sent += cnt) 
   {
      if (cnt == -1) {
         perror("Error in send()");
      } 
   /*
   If entering the for(), remaining bytes to send is decreased with the number of bytes already
   sent, to make sure the sent bytes are not sent more than one time 
   */
   bytes_to_send -= cnt;
   }
}
void start_server(char* port, int check) 
{  
   /*
   Declaring an adress for the server and for the client who is sending something
   */
   struct sockaddr_in server, client;
   memset(&server, 0, sizeof(struct sockaddr_in));
   /*
   Sets the server adress information
   */
   server.sin_family = AF_INET;
   server.sin_addr.s_addr = INADDR_ANY;
   server.sin_port = htons(atoi(port));
   /*
   Package recieved from the client is stored in a char buffer, to be 
   decrypted in package_handler.c
   */
   char package[BUFSIZE];
   memset(&package, 0, sizeof(package));

   /*
   Address size to the (eventually) connected client
   */
   socklen_t addr_size;

   /*
   Creates a socket for listening, and a socket that will
   handle accepting a connection from client
   A counter (max_fd_counter) is needed for knowing 
   the maximum file descriptor
   */
   int listening_sock, acc_sock, bytes_recieved;
   int max_fd_counter, i, j = 1;
   /*
   To use select(), I'll make a file descriptor for reading
   from the socket listening for connections
   */
   fd_set read_fd;
   /*
   Clear the file descriptors
   */
   FD_ZERO(&read_fd);
   /*
   A socket is created for the server
   */
   listening_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
   if (listening_sock < 0) {
      perror("Error in socket()");
   }
  
   /*
   Binds the socket at the server-adress, so it (eventually) can listen
   for connections. If "address in use"-error, forces a reset of the address
   with setsockopt();
   */
   while (bind(listening_sock, (struct sockaddr*)&server, sizeof(struct sockaddr_in)) < 0) {
      perror("Error in bind()");
      printf("Resetting adress....\n");
      setsockopt(listening_sock, SOL_SOCKET, SO_REUSEADDR, &j, sizeof(int)); 
   }
   sleep(1);
   printf("Good to go!\n");

   /*
   Sets up the listening socket for listening. This is all listening_socket
   will do. A different integer will be set for accepting and recieving 
   the data from the client
   */
   if (listen(listening_sock, 10) == -1) {
     perror("Error in listen()");
     exit(EXIT_SUCCESS);
   }
   /*
   Adds the listening socket to the file descriptor needed by select()
   */
   FD_SET(listening_sock, &read_fd);
   /*
   The currently largest file descriptor.
   */
   max_fd_counter = listening_sock;
   /*
   For multiplayer
   */
   while(1) {
      /*
      Sets read_fd to read to the file descriptor from whatever comes
      from the connected client
      */
      if (select(max_fd_counter+1, &read_fd, NULL, NULL, NULL) == -1) {
         perror("Error in select()");
      }
      for (i = 0; i <= max_fd_counter; i++) {
         /*
         Checks if the current integer is in the set
         */
         if(FD_ISSET(i, &read_fd)) {
            /*If i is in the file descriptor set, I need to check if has the 
            same value as the listening socket (which should only
            keep listening after a connection). If it has, it means 
            that the listening socket has found something to read.
            */
            if (listening_sock == i) {
               /*
               Sets the address size of the connected client
               */
               addr_size = sizeof(client);
               /*
               Uses a new integer to accept a connection from the client on the socket
               */
               acc_sock = accept(listening_sock, (struct sockaddr*)&client, &addr_size);
               if (acc_sock < 0) {
                  perror("Error in accept()");
               } else {
                  //Adds the new socket to the set
                  FD_SET(acc_sock, &read_fd);
                  /*
                  Increases the max-value to be sure to get to the newly accepted socket
                  */
                  if (acc_sock > max_fd_counter) {
                     max_fd_counter = acc_sock;
                  }
               }
            } else {
               printf("\n\n\n-----------------------------------\n");
               printf("Connected to client. Trying to recieve!\n");
               /*
               Checks to see if nr of bytes is 0, if so it indicates that
               the client has closed the connection. Calls recv to recieve
               from the connected socket (i), and sends a confirmation
               of recieved package back to the client as verification
               */
               if ((bytes_recieved = recv(i, package, BUFSIZE, 0) <= 0)) {
                  if (bytes_recieved == 0) {
                     printf("Connection closed!\n");
                  } else {
                     perror("Error in recv()");
                  }
                //Clears the set from the file descriptor, and closes the accept()
                  FD_CLR(i, &read_fd);
                  close(i);

               } else {
                  /*
                  Validates the package to check if size and msg type is correct
                  */
                  int package_validation = decide_package_type(package);
                  /* 
                  To be set if a package is to be sent back
                  */
                  int send_check = 0;
                  /*
                  Different checks to determine what msg to print
                  */
                     if (package_validation == 1) {
                       printf("Sending highscore list back to client!\n");
                       send_check = 1;
                     } else if (package_validation == 4) {
                        printf("Player ready to start playing!\n");
                        printf("Sending setup package back to the player!\n");
                        send_check = 1;
                     } else if (package_validation == 5) {
                        printf("Sending start package back to the other player!\n");
                        send_check = 1;
                     } else if (package_validation == 6) {
                        printf("Sending update package back to the other player!\n");
                        send_check = 1;
                     } else if (package_validation == 7) {
                        printf("Sending end package back to the other player!\n");
                        send_check = 1;
                     } else if (package_validation == 3) {
                        printf("Sending error package back to client!\n");
                        send_check = 1;
                     } else if (package_validation == -1) {
                        printf("Package too large! Sending error package back to client!\n");
                        send_check = 1;
                     } else if (package_validation == -2) {
                        printf("Unknown package type! Sending error package back to client!\n");
                        send_check = 1;
                     } else if (package_validation == -3) {
                        printf("No more players allowed!! Sending error package back to client!\n");
                        send_check = 1;
                     }
                  /*
                  Sending package back if it should, determined by the decice_package_type function
                  */
                  if (send_check == 1) {
                     if (send(i, fill_buf, BUFSIZE, 0) < 0) {
                        perror("Error in send()");
                     } else {
                     printf("Send successful\n");

                     }
                  }
               }
               /*
               Closes the accept(), and clears it from the set so
               another can be added at another run in the while
               (e.g. if the current client has closed the connection and
               another comes in)
               */
               FD_CLR(i, &read_fd);
               close(i);
            }
         } 
      }
   }
}