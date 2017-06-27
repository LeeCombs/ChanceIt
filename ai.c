#include <stdio.h>
#include "probabilities.h" // For getProb(roll)
#include "game.h" // For getTurn(), getScores()
#include <unistd.h> // For input delay on Linux:    sleep(1) = 1 second

#include <math.h>
#include <stdlib.h>

// Class vars
int aiGameScore, aiTurnScore, aiFirstRoll, opponentGameScore, scoreDifference;
float rollProbability;

/*
DESC:       -
PRE:        -
POST:       -
PARAMS:     -
RETURN:     -
CLEANUP:    -
*/


/*
DESC:       - Determines current game variables for AI use
PRE:        - A game is underway
POST:       - aiGameScore, aiTurnScore, aiFirstRoll, opponentGameScore, scoreDifference, rollProbability are set
PARAMS:     - N/A
RETURN:     - N/A
CLEANUP:    - N/A
*/
void turnSetup(){
    // Temporary vars to extract relevant info
    int p1GS, p2GS; // Player game scores
    int p1TS, p2TS; // Player turn scores
    int playerNum; // Which player the AI is

    // Get which player the ai is acting as
    if (get_networking()) playerNum = 1;
    else playerNum = get_playerTurn();

    // Reset the game and turn scores, then get them from game
    p1GS = p2GS = p1TS = p2TS = 0;
    getGameScores_Game(&p1GS, &p2GS);
    getTurnScores_Game(&p1TS, &p2TS);

    // Setup the probabilty of rolling the first roll
    aiFirstRoll = get_playerFirstRoll(playerNum);
    // rollProbability = getProb(aiFirstRoll);

    // Determine own scores base on playerNum
    if (playerNum == 1) {
        aiGameScore = p1GS;
        aiTurnScore = p1TS;
        opponentGameScore = p2GS;
    }
    else {
        aiGameScore = p2GS;
        aiTurnScore = p2TS;
        opponentGameScore = p1GS;
    }

    // Set up the difference between this AI and opponent
    // Negative if losing, positive if ahead
    scoreDifference = aiGameScore + aiTurnScore - opponentGameScore;
}

/*** AI Routines ***/

/*
DESC:       - Ben's AI Routine
PRE:        - A game is underway
POST:       - N/A
PARAMS:     - N/A
RETURN:     - 'y' for roll, 'n' for end turn
CLEANUP:    - N/A
*/
char routine_Ben(){
    // Get current scores and playerNum
    turnSetup();

	if (get_currentRound() == 20 && get_firstPlayer() != get_playerTurn()) {
        // If the score difference is negative (my points - opponents), then roll
        if (scoreDifference <= 0) return 'y';
        return 'n';
    }
    if(aiFirstRoll == 7){
        if(aiTurnScore < 40){
            return 'y';
        }else{
            return 'n';
        }
    }
    if(aiFirstRoll == 2 || aiFirstRoll == 12){
        if(aiTurnScore < 250){
            return 'y';
        }else{
            return 'n';
        }
    }
    if(aiTurnScore < 76){
		return 'y';
	}else{
		return 'n';
	}
}

/*
DESC:       - Connor's AI Routine
PRE:        - A game is underway
POST:       - N/A
PARAMS:     - N/A
RETURN:     - 'y' for roll, 'n' for end turn
CLEANUP:    - N/A
*/
char routine_Connor(){
    // Get current scores, first roll, and roll probability
    turnSetup();
	//if first roll, roll again
	if(aiTurnScore <= 0) {
		return 'y';
	}
	//if last turn roll till winning no matter the roll
	/*if(last turn && (aiGameScore + aiTurnScore) - opponentGameScore <= 0) {
			return 'y'
	} else {*/

	//if your total score is less than the oppenents score plus 50 roll it out
	if(aiGameScore + aiTurnScore < opponentGameScore + 50) {
		return 'y';
	}else{
		return 'n';
	}
}

