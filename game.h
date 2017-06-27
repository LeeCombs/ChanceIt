#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <stdbool.h>

// Start a local of network game session
void startGame(char _type);

// Set the rules of the game **FOR LOCAL ONLY**
void setRules(int _maxRounds, int _numDice, int _numSides);

// Get various in-game stats
void getGameScores_Game(int *score1, int *score2);
void getTurnScores_Game(int *score, int *score2);
int get_playerFirstRoll(int n);
int get_currentRound();
int get_turnRolls();

// Current player rolls
void chanceIt();

//Ends current players turn
void endTurn();

// Return true if there's a game underway, false if not
bool get_gameRunning();
bool get_networking();

// Return which player's turn it is (1 or 2)
int get_playerTurn();

//Start a game over the network
void startNetworkGame(char* line);

// End the current game
void endGame();

#endif // GAME_H_INCLUDED
