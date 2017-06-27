/*
PRE: seed() has been called, high > low
POST: N/A
RETURNS: an integer between low and high (inclusive)
CLEANUP: N/A
DESC: generate random number between high and low (inclusive)
*/

int randomNum(int low, int high);
/*
PRE: N/A
POST: N/A
RETURNS: N/A
CLEANUP: N/A
DESC: seeds generator, must be called before attemping to get a random number
*/
void seed();
