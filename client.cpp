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
   char rev[15];

   local = (sockaddr_in *)malloc(sizeof(sockaddr_in));
   serv_addr = (sockaddr_in *)malloc(sizeof(sockaddr_in));

   n = sizeof(*serv_addr);
   local->sin_family = AF_INET;
   local->sin_port = htons(10602);
   local->sin_addr.s_addr = INADDR_ANY;

   sockfd = socket(AF_INET, SOCK_DGRAM, 0);

   bind(sockfd, (sockaddr *)local, sizeof(*local));

   fprintf(stderr, "Socket bound, waiting to receive broadcast");
   recvfrom(sockfd, rev, 14 * sizeof(char), 0, (sockaddr *)serv_addr, (socklen_t *)&n);

   fprintf(stderr, "Message received: %s\n", rev);
   fprintf(stderr, "From: %s\n", inet_ntoa(serv_addr->sin_addr));
   return 0;
}



