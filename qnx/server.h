<<<<<<< HEAD:qnx/server.h
/*
 * server.h
 *
 *  Created on: 25/07/2012
 *      Author: C.G Ledezma
 */

#ifndef SERVER_H_
#define SERVER_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <time.h>

/*
 * Function that executes the commands that start a server that will handle one
 * domino game
 *
 * Input: None
 * Output: None
 * Side effects: None
 */
int runServer();

/*
 * Function that initializes the sockets through which the server will listen
 * to requests
 *
 * Input: servSock -> pointer to the integer that will hold the file descriptor
 *        portno -> Port used for communication
 * Output: 1 if connection was successfully initialized, 0 otherwise
 * Side effects: Variable sockfd contains the file descriptor for the
 *   server's socket
 */
int initializeConnection(int *servSock, int portno);

/*
 * Function that sends a network broadcast indicating to all members of the
 * network the servers IP address
 *
 * Input: acceptSocket -> File descriptor of the socket where the server should listen
 *                         to the requests of the players
 *        sockets -> Array where the sockets to the four connections should be
 *                   saved
 * Output: 1 if the broadcast was successfully sent, 0 otherwise
 * Side effects: The input array will contain four sockets, corresponding to the
 *               communication tunnels to each one of the players
 */
int getPlayers(int acceptSocket, int *sockets);

/*
 * This function sends to each player previously accepted with getPlayers a set
 * of 7 randomly selected domino pieces
 *
 * Input: players -> Opened sockets to each player
 * Output: The id of the player that got the 6|6, or -1 in case of error
 * Side effects: None
 */
int initializeGame(int *players);

/*
 * Function that is in charge of receiving player requests and answering
 * correspondingly for one hand
 *
 * Input: players -> Connections properly established with each one of the players
 *        s -> ID of the player that got the 6|6
 * Output: 4 if the hand ended because it came to a lock, or the ID of the player
 *         that ended the hand, -1 for error
 * Side effects: None
 */
int processPlays(int players[4], int s);

/*
 * Function that deals with the ending of a hand
 *
 * Input: players -> Opened connections with the players
 *        scores -> Array containing the actual score of the game
 *        endType -> The value returned by the last execution of processPlays
 * Output: 0 if the game must continue, 1 if the game has ended, -1 in error
 * Side effects: The scores array is updated with the new scores
 */
int endHand(int players[4], int scores[2], int endType);

/*
 * Function that receives a number between 0 and 9, and returns its char representation
 *
 * Input: n -> Number to be transformed
 * Output: 'n''s char representation
 * Side effects: None
 */
char intToAsc(int n);
#endif /* SERVER_H_ */
=======
/*
 * server.h
 *
 *  Created on: 25/07/2012
 *      Author: C.G Ledezma
 */

#ifndef SERVER_H_
#define SERVER_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <net/if.h>

/*
 * Function that executes the commands that start a server that will handle one
 * domino game
 *
 * Input: None
 * Output: None
 * Side effects: None
 */
int runServer();

/*
 * Function that initializes the sockets through which the server will listen
 * to requests
 *
 * Input: servSock -> pointer to the integer that will hold the file descriptor
 *        portno -> Port used for communication
 * Output: 1 if connection was successfully initialized, 0 otherwise
 * Side effects: Variable sockfd contains the file descriptor for the
 *   server's socket
 */
int initializeConnection(int *servSock, int portno);

/*
 * Function that sends a network broadcast indicating to all members of the
 * network the servers IP address
 *
 * Input: sockfd -> File descriptor of the socket where the server should listen
 *                  to the requests of the players
 *        sockets -> Array where the sockets to the four connections should be
 *                   saved
 * Output: 1 if the broadcast was successfully sent, 0 otherwise
 * Side effects: The input array will contain four sockets, corresponding to the
 *               communication tunnels to each one of the players
 */
int sendBroadcast(int sockfd, int *sockets);
#endif /* SERVER_H_ */
>>>>>>> Added initial trial for application graphics.:server.h
