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
   int n, i, starter, endType, gameStat;
   //File descriptor for the sockets used
   int sockfd = 0, playerSocket[4];
   int scores[2] = {0, 0};

   if(!initializeConnection(&sockfd, 10602))
   {
      fprintf(stderr, "Error initializing connection. Aborting server\n");
      exit(0);
   }

   if(!getPlayers(sockfd, playerSocket))
   {
      fprintf(stderr, "Error finding the players for the game. Aborting server\n");
      exit(0);
   }


   while(true)
   {
      if((starter = initializeGame(playerSocket)) == -1)
      {
         fprintf(stderr, "Error initializing players. Aborting server\n");
         exit(0);
      }

      if((endType = processPlays(playerSocket, starter)) == -1)
      {
         fprintf(stderr, "Error processing plays. Aborting server\n");
         exit(0);
      }

      gameStat = endHand(playerSocket, scores, endType);

      if(gameStat == 1)
      {
         fprintf(stderr, "The game has ended.\n");
         break;
      }
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
   int numDominoes = 28, i, j, k, randN, ss;
   int dominoes[28][2], assigned[7][2];
   char message[17];

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
      message[1] = intToAsc(i);
      for(j = 0; j < 7; ++ j)
      {
         randN = rand() % numDominoes;
         //Assign the domino to the player
         assigned[j][0] = dominoes[randN][0];
         assigned[j][1] = dominoes[randN][1];

         //This player got the double six
         if((assigned[j][0] == 6) && (assigned[j][1] == 6))
            ss = i;

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
         message[2 + 2 * k] = intToAsc(assigned[k][0]);
         message[2 + 2 * k + 1] = intToAsc(assigned[k][1]);
         fprintf(stderr, "%d | %d\n", assigned[k][0], assigned[k][1]);
      }

      fprintf(stderr, "Sending message to player %d is: %s\n", i, message);

      write(playerSockets[i], message, 16);
   }

   return ss;
}

int processPlays(int players[4], int s)
{
   int piecesLeft[4] = {7, 7, 7, 7};
   char type;
   char *ms, *mr;
   int actual, passStart = -1, i;

   //The first thing that must be done is send a message to the player,
   // that got 6|6, indicating it is his turn to play.
   ms = (char *)malloc(sizeof(char) * 1);
   ms[0] = 't';
   write(players[s], ms, 1);
   free(ms);
   actual = s;

   //Now, start taking the turns
   while(true)
   {
      //Listen on the corresponding players socket
      //Get the type of the message sent and build an appropriate response
      read(players[actual], &type, 1);
      switch(type)
      {
         case 'n':
            fprintf(stderr, "Player %d has called a pass\n", actual);
            //First, verify that the game has not been locked
            if(actual == passStart)
            {
               fprintf(stderr, "The game has been locked\n");
               return 4;
            }

            if(passStart == -1)
               passStart = actual;

            //Send all a message indicating there has been a pass
            ms = (char *)malloc(sizeof(char) * 2);
            ms[0] = 'p';
            ms[1] = intToAsc(actual);
            for(i = 0; i < 4; ++ i)
            {
               if(i == actual)
                  continue;
               write(players[i], ms, 2);
            }

            fprintf(stderr, "All have been notified, giving turn to %d\n", (actual + 1) % 4);
            ms[0] = 't';
            //Give the turn to the next player
            write(players[(++ actual) % 4], ms, 1);

            free(ms);
            break;

         case 'j':
            fprintf(stderr, "Player %d has made a move\n", actual);
            //Read the rest of the move
            mr = (char *) malloc(sizeof(char) * 3);
            read(players[actual], mr, 3);

            fprintf(stderr, "Move: %c | %c .. %c\n", mr[0], mr[1], mr[2]);
            //Notify everyone which move has been played
            ms = (char *)malloc(sizeof(char) * 5);
            ms[0] = 'a';
            ms[1] = intToAsc(actual);
            ms[2] = mr[0];
            ms[3] = mr[1];
            ms[4] = mr[2];

            for(i = 0; i < 4; ++ i)
            {
               if(i == actual)
                  continue;
               write(players[i], ms, 5);
            }

            //Now, discount the number of pieces the player has, and check if
            //the hand has ended
            if((-- piecesLeft[actual]) == 0)
            {
               fprintf(stderr, "Player %d is out of pieces. Hand ended normally\n", actual);
               return actual;
            }

            //If hand didn't end, update the pass checker and give turn to the next player
            passStart = -1;
            free(ms);
            ms = (char *)malloc(sizeof(char) * 1);
            ms[0] = 't';

            write(players[(++ actual) % 4], ms, 1);
            free(ms);
            free(mr);
            break;

         default:
            fprintf(stderr, "A malformed message has been received: %c\n", type);
            return -1;
      }
   }

   return 0;
}

