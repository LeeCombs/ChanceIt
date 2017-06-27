#include <stdio.h>


/*
DESC:       - Return the first char given through stdin then flushes it
PRE:        - N/A
POST:       - stdin is flushed
PARAMS:     - N/A
RETURN:     - The first char in stdin, '.' if no input was given
CLEANUP:    - N/A
*/
char getFirstChar() {
    char ch;
    ch = getchar();

    // If only a newline was given, return a period
    if (ch == '\n') return '.';

    // Flush stdin
    while(getchar() != '\n');

    return tolower(ch);
}

/*
DESC:       - Loop until the user gives valid input
PRE:        - N/A
POST:       - N/A
PARAMS:     - *validString: A string of valid chars considered valid input
RETURN:     - A char within validString
CLEANUP:    - N/A
*/
char getChar_Player(char *validString){
    if (validString == NULL || validString == "") {
        printf("Invalid validString given: %s", validString);
        exit(-1);
    }

    char ret;
    while (1) {
        ret = getFirstChar();
        if (strchr(validString, ret) == NULL){
            printf("Invalid input, please input a selection above!\n");
        }
        else break;
    }
    return tolower(ret);
}


/*
DESC:       -
PRE:        -
POST:       -
PARAMS:     -
RETURN:     -
CLEANUP:    -
*/
#define OK          0
#define NO_INPUT    1
#define TOO_LONG    2
int getLine(char *buff, size_t sz){
    int ch, extra;

    // Get line with buffer overrun protection
    if (fgets(buff, sz, stdin) == NULL) {
        return NO_INPUT;
    }

    // If too long, there will be no newline. In that case, flush
    // to end of line so excess doesn't affect the next call
    if(buff[strlen(buff)-1] != '\n') {
        extra = 0;
        while (((ch = getchar()) != '\n') && (ch != EOF)){
            extra = 1;
        }
        return (extra == 1) ? TOO_LONG : OK;
    }

    // Remove newline and give string back to caller
    buff[strlen(buff)-1] = '\0';
    return OK;
}


/*
DESC:       -
PRE:        -
POST:       -
PARAMS:     -
RETURN:     -
CLEANUP:    -
*/
char* getLine_Player(){
    while(1) {
        int rc;
        char buff[15];
        rc = getLine(buff, sizeof(buff));
        if (rc == NO_INPUT) {
            printf("No input\n");
            continue;
        }
        if (rc == TOO_LONG) {
            printf("Input too long [%s]\n", buff);
            continue;
        }
        printf("OK [%s]\n", buff);
        return buff;
    }
}
