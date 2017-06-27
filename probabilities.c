#include <math.h>

// Class vars
int _numDice;
int _numSides;

/**
 * Set up the values to use for determining probability
 * TODO - Think about instead just using getProb(roll, numDice, numSides)?
 */
/*
DESC:       -
PRE:        -
POST:       -
PARAMS:     -
RETURN:     -
CLEANUP:    -
*/
void setupProb(int numDice, int numSides) {
    _numDice = numDice;
    _numSides = numSides;
}

/**
 * Get the probability of rolling a given value
 *
 * Roll the number we're interested in getting the probabilty of
 */
/*
DESC:       -
PRE:        -
POST:       -
PARAMS:     -
RETURN:     -
CLEANUP:    -
*/
float getProb(int roll) {
    char* prob;
    double ret;

    // TODO - Don't hardcode these values, generate value based on setup values
    if (roll == 2 || roll == 12) prob = "1/36 - 2.78%";
    else if (roll == 3 || roll == 11) prob = "2/36 - 5.56%";
    else if (roll == 4 || roll == 10) prob = "3/36 - 8.33%";
    else if (roll == 5 || roll == 9) prob = "4/36 - 11.11%";
    else if (roll == 6 || roll == 8) prob = "5/36 - 13.89%";
    else if (roll == 7) prob = "6/36 - 16.67%";

    if (roll == 2 || roll == 12) ret = 0.0278;
    else if (roll == 3 || roll == 11) ret = 0.0556;
    else if (roll == 4 || roll == 10) ret = 0.0833;
    else if (roll == 5 || roll == 9) ret = 0.1111;
    else if (roll == 6 || roll == 8) ret = 0.1389;
    else if (roll == 7) ret = 0.1667;

    printf("Probabilty of rolling %d: %s\n", roll, prob);
    return ret;
}
