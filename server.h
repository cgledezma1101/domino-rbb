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
 * Input: None
 * Output: 1 if the broadcast was successfully sent, 0 otherwise
 * Side effects: None
 */
int sendBroadcast();
#endif /* SERVER_H_ */
