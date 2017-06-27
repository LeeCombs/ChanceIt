#include "random.h"
#include "dice.h"
// roll dice
/*
DESC:       -
PRE:        -
POST:       -
PARAMS:     -
RETURN:     -
CLEANUP:    -
*/
unsigned roll(unsigned numDice, unsigned numSides){
	int sum = 0;
	int i = 0;
	// add rolls to sum
	for (i = 0; i < numDice; i++){
		sum += randomNum(1,numSides);
	}
	return sum;
}
