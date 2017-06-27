#ifndef USERINTERFACE_H_INCLUDED
#define USERINTERFACE_H_INCLUDED

// Sign in a player
void signIn_UI(char* name);

// Tell the game to roll
void roll_UI();

// Tell the game to end the current turn
void endTurn_UI();

// Get the probabilty of rolling roll
int getProb_UI(int roll);

// Get both player's scores
void getGameScores();

#endif // USERINTERFACE_H_INCLUDED
