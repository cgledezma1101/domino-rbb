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

   /*
   if(!initializeConnection(&sockfd, portno))
   {
      fprintf(stderr, "Error initializing connection. Aborting server\n");
      exit(0);
   }

   if(!getPlayers(sockfd, playerSocket))
   {
      fprintf(stderr, "Error finding the players for the game. Aborting server\n");
      exit(0);
   }
   */
   if(!initializeGame(playerSocket))
   {
      fprintf(stderr, "Error initializing players. Aborting server\n");
      exit(0);
   }

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

int getPlayers(int acceptSocket, int *sockets)
{
   char serversIp[INET_ADDRSTRLEN], netmask[INET_ADDRSTRLEN];
   char *message, *ipString, *ipElem;
   struct sockaddr_in *localIp, *subnetMask, *broadcastIp;
   int broadSocket;
   int i, j, k;

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

   //Here, the servers IP address is displayed
   fprintf(stderr, "IP Address: %s\n", serversIp);

   /* This area is not temporarily available, but will be in future releases.
    *
    * Here, the LAN's broadcast IP is calculated so a message can be sent to
    * all Playbooks on the network with this server's IP address. This way users
    * will be able to connect to this server without any knowledge of the underlying
    * communication

   fprintf(stderr, "Netmask: %s\n", netmask);

   broadcastIp = (sockaddr_in *)malloc(sizeof(struct sockaddr_in));

   //Calculation of broadcast IP address: (network address) | ~(network mask)
   //Calculation of network address: (local ip) & (network mask)
   broadcastIp->sin_addr.s_addr = (localIp->sin_addr.s_addr &
                                     subnetMask->sin_addr.s_addr)
                                  | (~subnetMask->sin_addr.s_addr);
   broadcastIp->sin_family = AF_INET;
   broadcastIp->sin_port = htons(10602);

   fprintf(stderr, "Broadcast IP: %s\n", inet_ntoa(broadcastIp->sin_addr));

   //Opening socket to initialize broadcast of servers IP
   broadSocket = socket(AF_INET, SOCK_DGRAM, 0);

   if(broadSocket < 0)
   {
      fprintf(stderr, "Error opening broadcast socket\n");
      exit(1);
   }

   //Now that the socket has been opened, start to build the message to broadcast
   message = (char *)malloc(11 * sizeof(char));
   //Message type, b for broadcast
   message[0] = 'b';
   //Table number, always sending 0. Assuming there won't be other tables in
   //the same LAN. This has to be scalated
   message[1] = '0';

   //Now, to build the server's ip
   ipString = inet_ntoa(localIp->sin_addr);
   ipElem = strtok(ipString, ".");
   for(i = 0; i < 4; ++ i)
   {
      fprintf(stderr, "Building IP element: %s\n", ipElem);
      //Count how many elements there are in this ip
      j = 0;
      char num = ipElem[0];
      while(num != NULL)
      {
         num = ipElem[++ j];
      }

      fprintf(stderr, "Number of elements counted: %d\n", j);
      //Now that 'j' has the number of elements, write them into the message with
      //the corresponding leading zeroes
      for(k = 3; k > 0; -- k)
      {
         if(j > 0)
         {
            //If there are still characters from the ip to write, then write them
            message[1 + i * 3 + k] = ipElem[-- j];
         }else
         {
            //If not, just write a '0'
            message[1 + i * 3 + k] = '0';
         }
      }
      ipElem = strtok(NULL, ".");
   }

   fprintf(stderr, "The message to be sent is: %s\n", message);

   //Connect the broadcast buffer
   int n;
   if((n = connect(broadSocket, (struct sockaddr *)broadcastIp, sizeof(*broadcastIp))) < 0)
   {
      fprintf(stderr, "Error connecting to the broadcast buffer. Error: %d\n", n);
      exit(1);
   }

   //Send the broadcast
   sendto(broadSocket, message, 11 * sizeof(char), 0, (struct sockaddr *)broadcastIp, sizeof(*broadcastIp));

   fprintf(stderr, "Message has been broadcasted\n");
    */

   //Here, the server waits for 4 connections to be established. PLayers will be
   //set in the order in which they establish the connection

   for(i = 0; i < 4; ++ i)
   {
      fprintf(stderr, "Waiting for player %d to connect\n", i);
      sockets[i] = accept(acceptSocket, NULL, NULL);
      fprintf(stderr, "Accepted and saved connection to player %d\n", i);
   }

   //Setting free allocated memory and returning
   /*free(message);
   free(localIp);
   free(subnetMask);
   free(broadcastIp);*/
   return 1;
}

int initializeGame(int *playerSockets)
{
   int numDominoes = 28, i, j, k, randN;
   int dominoes[28][2], assigned[7][2];
   char message[17];
   char intToAsc[] = {'0', '1', '2', '3', '4', '5', '6'};

   //Initialize the array with all the pieces
   k = 0;
   for(i = 0; i < 7; ++ i)
   {
      for(j = i; j < 7; ++ j)
      {
         dominoes[k][0] = i;
         dominoes[k][1] = j;
         ++ k;
         fprintf(stderr, "Building dominoe %d | %d\n", i, j);
      }
   }

   //Initialize the random seed
   srand(time(0));

   //Get the assignments for each player and send them
   message[0] = 'e';
   message[16] = '\0';
   for(i = 0; i < 4; ++ i)
   {
      message[1] = intToAsc[i];
      for(j = 0; j < 7; ++ j)
      {
         randN = rand() % numDominoes;
         //Assign the dominoe to the player
         assigned[j][0] = dominoes[randN][0];
         assigned[j][1] = dominoes[randN][1];

         //Now re-arrange the dominoes array to eliminate the assigned piece
         for(k = randN; k < numDominoes; ++ k)
         {
            dominoes[k][0] = dominoes[k + 1][0];
            dominoes[k][1] = dominoes[k + 1][1];
         }
         -- numDominoes;
         /*fprintf(stderr, "Piece %d | %d, has been assigned to player %d\n",
                 assigned[j][0], assigned[j][1], i);*/
         //fprintf(stderr, "There are %d pieces left to assign\n", numDominoes);
      }

      //fprintf(stderr, "\nAssignment for player %d completed\n", i);
      //Assignment for this player is complete, build message and send it
      fprintf(stderr, "Assignment to player %d: \n", i);
      for(k = 0; k < 7; ++ k)
      {
         message[2 + 2 * k] = intToAsc[assigned[k][0]];
         message[2 + 2 * k + 1] = intToAsc[assigned[k][1]];
         fprintf(stderr, "%d | %d\n", assigned[k][0], assigned[k][1]);
      }

      fprintf(stderr, "Sending message to player %d is: %s\n", i, message);

      write(playerSockets[i], message, 16);
   }
   return 1;
}
