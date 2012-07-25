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
   struct sockaddr_in serv_addr;
   struct hostent *server;

   char buffer[256] = "Hi, sending data from client";

   sockfd = socket(AF_INET, SOCK_STREAM, 0);

   if (sockfd < 0)
       printf("ERROR opening socket");

   server = gethostbyname("192.168.136.132");
   if (server == NULL) {
       fprintf(stderr,"ERROR, no such host\n");
       exit(0);
   }
   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   bcopy((char *)server->h_addr,
        (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);
   serv_addr.sin_port = htons(portno);
   if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
       printf("ERROR connecting");

   n = write(sockfd,buffer,strlen(buffer));
   if (n < 0)
        printf("ERROR writing to socket");

   bzero(buffer,256);
   n = read(sockfd,buffer,255);
   if (n < 0)
        printf("ERROR reading from socket");
   printf("%s\n",buffer);
   close(sockfd);
   return 0;
}



