#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#include <stdio.h>

//connect to the network
int connect_Network(char *IP, int port);

//read string from server line by line
int readLine(char * lineBuffer);

//send string to server
int server_send(char *msg);

// Tell the network to roll the dice
void roll_Network();

// Tell the network to end the turn
void endTurn_Network();

// Tell the network to end the game
void endGame_Network();

#endif // NETWORK_H_INCLUDED
