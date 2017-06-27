#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "dice.h"
#include "network.h"

// Game Rules
int currentRound, maxRounds;
int numDice, numSides;

// In-game vars
int p1TurnScore, p2TurnScore;
int p1GameScore, p2GameScore; // Sum of turn scores
int p1FirstRoll, p2FirstRoll;
int turnRolls; // Current number of rolls in a turn
int playerTurn;
int firstPlayer;// The first player to take a turn
bool gameRunning, networking;
char gameType;

/*
DESC:       -
PRE:        -
POST:       -
PARAMS:     -
RETURN:     -
CLEANUP:    -
*/
void endGame(){
    gameRunning = false;
}

/*
DESC:       -
PRE:        -
POST:       -
PARAMS:     -
RETURN:     -
CLEANUP:    -
*/
void parseLine(char* line){
    //find data stored within the networks output and assign to local variables
    if(strstr(line, "Your roll was") != NULL){
        if(strstr(line,"You go first") != NULL){
            firstPlayer = 1;
        }else{
            firstPlayer = 2;
        }
    }
    if(strstr(line, "Roll#:") != NULL){
        sscanf(line, "Roll#: %d", &turnRolls);
		if(turnRolls == 1){
			p1FirstRoll = -1;
		}
    }
    if(strstr(line, "Turn#:") != NULL){
        sscanf(line, "Turn#: %d", &currentRound);
    }
    if(strstr(line, "Running Turn Score:") != NULL){
        sscanf(line, "Running Turn Score: %d", &p1TurnScore);
    }
    if(strstr(line, "Turn Starting Score:") != NULL){
        // 0-0 you then other player
        sscanf(line, "Turn Starting Score: %d-%d", &p1GameScore, &p2GameScore);
    }
    if(strstr(line,"You Rolled:") != NULL && p1FirstRoll == -1){
        int roll1, roll2;
        sscanf(line, "You Rolled: [%d,%d]", &roll1, &roll2);
        p1FirstRoll = roll1 + roll2;
    }
}

/*
DESC:       -
PRE:        -
POST:       -
PARAMS:     -
RETURN:     -
CLEANUP:    -
*/
void startNetworkTurn(){
    char line[100];

    //read lines until a wait condition wait condition ie times when you need to respond or the game has ended
    while(strstr(line, "chance-it? [Y/n]") == NULL  && strstr(line, "You Win") == NULL && strstr(line, "You Lose") == NULL){
		//printf("in loop\n");
        //get line
        int bytes = readLine(line);
		//printf("%d\n", bytes);
        //regurgitate network output
        printf("%s\n", line);
        //grab variable data from network output
        parseLine(line);
    }
    //check if the game has ended
    if(strstr(line, "You Win") != NULL || strstr(line, "You Lose") != NULL){
		p1GameScore += p1TurnScore;
		readLine(line);
		printf("%s\n", line);
        endGame();
        networking = false;
        gameRunning = false;
    }
}

/*
DESC:       - Starts a new local or network game session
PRE:        -
POST:       -
PARAMS:     -
RETURN:     -
CLEANUP:    - N/A
*/
void startGame(char _type){
    // Re-initialize all game variables to start a game fresh
    p1TurnScore = 0, p2TurnScore = 0;
    p1GameScore = 0, p2GameScore = 0;
    p1FirstRoll = 0, p2FirstRoll = 0;
    gameRunning = true;

    // Set the rules
    // TODO - Move this to types
    currentRound = 1, maxRounds = 20;
    numDice = 2, numSides = 6;

    _type = tolower(_type);
    gameType = _type;
    if (_type == 'l') {
        // Start a local game allowing varying rules
    }
    else if (_type == 'n') {
		//printf("network game starting\n");
        startNetworkTurn();
        return;
    }
    else {
        printf("Invalid game type given: %c", _type);
        return;
    }

	int p1Roll = roll(1, numSides);
	int p2Roll = roll(1, numSides);

	//find out who goes first and make their first roll
	while(p1Roll == p2Roll){
		p1Roll = roll(1, numSides);
		p2Roll = roll(1, numSides);
	}

	if(p1Roll > p2Roll){
		firstPlayer = 1;
		playerTurn = 1;
		p1FirstRoll = roll(numDice,numSides);
		p1TurnScore += p1FirstRoll;
		printf("Player1's roll was %d. Player2's roll was %d. Player%d goes first.\n", p1Roll, p2Roll, playerTurn);
		printf("Turn Starting Score: %d-%d\n", p1GameScore, p2GameScore);
		printf("Turn#: %d\n", currentRound);
		printf("You Rolled: %d\n" , p1FirstRoll);
		printf("Running Turn Score: %d\n", p1TurnScore);
		printf("---\n");
		printf("chance-it? [Y/n]");
	}else{
		firstPlayer = 2;
		playerTurn = 2;
		p2FirstRoll = roll(numDice,numSides);
		p2TurnScore += p2FirstRoll;
		printf("Player1's roll was %d. Player2's roll was %d. Player%d goes first.\n", p1Roll, p2Roll, playerTurn);
		printf("Turn Starting Score: %d-%d\n", p1GameScore, p2GameScore);
		printf("Turn#: %d\n", currentRound);
		printf("You Rolled: %d\n" , p2FirstRoll);
		printf("Running Turn Score: %d\n", p2TurnScore);
		printf("---\n");
		printf("chance-it? [Y/n]");
	}
}

