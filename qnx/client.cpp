#include "client.h"

/*
 * client.cpp
 *
 *  Created on: 25/07/2012
 *      Author: C.G Ledezma
 */

int runClient()
{
   int sockfd, portno = 10602, n;
   struct sockaddr_in *serv_addr, *local;
   struct hostent *server;
   char rev[28];

   if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
   {
      fprintf(stderr, "Error creating socket. Aborting client\n");
      exit(0);
   }
   fprintf(stderr, "Socket created successfully\n");
   server = gethostbyname("192.168.136.128");
   fprintf(stderr, "Hostname obtained\n");

   serv_addr = (sockaddr_in *)malloc(sizeof(sockaddr_in));
   serv_addr->sin_family = AF_INET;
   fprintf(stderr, "sin_family set\n");
   bcopy((char *)server->h_addr,
        (char *) &serv_addr->sin_addr.s_addr,
        server->h_length);
   fprintf(stderr, "bcopy executed\n");
   serv_addr->sin_port = htons(10602);
   fprintf(stderr, "sin_port set\n");

   fprintf(stderr, "Connecting to the socket\n");
   if(connect(sockfd, (struct sockaddr *)serv_addr, sizeof(*serv_addr)) < 0)
   {
      fprintf(stderr, "Error establishing a connection. Aborting client\n");
      exit(0);
   }

   fprintf(stderr, "Waiting for server to send reply\n");
   read(sockfd, rev, 28);
   fprintf(stderr, "Message received: %s\n", rev);
   return 0;
}



