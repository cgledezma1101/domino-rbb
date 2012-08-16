#include "client.h"

/*
 * client.cpp
 *
 *  Created on: 25/07/2012
 *      Author: C.G Ledezma
 */

int runClient()
{
   return 0;
}

int initializeConnection(char *IP)
{
   int sockfd;
   struct sockaddr_in *serv_addr, *local;
   struct hostent *server;
   char rev[28];

   if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
   {
      fprintf(stderr, "Error creating socket. Aborting client\n");
      exit(0);
   }
   fprintf(stderr, "Socket created successfully\n");

   server = gethostbyname(IP);
   fprintf(stderr, "Hostname obtained\n");

   serv_addr = (sockaddr_in *)malloc(sizeof(sockaddr_in));
   serv_addr->sin_family = AF_INET;
   bcopy((char *)server->h_addr, (char *) &serv_addr->sin_addr.s_addr, server->h_length);
   serv_addr->sin_port = htons(10602);

   fprintf(stderr, "Connecting to the socket\n");
   if(connect(sockfd, (struct sockaddr *)serv_addr, sizeof(*serv_addr)) < 0)
   {
      fprintf(stderr, "Error establishing a connection. Aborting client\n");
      exit(0);
   }

   free(serv_addr);
   free(server);
   return sockfd;
}

int sendPlay(int v0, int v1, int s, int serv)
{
   char ms[4];

   //Build the message to be sent
   ms[0] = 'j';
   ms[1] = intToAsc(v0);
   ms[2] = intToAsc(v1);
   ms[3] = intToAsc(s);

   //Send the message through the socket
   if(-1 == write(serv, ms, 4))
   {
      fprintf(stderr, "Error sending play to the server\n");
      return 0;
   }

   return 1;
}

int sendPass(int serv)
{
   char ms = 'n';

   if(-1 == write(serv, &ms, 1))
   {
      fprintf(stderr, "Error notifying server of the pass\n");
      return 0;
   }

   return 1;
}

int sendPiecesLeft(int *pieces, int numPieces, int serv)
{
   char ms[2 + (2 * numPieces)];
   int i;

   //Build the message
   ms[0] = 'z';
   ms[1] = intToAsc(numPieces);

   for(i = 0; i < 2 * numPieces; ++ i)
   {
      ms[2 + i] = intToAsc(pieces[i]);
   }

   if(-1 == write(serv, ms, 2 + (2 * numPieces)))
   {
      fprintf(stderr, "Error sending the hand to the server\n");
      return 0;
   }

   return 1;
}
char intToAsc(int n)
{
   char arr[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

   return arr[n];
}
