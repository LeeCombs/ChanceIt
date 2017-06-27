#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include "network.h"

static int socket_desc;
static char buffer[1024];
static int bufPtr = 0;
static int bufSize = 0;

/**
* Connect to the Server
*/
/*
DESC:       -
PRE:        -
POST:       -
PARAMS:     -
RETURN:     -
CLEANUP:    -
*/
int connect_Network(char *IP, int port) {
    struct sockaddr_in server;
	//Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);

    if (socket_desc == -1) {
        puts("Could not create socket");
		return 0;
    }

    server.sin_addr.s_addr = inet_addr(IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    //Connect to remote server
    if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0) {
        puts("connect error");
		return 0;
    }
	return 1;
}

/**
* Read from server
*/

//return the number of characters in the line - \0
//fill lineBuffer with text from server
/*
DESC:       -
PRE:        -
POST:       -
PARAMS:     -
RETURN:     -
CLEANUP:    -
*/
int readLine(char *lineBuffer) {
    int i = 0;
    while (bufPtr < bufSize) { // while buffer contains data
       //printf("2\n");
      if (buffer[bufPtr] == '\n') {
          //printf("3\n");
         lineBuffer[i] = '\0';  // replace the newline with a NULL
         bufPtr++; // skip over the newline
         return i; // return a count of the # of bytes copied into lineBuffer
      }
      lineBuffer[i++] = buffer[bufPtr++];
    }
    //printf("4\n");
    bufSize = recv(socket_desc, buffer, 1024, 0); // read from socket into buffer

    if(bufSize < 1) {
        //printf("5\n");
        //read failed
        return 0;
    }
    //printf("6\n");
    bufPtr = 0; // reset our read index to the beginning of buffer
    return i + readLine(&lineBuffer[i]); // recurse
}

/**
* send info to server
*/
/*
DESC:       -
PRE:        -
POST:       -
PARAMS:     -
RETURN:     -
CLEANUP:    -
*/
int server_send(char *msg) {
    if(send(socket_desc, msg, strlen(msg), 0) < 0) {
        //sending failed
        return 0;
    }
    puts("sending...");
    //sending successful
    return 1;
}

/**
 * Tell the network to roll the dice
 */
/*
DESC:       -
PRE:        -
POST:       -
PARAMS:     -
RETURN:     -
CLEANUP:    -
*/
void roll_Network(){
    server_send("Y\n");
}

/**
 * Tell the network to end the turn
 */
/*
DESC:       -
PRE:        -
POST:       -
PARAMS:     -
RETURN:     -
CLEANUP:    -
*/
void endTurn_Network(){
    server_send("n\n");
}

/**
 * Tell the network to end the game
 */
/*
DESC:       -
PRE:        -
POST:       -
PARAMS:     -
RETURN:     -
CLEANUP:    -
*/
void endGame_Network() {
	server_send("stop\n");
}
