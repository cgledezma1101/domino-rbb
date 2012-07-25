/*
 * server.cpp
 *
 *  Created on: 25/07/2012
 *      Author: C.G Ledezma
 */
#include "server.h"

int runServer()
{
   int sockfd, newsockfd, portno = 10602;
   socklen_t clilen;
   char buffer[256];
   struct sockaddr_in serv_addr, cli_addr;
   int n;
   sockfd = socket(AF_INET, SOCK_STREAM, 0);

   if (sockfd < 0)
      printf("ERROR opening socket\n");

   bzero((char *) &serv_addr, sizeof(serv_addr));

   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);

   if (bind(sockfd, (struct sockaddr *) &serv_addr,
            sizeof(serv_addr)) < 0)
            printf("ERROR on binding\n");

   printf("Getting ready to listen to socket\n");
   listen(sockfd,5);
   printf("Already listening to socket\n");
   clilen = sizeof(cli_addr);
   printf("Waiting to accept a connection\n");
   newsockfd = accept(sockfd,
               (struct sockaddr *) &cli_addr,
               &clilen);
   printf("A connection was accepted\n");
   if (newsockfd < 0)
        printf("ERROR on accept\n");

   bzero(buffer,256);

   n = read(newsockfd,buffer,255);
   if (n < 0) printf("ERROR reading from socket\n");

   printf("Here is the message: %s\n",buffer);

   n = write(newsockfd,"I got your message",18);
   if (n < 0) printf("ERROR writing to socket\n");
   close(newsockfd);
   close(sockfd);
   return 0;
}