/*
DESC:       - Kevin's AI Routine
PRE:        - A game is underway
POST:       - N/A
PARAMS:     - N/A
RETURN:     - 'y' for roll, 'n' for end turn
CLEANUP:    - N/A
*/
char routine_Kevin(){
    // Get current scores, first roll, and roll probability
    turnSetup();
	//if first roll, roll again
	if(aiTurnScore <= 0) {
		return 'y';
	}
	//if last turn roll till winning no matter the roll
	if (get_currentRound() == 20 && get_firstPlayer() != get_playerTurn()) {
        // If the score difference is negative (my points - opponents), then roll
        if (scoreDifference <= 0) return 'y';
        return 'n';
    }

	//if 2 or 12 roll up to 100 points while ahead by at least 50 points
	//roll up to 200 if behind by 100+ points
	if((aiFirstRoll == 2 || aiFirstRoll == 12) && aiTurnScore <= 200) {
		if((aiGameScore + aiTurnScore) - opponentGameScore >= 100) {
			return 'y';
		} else if ((aiGameScore + aiTurnScore) - opponentGameScore <= 50) {
			return 'n';
		} else {
            return 'n';
		}
	}

	//if 7 only roll up to 30 points
	if(aiFirstRoll == 7 && aiTurnScore <= 30) {
		return 'y';
	}

	//if 5,6,8,9 roll up to 50 points unless behind by 75+ points
	if((aiFirstRoll == 5 || aiFirstRoll == 6 || aiFirstRoll == 8 || aiFirstRoll == 9) && aiTurnScore <= 75) {
		if(opponentGameScore >= (aiGameScore + aiTurnScore + 75)) {
			return 'y';
		}
		else if(aiTurnScore >= 50) {
			return 'n';
		} else {
			return 'y';
		}
	}

	//if 3,4,10,11 roll up to 75, unless winning then roll to 50
	if((aiFirstRoll == 3 || aiFirstRoll == 4 || aiFirstRoll == 10 || aiFirstRoll == 11) && aiTurnScore <= 75) {
		if((aiGameScore + aiTurnScore) > opponentGameScore) {
			return 'n';
		}
		return 'y';
	}
    return 'n';
}

/*
DESC:       - Lee's AI Routine
PRE:        - A game is underway
POST:       - N/A
PARAMS:     - N/A
RETURN:     - 'y' for roll, 'n' for end turn
CLEANUP:    - N/A
*/
char routine_Lee(){
    // Get current scores, first roll, and roll probabilty
    turnSetup();

    // If it's the last round, and the AI is the last player, roll until it wins, then stop
    if (get_currentRound() == 20 && get_firstPlayer() != get_playerTurn()) {
        // The AI has already won, rub it in by going for a high score
        if (aiGameScore < opponentGameScore) {
            // If the score difference is negative (my points - opponents), then roll
            return 'n';
        }
        else printf("Already winning %d to %d, time to show off!\n", aiGameScore, opponentGameScore);
    }

    // Smaller words
    int tr, sd, ts;
    tr = get_turnRolls();
    sd = scoreDifference;
    ts = aiTurnScore;

    // Roll based off average rolls per probability
    // If getting close to the average, check for a lead and do a cautious stop

    if (aiFirstRoll == 2 || aiFirstRoll == 12) {
        if (tr > 27 && sd > 200) return 'n';
        if (tr < 37) return 'y';
    }
    if (aiFirstRoll == 3  || aiFirstRoll == 11) {
        if (tr > 12 && sd > 150) return 'n';
        if (tr < 18) return 'y';
    }
    if (aiFirstRoll == 4 || aiFirstRoll == 10) {
        if (tr > 9 && sd > 150) return 'n';
        if (tr < 11) return 'y';
    }

    // On rolls close to 7, play it safer and stop at 70+ points
    if (tr > 70) return 'n';

    if (aiFirstRoll == 5 || aiFirstRoll == 9) {
        if (tr > 7 && sd > 125) return 'n';
        if (tr < 9) return 'y';
    }
    if (aiFirstRoll == 6 || aiFirstRoll == 8) {
        if (tr > 4 && sd > 125) return 'n';
        if (tr < 7) return 'y';
    }
    if (aiFirstRoll == 7) {
        if (tr > 3 && sd > 100) return 'n';
        if (tr < 6) return 'y';
    }

    return 'n';

}

// Public methods

/*
DESC:       - Pauses for a second, then gets an AI's response and returns it
PRE:        - A game is underway
POST:       - N/A
PARAMS:     - _type: Which AI to execute
RETURN:     - 'y' for roll, 'n' for end turn
CLEANUP:    - N/A
*/
char getInput_AI(char _type){
    // Delay input by a second
    sleep(1);

    _type = tolower(_type);
    // Return the input char from the appropiate AI
    switch(_type){
        case 'b':
            return(routine_Ben());
            break;
        case 'c':
            return(routine_Connor());
            break;
        case 'k':
            return(routine_Kevin());
            break;
        case 'l':
            return(routine_Lee());
            break;
        default:
            printf("Error, invalid computer player given: %c. Expected B, C, K, or L", _type);
            exit(-1);
    }
}
