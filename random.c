#include <time.h>
#include "random.h"
//generate random number between high and low inclusive
/*
DESC:       -
PRE:        -
POST:       -
PARAMS:     -
RETURN:     -
CLEANUP:    -
*/
int randomNum(int low, int high){
	int range = high - low + 1;
	int r = (rand() % range) + low;
	return r;
}
// seed random number generator
/*
DESC:       -
PRE:        -
POST:       -
PARAMS:     -
RETURN:     -
CLEANUP:    -
*/
void seed(){
	srand(time(NULL));
}
