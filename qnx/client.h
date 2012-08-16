/*
 * client.h
 *
 *  Created on: 25/07/2012
 *      Author: C.G Ledezma
 */

#ifndef CLIENT_H_
#define CLIENT_H_

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
 * Starts the necessary procedures so a client can connect to a server and
 * start playing. Initially it is going to be empty and will only work as
 * a debugging element.
 */
int runClient();

/*
 * Initializes a socket that connects to the server indicated by the parameter
 * passed
 *
 * Input: IP -> A string representing the name or IP of the server
 * Output: An integer containing the file descriptor of an opened socket with
 *         the server, or 0 if there has been an error.
 * Side effects: None
 */
int initializeConnection(char *IP);

/*
 * When the client makes a play, send said information to the server
 *
 * Input: v0 -> Value of one side of the domino played
 *        v1 -> Value of the other side of the domino played
 *        s -> Indication of the side on which the domino was played
 *        serv -> File descriptor to the socket returned by initializeConnection
 * Output: 1 in case of success, 0 otherwise
 * Side effects: None
 */
int sendPlay(int v0, int v1, int s, int serv);

/*
 * Notify the server that the client can't play
 *
 * Input: serv -> File descriptor to the socket returned by initializeConnection
 * Output: 1 in case of success, 0 otherwise
 * Side effects: None
 */
int sendPass(int serv);

/*
 * When the hand is over, through this function the client can give the server
 * the pieces he has left
 *
 * Input: pieces -> Array containing the information of the pieces the player
 *                  has left
 *        numPieces -> Integer with a value half the size of the previous array
 *        serv -> File descriptor to the socket returned by initializeConnection
 * Output: 1 on success, 0 otherwise
 * Side effects: None
 */
int sendPiecesLeft(int *pieces, int numPieces, int serv);
/*
 * Function that receives a number between 0 and 9, and returns its char representation
 *
 * Input: n -> Number to be transformed
 * Output: 'n''s char representation
 * Side effects: None
 */
char intToAsc(int n);
#endif /* CLIENT_H_ */