/*
DESC:       - Set the game vars for a local game
PRE:        - ??
POST:       - maxRounds, numDice, numSides are set
PARAMS:     - _maxRound: Maximum number of rounds
            - _numDice: Number of dice to roll
            - _numSides: Number of sides the dice have
RETURN:     - N/A
CLEANUP:    - N/A
*/
void setRules(int _maxRounds, int _numDice, int _numSides){
    // Check for validity
    if (_maxRounds <= 0 || _numDice <= 0 || _numSides <= 1) {
        printf("Invalid rules given: Max: %d(1+), Dice: %d(1+), Sides: %d(2+)", _maxRounds, _numDice, _numSides);
    }

    // Set vars
    maxRounds = _maxRounds;
    numDice = _numDice;
    numSides = _numSides;
}

/*
DESC:       - Get the game scores of the current game
PRE:        - A Game is underway and AI has score vars initialized
POST:       - score1/2 are set
PARAMS:     - *score1/2: The scores to set (By reference) (&p1score, &p2score)
RETURN:     - N/A
CLEANUP:    - N/A
*/
void getGameScores_Game(int *score1, int *score2){
    *score1 = p1GameScore;
    *score2 = p2GameScore;
}

/*
DESC:       - Get the turn scores of the current game
PRE:        - A Game is underway and AI has score vars initialized
POST:       - score1/2 are set
PARAMS:     - *score1/2: The scores to set (By reference) (&p1score, &p2score)
RETURN:     - N/A
CLEANUP:    - N/A
*/
void getTurnScores_Game(int *score1, int *score2){
    *score1 = p1TurnScore;
    *score2 = p2TurnScore;
}

/*
DESC:       - End the current game's turn
PRE:        - A Game is underway
POST:       - players Game/Turn scores are updated, the turn is set to the next player
            - The first roll for the next player is rolled and the player prompted
            - If both players have gone, the round counter is incremented
            - If the round reaches the last round, the game ends
            - gameRunning is set to false and the winner is determined
PARAMS:     - N/A
RETURN:     - N/A
CLEANUP:    - N/A
*/
void endTurn(){
    if(gameType == 'n'){
        endTurn_Network();
        startNetworkTurn();
        return;
    }
	// if player one's turn
	if(playerTurn == 1){
		//new game score
		p1GameScore += p1TurnScore;
		p1TurnScore = 0;
		printf("Turn End ==================================\n\n");

		//check if round has turned over
		if(firstPlayer == 2){
			currentRound++;
			//check if game is over
			if(currentRound > maxRounds){
                endGame();
				printf("Game has ended\n");
				gameRunning = false;
				if(p1GameScore > p2GameScore){
					printf("Player1 wins %d-%d\n", p1GameScore, p2GameScore);
				}else if(p1GameScore < p2GameScore){
					printf("Player2 wins %d-%d\n", p1GameScore, p2GameScore);
				}else{
					printf("Game Tied at %d\n", p1GameScore);
				}
				return;
			}
		}

		//roll player2's first dice
		turnRolls = 1;
		playerTurn = 2;
		p2FirstRoll = roll(numDice,numSides);
		p2TurnScore += p2FirstRoll;
		printf("\nTurn Starting Score: %d-%d\n", p1GameScore, p2GameScore);
		printf("Turn#: %d\n", currentRound);
		printf("You Rolled: %d\n" , p2FirstRoll);
		printf("Running Turn Score: %d\n", p2TurnScore);
		printf("---\n");
		printf("chance-it? [Y/n]");
	}else{
	    // Add the turn score and set it back to 0
		p2GameScore += p2TurnScore;
		p2TurnScore = 0;
		printf("Turn End ==================================\n\n");

		// Check if the round's over
		if(firstPlayer == 1){
			currentRound++;

			// Check if the game's over
			if(currentRound > maxRounds){
                endGame();
				printf("Game has ended\n");
				gameRunning = false;
				if(p1GameScore > p2GameScore){
					printf("Player1 wins %d-%d\n", p1GameScore, p2GameScore);
				}else if(p1GameScore < p2GameScore){
					printf("Player2 wins %d-%d\n", p1GameScore, p2GameScore);
				}else{
					printf("Game Tied at %d\n", p1GameScore);
				}
				return;
			}
		}

		// Roll player1's first roll
		turnRolls = 1;
		playerTurn = 1;
		p1FirstRoll = roll(numDice,numSides);
		p1TurnScore += p1FirstRoll;
		printf("\nTurn Starting Score: %d-%d\n", p1GameScore, p2GameScore);
		printf("Turn#: %d\n", currentRound);
		printf("You Rolled: %d\n" , p1FirstRoll);
		printf("Running Turn Score: %d\n", p1TurnScore);
		printf("---\n");
		printf("chance-it? [Y/n]");
	}
}

