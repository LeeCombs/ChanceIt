#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h> // For tolower()
#include <assert.h>
#include <math.h>
#include <time.h>

#include "highscore.h"
#include "ai.h"
#include "network.h"
#include "probabilities.h"


/*
DESC:       - Display the intro message
PRE:        - N/A
POST:       - N/A
PARAMS:     - N/A
RETURN:     - N/A
CLEANUP:    - N/A
*/
void promptIntro(){
    // Clear the screen the write the prompt
    // system("cls");

    printf("\n");
    printf("Welcome to Chance-it!\n");
    printf("\n");
    printf("Please Enter what you would like to do:\n");
    printf("[G] = Play a game\n");
    printf("[R] = Read the rules\n");
    printf("[Q] = Quit the game\n");
    printf("[H] = See High Scores\n");
    printf("\n");
}


/*
DESC:       - Display the options of starting a game
PRE:        - N/A
POST:       - N/A
PARAMS:     - N/A
RETURN:     - N/A
CLEANUP:    - N/A
*/
void promptGame(){
    // Clear the screen the write the prompt
    // system("cls");

    printf("\n");
    printf("What kind of game would you like to play?\n");
    printf("[1] = You vs Local Opponent\n");
    printf("[2] = You vs Computer\n");
    printf("[3] = You vs Network Opponent\n");
    printf("[4] = Computer vs Computer\n");
    printf("[5] = Computer vs Network Opponent\n");
    printf("[6] = Back to Main Menu\n");
    printf("\n");
}

/*
DESC:       - Display the rules of the game
PRE:        - N/A
POST:       - N/A
PARAMS:     - N/A
RETURN:     - N/A
CLEANUP:    - N/A
*/
void showRules(){
    // Clear the screen the write the prompt
    // system("cls");

    printf("\n");
    printf("The Rules!\n");
    printf("--------------------------------------------------------------------------\n");
    printf("The object of the game is to be the player with\n");
    printf("the highest score after completing 20 rounds.\n");
    printf("\n");
    printf("During a round, each player takes a turn.\n");
    printf("A player's total score is the sum of their turn scores.\n");
    printf("At the begging of the game, each player rolls a die; High goes first.\n");
    printf("\n");
    printf("At the beginning of their turn, a player rolls two six-sided dice.\n");
    printf("The sum is their first roll\n");
    printf("\n");
    printf("After their first roll, a player must decided to stop or continue.\n");
    printf("\n");
    printf("If a player decides to stop, then their turn ends and their score\n");
    printf("for that turn is the sum of all rolls during that turn.\n");
    printf("If they decide to continue, they roll the dice again,\n");
    printf("\n");
    printf("If in re-rolling the dice, a player re-rolls their first roll\n");
    printf("(any combination that results in the same sum)\n");
    printf("their turn ends and their score for that turn is zero.\n");
    printf("\n");
    printf("Continue for 20 rounds. The winner is the player with the highest score.\n");
    printf("--------------------------------------------------------------------------\n");
}

/*
DESC:       - Display a goodbye message and exit the program
PRE:        - N/A
POST:       - N/A
PARAMS:     - N/A
RETURN:     - N/A
CLEANUP:    - N/A
*/
void quitGame(){
    printf("Thanks for playing!\n");
    exit(0);
}

/*
DESC:       - Figure out which type of player to get input from
PRE:        - A Game is underway
POST:       - N/A
PARAMS:     - _type: The type of player to get input from
RETURN:     - the char from the player
CLEANUP:    - N/A
*/
char getInputFrom(char _type) {
    switch(_type){
        case 'p':
            // Human Player
            return getChar_Player("ynpes");
        case 'n':
            // Network Player
            // return getInput_Network();
        case 'b':
        case 'c':
        case 'k':
        case 'l':
            // Computer Player
            return getInput_AI(_type);
        default:
            printf("Invalid input given %c", _type);
    }
    return '\0';
}


