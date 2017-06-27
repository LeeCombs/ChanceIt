#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Class Var
char entries[10][3][40]; // 10 sets, of 3 words 20 chars long

/*
DESC:       - Loads the top 10 scores from highscores.txt
PRE:        - N/A
POST:       - entries[][][] is populated
PARAMS:     - N/A
RETURN:     - N/A
CLEANUP:    - N/A
*/
void loadHighScores(){
    printf("Loading high scores\n");
    // Open the highscores text file
    const char filename[] = "highscore.txt";
    FILE *file;
    file = fopen(filename, "r");
    // If something went wrong, exit
    if (file == NULL) perror(filename);

    char *token, line[1000]; // Token (see below), line is for reading the file
    int i = 0, j = 0;

    // Read each line from the file -- i < 10 prevents reading more than 10 lines
    while(fgets(line, sizeof(line), file) != NULL && i < 10) {
        // For every line read, break up the line by commas and put them into the array
        // (i.e. Joe,100,01/01/2016 -> [[Joe][100][01/01/2016]]
        // Questions? http://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm

        j = 0; // Reset the second index every loop
        token = strtok(line, ","); // Get the first token
        // Walk through the other tokens
        while (token != NULL) {
            strcpy(entries[i][j], token); // Copy the string to the entries array
            j++; // Iterate the second index
            token = strtok(NULL, ",\n"); // Set up the next token, separate from the comma
        }
        i++; // Iterate first index
    }

    // Close the file
    fclose(file);
}

/*
DESC:       - Save the current highscores to highscores.txt
PRE:        - entries[][][] exists and has scores
POST:       - highscores.txt is written to
PARAMS:     - N/A
RETURN:     - N/A
CLEANUP:    - N/A
*/
void saveHighScores(){
    printf("Saving high scores\n");
    const char filename[] = "highscore.txt";
    FILE *file;
    file = fopen(filename, "w");
    // If something went wrong, exit
    if (file == NULL) perror(filename);

    int i;
    for (i = 0; i < 10; i++){
        fprintf(file, "%s,%s,%s\n", entries[i][0], entries[i][1], entries[i][2]);
    }
    fclose(file);
}

/*
DESC:       - Display the top 10 high scores
PRE:        - Highscores.txt exists and has scores
POST:       - N/A
PARAMS:     - N/A
RETURN:     - N/A
CLEANUP:    - N/A
*/
void showHighScores(){
    // Load the current highscores
    loadHighScores();

    // Print the top of the table
    printf("\n");
    printf("HIGH SCORES\n");
    printf("+--------------------------------+-------+----------+\n");
    printf("| Name                           | Score | Date     |\n");
    printf("+--------------------------------+-------+----------+\n");

    // Iterate through the highscore entries and display them
    int i;
    for (i = 0; i < 10; i++){
        printf("| %s | %s\t | %s |\n", entries[i][0], entries[i][1], entries[i][2]);
    }

    // Print the bottom of the table
    printf("+--------------------------------+-------+----------+\n");
    printf("\n");
}

/*
DESC:       - Add a score to the highscores list
PRE:        - Highscores.txt exists and has scores
POST:       - name, score, and current date are added to Highscore.txt
            - Lowest score is removed from highscore.txt
PARAMS:     - name: The name to enter
            - score: The score to add
RETURN:     - N/A
CLEANUP:    - N/A
*/
void addHighScore(char *name, int score){
    puts("add high score");
    // Load the current scores and check them against entry[][]
    loadHighScores();

    // Print the score from entry
    int i, pos = -1;
    for (i = 0; i < 10; i++) {
        // Compare the int values of the supplied entry and highscores
        // If the entry is higher than a highscore, set position and exit the loop
        int a = score;
        int b = atoi(entries[i][1]);
        if (a > b) {
            pos = i;
            break;
        }
    }

    // If position was changed...
    if (pos != -1) {
        // Iterate the current scores bottom up, up to pos
        // Swap each entry with the one above it
        // i.e. entries[9] = entries[8]
        for (i = 9; i > pos; i--) {
            int a = score;
            int b = entries[i-1][1];
            strcpy(entries[i][1], entries[i-1][1]);
        }


        // Write the new highscore to entries[]

        // Force a certain length for names
        if (strlen(name) < 30) {
            puts("name short, padding");
            int i;
            int x = strlen(name);
            for (i = 0; i < 30 - x; i++) {
                strcat(name, " ");
            }
        }
        else if (strlen(name) >= 30) {
            name[30] = '\0';
        }
        // Add the name
        strcpy(entries[pos][0], name);

        // Convert the score from int to string and add it
        char strScore[256];
        sprintf(strScore, "%d", score);
        strcpy(entries[pos][1], strScore);

        // Determine the current time, format it, then add it
        time_t rawtime;
        struct tm *info;
        char tStr[20];
        time(&rawtime);
        info = localtime(&rawtime);
        strftime(tStr, sizeof(tStr), "%x", info);
        strcpy(entries[pos][2], tStr);

        // Save highscores
        saveHighScores();
    }
    else {
        printf("Score %d did not exceed any current scores\n", score);
    }

}

/*
DESC:       - Check if a given score is higher than the current high scores
PRE:        - Highscores.txt exists and has scores
POST:       - N/A
PARAMS:     - score: The score to check
RETURN:     - 1 if score is a highscore, 0 if not
CLEANUP:    - N/A
*/
int checkHighScore(int score) {
    int i;
    for (i = 0; i < 10; i++) {
        // Compare the int values of the supplied entry and highscores
        // If the entry is higher than a highscore, return 1 (true)
        int b = atoi(entries[i][1]);
        if (score > b) return 1;
    }
    return 0;
}