/*
DESC:       - Roll the dice for the current player
PRE:        - A game is underway
POST:       -
            -
            -
            -
PARAMS:     - N/A
RETURN:     - N/A
CLEANUP:    - N/A
*/
void chanceIt(){
    if(gameType == 'n'){
        roll_Network();
        startNetworkTurn();
        return;
    }
	int currentRoll;
    turnRolls++;
	//if player one's turn
	if(playerTurn == 1){
		//roll dice
		currentRoll = roll(numDice, numSides);
		// if rolled first roll
		if(currentRoll == p1FirstRoll){
			p1TurnScore = 0;
			printf("\nTurn Starting Score: %d-%d\n", p1GameScore, p2GameScore);
			printf("Turn#: %d\n", currentRound);
			printf("You Rolled: %d\n" , currentRoll);
			printf("Running Turn Score: %d\n", p1TurnScore);
			endTurn();
		}else{
			p1TurnScore += currentRoll;
			printf("\nTurn Starting Score: %d-%d\n", p1GameScore, p2GameScore);
			printf("Turn#: %d\n", currentRound);
			printf("You Rolled: %d\n" , currentRoll);
			printf("Running Turn Score: %d\n", p1TurnScore);
			printf("---\n");
			printf("chance-it? [Y/n]\n");
		}
	//player two's turn
	}else{
		currentRoll = roll(numDice, numSides);
		if(currentRoll == p2FirstRoll){
			p2TurnScore = 0;
			printf("\nTurn Starting Score: %d-%d\n", p1GameScore, p2GameScore);
			printf("Turn#: %d\n", currentRound);
			printf("You Rolled: %d\n" , currentRoll);
			printf("Running Turn Score: %d\n", p2TurnScore);
			endTurn();
		}else{
			p2TurnScore += currentRoll;
			printf("\nTurn Starting Score: %d-%d\n", p1GameScore, p2GameScore);
			printf("Turn#: %d\n", currentRound);
			printf("You Rolled: %d\n" , currentRoll);
			printf("Running Turn Score: %d\n", p2TurnScore);
			printf("---\n");
			printf("chance-it? [Y/n]\n");
		}
	}
}

// Return true if there's a game underway, false if not
bool get_gameRunning(){
    return gameRunning;
}

// Return true if there's a network game underway, false if not
bool get_networking(){
    return networking;
}

// Return which player's turn it is (1 or 2)
int get_playerTurn(){
    return playerTurn;
}

// Get the first roll of player n (1 or 2)
int get_playerFirstRoll(int n) {
    if (n == 1) return p1FirstRoll;
    else return p2FirstRoll;
}

// Get the round of the current game
int get_currentRound(){
    return currentRound;
}

// Get the number of rolls of the current turn
int get_turnRolls(){
    return turnRolls;
}

// Get which player went first of the current game
int get_firstPlayer(){
    return firstPlayer;
}

//needs to be added to .h file (only new public function)
/*
DESC:       -
PRE:        -
POST:       -
PARAMS:     -
RETURN:     -
CLEANUP:    -
*/
void startNetworkGame(char* name){
    if(connect_Network("52.38.98.137", 1099) == 1){
        //throw away "is it me youre looking for"
        //char line[100];
        //readLine(line);
		//printf("%s", line);
        //send sign in string
        char signIn[100];
		strcpy(signIn, "HELLO:");
		strcat(signIn, name);
		//strcat(signIn, "\n");
		//printf("%s\n", signIn);
        if(server_send(signIn) == 1){
			//printf("send complete\n");
			//start game proper
			networking = true;
			startGame('n');
		}else{
			puts("failed to send login info");
		}
    }else{
        puts("failed connection");
    }
}