int endHand(int players[4], int scores[2], int endType)
{
   int i, j, k, tScore0, tScore1, msLength = 11;
   int partialScore[2] = {0, 0}, pNumPieces[4] = {0, 0, 0, 0};
   char *ms, *mr;
   char num[2] = {'0', '\0'}, pPieces[4][14];

   //First, send all players the message that the hand has ended
   ms = (char *)malloc(sizeof(char) * 1);
   ms[0] = 'm';
   for(i = 0; i < 4; ++ i)
   {
      //Ommit the player that ended the hand
      if(i == endType)
         continue;
      fprintf(stderr, "Sending player %d the end hand message.\n", i);
      write(players[i], ms, 1);
   }

   //Now, receive from each player their scores
   for(i = 0; i < 4; ++ i)
   {
      fprintf(stderr, "Analizing the hand of player %d\n", i);
      //Omit the player that ended the hand, for he will send nothing
      if(i == endType)
         continue;

      mr = (char *)malloc(sizeof(char) * 3);

      mr[3] = '\0'; //This is necessary for 'atoi' to work fine
      read(players[i], mr, 2);

      if(mr[0] != 'z')
      {
         fprintf(stderr, "Received the wrong message: %c.\n", mr[0]);
         return -1;
      }

      pNumPieces[i] = atoi(&mr[1]);
      msLength += 2 * pNumPieces[i];

      free(mr);
      mr = (char *)malloc(sizeof(char) * (2 * pNumPieces[i]));
      read(players[i], mr, (2 * pNumPieces[i]));

      for(j = 0; j < 2 * pNumPieces[i]; ++ j)
      {
         num[0] = mr[j];
         pPieces[i][j] = mr[j];
         fprintf(stderr, "Adding to team %d the amount %c\n", i % 2, num[0]);
         partialScore[i % 2] += atoi(num);
      }

      free(mr);
   }

   //Now decide who wins and update the real scores
   if(endType == 4)
   {
      //The hand was a block, calculate who wins and add up the score
      if(partialScore[0] > partialScore[1])
      {
         fprintf(stderr, "Team 1 wins the block.\n");
         scores[1] += partialScore[0];
      }else if(partialScore[1] > partialScore[0])
      {
         fprintf(stderr, "Team 2 wins the block.\n");
         scores[0] += partialScore[1];
      }else
      {
         fprintf(stderr, "Score was even, no one wins the block.\n");
      }
   }else
   {
      if((endType == 0) || (endType == 2))
      {
         scores[0] += partialScore[1];
      }else if((endType == 1) || (endType == 3))
      {
         scores[1] += partialScore[0];
      }else
      {
         fprintf(stderr, "Received an invalid endType: %d", endType);
         return -1;
      }
   }

   //Now, build the message to be sent to everyone
   ms = (char *)malloc(sizeof(char) * msLength);
   ms[0] = 's';

   //Build both scores
   tScore0 = scores[0];
   tScore1 = scores[1];
   for(i = 3; i > 0; ++ i)
   {
      ms[i] = intToAsc(tScore0 % 10);
      ms[3 + i] = intToAsc(tScore1 % 10);
      tScore0 /= 10;
      tScore1 /= 10;
   }

   //Add all hands to the message
   i = 7;
   for(j = 0; j < 4; ++ j)
   {
      ms[i] = intToAsc(pNumPieces[j]);
      ++ i;
      for(k = 0; k < 2 * pNumPieces[j]; ++ k)
      {
         ms[i] = intToAsc(pPieces[j][k]);
         ++ i;
      }
   }

   fprintf(stderr, "The message that will be sent is: %s", ms);

   //Send everyone the message
   for (i = 0; i < 4; ++ i)
   {
      fprintf(stderr, "End of hand message being sent to player %d.\n", i);
      write(players[i], ms, msLength);
   }

   free(ms);
   //Now, return depending to the games state
   if((scores[0] >= 100) || (scores[1] >= 100))
   {
      //Game has ended
      fprintf(stderr, "The game has ended, final scores have been sent.\n");
      return 1;
   }

   return 0;
}

char intToAsc(int n)
{
   char arr[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

   return arr[n];
}
