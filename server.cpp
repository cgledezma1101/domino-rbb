/*
 * server.cpp
 *
 *  Created on: 25/07/2012
 *      Author: C.G Ledezma
 */
#include "server.h"

int runServer()
{
   socklen_t clilen;
   char buffer[256];
   int n, i;
   //File descriptor for the servers socket
   int sockfd = 0;
   //File descriptors to the players sockets
   int playerSocket[4];
   //Structures that describe the players internet addresses
   struct sockaddr_in cliAddr[4];
   //Port used to communicate
   int portno = 10602;


   if(!initializeConnection(&sockfd, portno))
   {
      fprintf(stderr, "Error initializing connection\n");
      exit(1);
   }

   if(!sendBroadcast())
   {
      fprintf(stderr, "Error sending the broadcast message\n");
      exit(1);
   }

   exit(0);

   clilen = sizeof(cliAddr[0]);

   //Wait for all four players to join, which is, open a socket with the server
   for(i = 0; i < 4; ++ i){
      fprintf(stderr, "Waiting to accept connection %d\n", i);
      playerSocket[i] = accept(sockfd,
                  (struct sockaddr *) &cliAddr[i],
                  &clilen);
      fprintf(stderr, "Connection %d was accepted\n", i);
      if (playerSocket[i] < 0)
        printf("ERROR on accept %d\n", i);
   }

   //If here, all four players have established a connection to the server
   return 0;
}

int initializeConnection(int *sockfd, int portno)
{
   //Structure that describes the servers internet address
   struct sockaddr_in servAddr;
   //Creation of the server's socket
   *sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (*sockfd < 0)
      fprintf(stderr, "ERROR opening socket\n");

   bzero((char *) &servAddr, sizeof(servAddr));

   //Initialization of the server's address
   servAddr.sin_family = AF_INET;
   servAddr.sin_addr.s_addr = INADDR_ANY;
   servAddr.sin_port = htons(portno);

   //Binding socket to port
   if (bind(*sockfd, (struct sockaddr *) &servAddr,
      sizeof(servAddr)) < 0)
         fprintf(stderr, "ERROR on binding\n");

   //Listening to connections on the socket
   fprintf(stderr, "Getting ready to listen to socket\n");
   listen(*sockfd,5);
   fprintf(stderr, "Already listening to socket\n");

   return 1;
}

int sendBroadcast()
{
   char serversIp[INET_ADDRSTRLEN], netmask[INET_ADDRSTRLEN];
   struct sockaddr_in *localIp, *subnetMask, *broadcastIp;

   /*
    * Find out server's IP address and the subnet mask, in order to build the
    * braoadcast address
    */

   struct ifaddrs * ifAddrStruct=NULL;
   struct ifaddrs * ifa=NULL;

   //This gets a linked list of structures that describe each network interface
   getifaddrs(&ifAddrStruct);

   //Now we check the IPs of the interfaces in order to find the local IP
   for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next)
   {
      //Check it is IP4
      if (ifa ->ifa_addr->sa_family==AF_INET)
      {
         //Save the interface's address
         localIp = (struct sockaddr_in *)ifa->ifa_addr;
         inet_ntop(AF_INET, &localIp->sin_addr, serversIp, INET_ADDRSTRLEN);

         //If the address is localhost, find another intarface
         if(0 == strcmp(serversIp, "127.0.0.1"))
            continue;

         //Get the subnet mask
         subnetMask = (struct sockaddr_in *)ifa->ifa_netmask;
         inet_ntop(AF_INET, &subnetMask->sin_addr, netmask, INET_ADDRSTRLEN);
         break;
      }
   }

   if(NULL == ifa)
   {
      fprintf(stderr, "No network interface found, check connection\n");
      return 0;
   }
   //Avoid memory leak
   if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);

   fprintf(stderr, "IP Address: %s\n", serversIp);
   fprintf(stderr, "Netmask: %s\n", netmask);

   broadcastIp = (sockaddr_in *)malloc(sizeof(struct sockaddr_in));

   //Calculation of broadcast IP address: (network address) | ~(network mask)
   //Calculation of network address: (local ip) & (network mask)
   broadcastIp->sin_addr.s_addr = (localIp->sin_addr.s_addr &
                                     subnetMask->sin_addr.s_addr)
                                  | (~subnetMask->sin_addr.s_addr);

   fprintf(stderr, "Broadcast IP: %s\n", inet_ntoa(broadcastIp->sin_addr));
   return 1;
}
