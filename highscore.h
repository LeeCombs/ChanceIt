#ifndef HIGHSCORE_H_INCLUDED
#define HIGHSCORE_H_INCLUDED

// Display the highscores
void showHighScores();

// Attempt to add an entry to the highscores
void addHighScore(char *name, int score);

int checkHighScore(int score);

#endif // HIGHSCORE_H_INCLUDED