/*
DESC:       - Main function of the program.
PRE:        - N/A
POST:       - See everything.
PARAMS:     - N/A
RETURN:     - N/A
CLEANUP:    - N/A
*/
int main(){
    // Seed the random number generator on load
    time_t t;
    srand((unsigned) time(&t));

    // The program loop
    bool programLoop = true;
    while (programLoop) {
        // Set up loop vars, start with looping mainMenuLoop first
        bool mainMenuLoop = true, gameSelectLoop = false, gameLoop = false;

        // For keeping track of what each player type is (human, ai, net)
        char playerOne, playerTwo;

        // On entering the program for the first time, display the intro
        promptIntro();

        // Start with the intro loop
        // The user may select a game type, show rules, highscores, or quit the game
        // Remain in the intro loop until the user selects play a game or exits
        while (mainMenuLoop) {
            // Wait until the user gives a valid input
            char* validInput_Menu = "grhq"; // Game, Rules, Highscore, Quit
            char inputChar = getChar_Player(validInput_Menu);

            // Got valid input, figure out what to do with it
            switch(inputChar) {
                case 'g': // Play a game
                    // Show the game selection menu and move into gameSelect loop
                    promptGame();
                    mainMenuLoop = false;
                    gameSelectLoop = true;
                    break;
                case 'r': // Show the rules
                    showRules();
                    printf("\nEnter anything to continue.\n");

                    // Wait for user input, then display the intro again
                    getFirstChar();
                    promptIntro();
                    break;
                case 'q': // Quit the game
                    quitGame();
                    break;
                case 'h': // See the high scores
                    showHighScores();
                    printf("\nEnter anything to continue.\n");

                    // Wait for user input, then display the intro again
                    getFirstChar();
                    promptIntro();
                    break;
                default: //Invalid input
                    printf("Error, invalid inputChar: %c", inputChar);
            }
        }

        // gameSelectLoop
        while (gameSelectLoop) {
            printf("Game select loop\n");

            // Set valid inputs for the menu and the computer selections
            char* validInput_Menu = "123456";
            char* validInput_Comps = "bckl"; // Ben, Connor, Kevin, Lee

            // Wait until the user gives a valid input
            char inputChar = getChar_Player(validInput_Menu);

            // Got valid input, figure out what to do with it
            switch(inputChar) {
                case '1': // You vs Local Player
                    printf("You vs Local Player \n");
                    printf("\n");

                    // Set players
                    playerOne = 'p';
                    playerTwo = 'p';

                    // Exit the select loop and enter the game loop
                    gameSelectLoop = false;
                    gameLoop = true;
                    puts("Start game");
                    startGame('l');
                    break;
                case '2': // You vs Comp
                    printf("You vs Comp \n");
                    printf("\n");
                    printf("Pick a computer opponent \n");
                    printf("[B] - Ben\n");
                    printf("[C] - Connor\n");
                    printf("[K] - Kevin\n");
                    printf("[L] - Lee\n");
                    printf("\n");

                    // Set players
                    playerOne = 'p';
                    playerTwo = getChar_Player(validInput_Comps);

                    // Exit the select loop and enter the game loop
                    gameSelectLoop = false;
                    gameLoop = true;
                    puts("Start game");
                    startGame('l');
                    break;
                case '3': // You vs Net Opponent
                    printf("You vs Net Opponent \n");
                    printf("\n");

                    // TODO - Add input network credentials here?
                    puts("Enter your name!");
                    char netName[100];
                    fgets(netName, 100, stdin);
                    // Set players
                    playerOne = 'p';
                    playerTwo = 'n';

                    // Exit the select loop and enter the game loop
                    gameSelectLoop = false;
                    gameLoop = true;

                    // Start the networked game
                    puts("Start game");
                    startNetworkGame(netName);
                    break;
                case '4': // Comp vs Comp
                    printf("Comp vs Comp \n");
                    printf("\n");
                    printf("Pick the first computer opponent type\n");
                    printf("[B] - Ben\n");
                    printf("[C] - Connor\n");
                    printf("[K] - Kevin\n");
                    printf("[L] - Lee\n");
                    printf("\n");
                    playerOne = getChar_Player(validInput_Comps);

                    printf("Pick the second computer opponent type\n");
                    printf("[B] - Ben\n");
                    printf("[C] - Connor\n");
                    printf("[K] - Kevin\n");
                    printf("[L] - Lee\n");
                    printf("\n");
                    playerTwo = getChar_Player(validInput_Comps);

                    // Exit the select loop and enter the game loop
                    gameSelectLoop = false;
                    gameLoop = true;
                    puts("Start game");
                    startGame('l');
                    break;
                case '5': // Comp vs Net
                    printf("Comp vs Net\n");
                    printf("\n");
                    printf("Pick a computer to send against opponent \n");
                    printf("[B] - Ben\n");
                    printf("[C] - Connor\n");
                    printf("[K] - Kevin\n");
                    printf("[L] - Lee\n");
                    printf("\n");

                    // Set players
                    playerOne = getChar_Player(validInput_Comps);
                    playerTwo = 'n';

                    // TODO - Add input network credentials here?

                    // Exit the select loop and enter the game loop
                    gameSelectLoop = false;
                    gameLoop = true;

                    // Start the networked game using a default name
                    puts("Start game");
                    startNetworkGame("Team Redundant Team\n");
                    break;
                case '6': // Back to main menu
                    printf("Back to main menu \n");
                    printf("\n");

                    // Exit the select loop and enter the menu loop
                    gameSelectLoop = false;
                    mainMenuLoop = true;
                    break;
                default: //Invalid input
                    printf("Error, invalid input for game selection: %c \n", inputChar);
                    exit(0);
            }
        }

        // gameLoop
        while (gameLoop) {
            setupProb(2, 6); // TODO - Move this to game? Get rid of it?

            // Enter yet another loop while the game is running
            while (get_gameRunning()){
                char inp;

                // Check if the game is networking, and only supply player one inputs
                if (get_networking()) {
                    inp = getInputFrom(playerOne);
                    if (inp == 'y') chanceIt();
                    else if (inp == 'n') endTurn();
                }
                else {
                    // Figure out which player should be giving input
                    // Then get it and tell the game what to do
                    get_playerTurn() == 1 ? (inp = getInputFrom(playerOne)) : (inp = getInputFrom(playerTwo));

                    if (inp == 'y') chanceIt();
                    else if (inp == 'n') endTurn();
                    else if (inp == 'p') {
                        // Display probability of rolling current player's first roll
                        getProb(get_playerFirstRoll(get_playerTurn()));
                    }
					else if (inp == 'e' || inp == 's') {
						endGame();
						gameLoop = false;
						mainMenuLoop = false;
						continue;
					}
                }
            }

            // Check for high scores
            // TODO - Sometimes this tries to set a highscore that isn't a high score
            // This is during checkHighScores(), find out why
            int p1gs, p2gs;
			p1gs = 0, p2gs = 0;
            getGameScores_Game(&p1gs, &p2gs);

            // Show highscores flag
            int g = 0;
            if (checkHighScore(p1gs) == 1){
                g = 1;
                printf("Player 1 got a highscore!\n");
                printf("Enter your name!: ");
                char name[100];
                fgets(name, 100, stdin);
                int len;
                len = strlen(name);
                if (name[len-1] == '\n') name[len-1] = 0;
                addHighScore(name, p1gs);
            }
            if (checkHighScore(p2gs) == 1){
                g = 1;
                printf("Player 2 got a highscore!\n");
                printf("Enter your name!: ");
                char name[100];
                fgets(name, 100, stdin);
                int len;
                len = strlen(name);
                if (name[len-1] == '\n') name[len-1] = 0;
                addHighScore(name, p2gs);
            }
            if (g == 1) {
                printf("\nHere are the new scores!\n");
                showHighScores();
            }

            // Return to the main menu
            printf("\nThanks for playing, returning to main menu!\n");

            gameLoop = false;
            mainMenuLoop = true;
        }
    }

    printf("Out of program loop");
    return 0;
}
