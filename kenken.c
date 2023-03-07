/** @file main.c
* @brief Kenken puzzle interactive and automatic solver
*
* Reads a txt file with the unsolved puzzle. Stores the puzzle in a correct format and the user
* decides to solve it interactively or to get an automatic solution. Prints the solved puzzle in a text file.
 * The text file is considered to be in a correct format.
*
* @author Socrates Yiannakou
* @bug Not any known bugs
*/

//Include needed header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//Max Size=9x9
#define S 9

//Function prototypes
void readPuzzle(char *file, int result[], char operation[], char puzzle[S][S]);

void play(int result[], char operation[], char puzzle[S][S], int values[S][S]);

void writePuzzle(char puzzle[S][S], int values[S][S]);

bool isValid(int index, char letter, int number, int row, int column, int result[], char operation[],
             char puzzle[S][S], int values[S][S]);

bool validRow(int number, int row, int values[S][S]);

bool validColumn(int number, int column, int values[S][S]);

bool validCage(int index, char letter, int number, int row, int column, int result[],
               char operation[], char puzzle[S][S], int values[S][S]);

bool solve(int result[], char operation[], char puzzle[S][S], int values[S][S]);

//Global variables
int size = 0;
int counter = 0;
char name[50];
char newName[50];
FILE *fp=NULL;

/** @brief Reads the unsolved puzzle from a text file and stores it in a valid format
*
* Reads the size of the puzzle from the first line, if it is bigger than the max size, program exits.
* Reads from every other line the result of the operation, the operators and stores them in arrays.
* While reading the data, a 2D array of characters is created with letters from the alphabet.
*
* @param *file Pointer that points input file string
* @param result[] Array of integers where the results of the operations are stored
* @param operation[] Array of characters where the operators are stored
* @param puzzle[][] 2D Array of characters where the unsolved puzzle is stored using alphabetic characters
* @return Void.
*/
void readPuzzle(char *file, int result[], char operation[], char puzzle[S][S]) {
    fp = fopen(file, "r");
    int lineCount = 0; //counter for the first line
    char line[9999]; //stores the contents of each line of the text
    char *ptr=NULL; //used for stroul() function
    char *token=NULL; //used for tokenizer() function
    char alphabet = 'a'; //character counter

    while ((!feof(fp)) && (lineCount <= 26)) {
        bool  check= fgets(line, 9999, fp);
        if (lineCount == 0) {
            size = strtoul(line, &ptr, 10);
            lineCount++;
            if (size > S) {
                exit(0);
            }
        } else {
            if (check) {
                counter++;
                result[counter] = strtoul(line, &ptr, 10);
                if (strstr(line, "*")) {
                    operation[counter] = '*';
                } else {
                    operation[counter] = '+';
                }
                token = strtok(line, " ");
                while (token != NULL) {
                    token = strtok(NULL, " ");
                    if (token != NULL) {
                        int temp1 = token[1] - '0';
                        int temp2 = token[3] - '0';
                        int i, j;
                        for (i = 1; i <= size; i++) {
                            for (j = 1; j <= size; j++) {
                                if (temp1 == i && temp2 == j) {
                                    puzzle[i][j] = alphabet;
                                }

                            }
                        }
                    }
                }
                alphabet++;
            }
        }
    }
    fclose(fp);
}

/** @brief Interactive kenken solver
*
* Shows the unsolved puzzle to the screen.
* Restricts the user from entering illegal input.
* Stores the input of the user in a 2D array of integers.
* If the puzzle is solved, a message is shown and the method writePuzzle() is called.
*
* @param result[] Array of integers where the results of the operations are stored
* @param operation[] Array of characters where the operators are stored
* @param puzzle[][] 2D Array of characters where the unsolved puzzle is stored using alphabetic characters
* @param values[][] 2D Array of integers where the input from the user is stored. Initially is filled with zeros.
* @return Void.
*/
void play(int result[], char operation[], char puzzle[S][S], int values[S][S]) {
    int i, k, j;
    int condition = 0; //checks if the puzzle is solved

    while (condition == 0) {
        char alph = 'a';
        for (i = 1; i <= counter; i++) {
            printf("%c=%d%c ", alph, result[i], operation[i]);
            alph++;
        }
        printf("\n\n");
        for (k = 1; k <= size; k++) {
            printf("+------");
        }
        printf("+\n");
        for (i = 1; i <= size; i++) {
            for (j = 1; j <= size; j++) {
                printf("| %2d%1c  ", values[i][j], puzzle[i][j]);
            }
            printf("|\n");
            for (k = 1; k <= size; k++) {
                printf("+------");
            }
            printf("+\n");
        }
        printf("\n\n\nEnter your command in the following format:\n+ i,j=val:"
               " for entering val at position (i,j)\n+ i,j=0 : for clearing cell (i,j)\n+ 0,0=0 : "
               "for saving and ending the game\nNotice: i,j,val numbering is from [1..%d]\n", size);

        int pos1, pos2, val; // row,column,value - entered by user

        if (scanf("%d,%d=%d", &pos1, &pos2, &val) != 3) {
            while (getchar() != '\n') {};
            printf("\nwrong format of command\n");
        } else if ((pos1 >= 1 && pos1 <= size) && (pos2 >= 1 && pos2 <= size) && (val == 0)) {
            values[pos1][pos2] = 0;
            printf("\n Value cleared!\n");
        } else if ((pos1 == 0) && (pos2 == 0) && (val == 0)) {
            printf("\n***Exiting***\n");
            writePuzzle(puzzle, values);
            exit(0);
        } else if ((pos1 < 1 || pos1 > size) || (pos2 < 1 || pos2 > size) || (val < 1 || val > size) ||
                   (values[pos1][pos2] != 0)) {
            printf("\nError: Illegal value insertion!\n");
        } else {
            condition = 1;
            for (i = 1; i <= size; i++) {
                if (values[pos1][i] == val && i != pos2) {
                    printf("\nError: Illegal value insertion!\n");
                    condition = 0;
                    break;
                }
            }
            for (i = 1; i <= size; i++) {
                if (values[i][pos2] == val && i != pos1) {
                    printf("\nError: Illegal value insertion!\n");
                    condition = 0;
                    break;
                }
            }
            if (condition == 1) {
                values[pos1][pos2] = val;
                char letter = puzzle[pos1][pos2];
                for (i = 1; i <= size; i++) {
                    for (j = 1; j <= size; j++) {
                        if (puzzle[i][j] == letter) {
                            if (values[i][j] == 0) {
                                printf("\nValue inserted!\n");
                                condition = 0;
                                break;
                            }
                        }
                    }
                }
            }
            if (condition == 1) {
                char letter = puzzle[pos1][pos2];
                int temp, num;
                alph = 'a';
                for (k = 1; k <= counter; k++) {
                    if (letter == alph) {
                        num = k;
                        break;
                    }
                    alph++;
                }
                if (operation[num] == '+') {
                    temp = 0;
                } else {
                    temp = 1;
                }
                for (i = 1; i <= size; i++) {
                    for (j = 1; j <= size; j++) {
                        if (puzzle[i][j] == letter) {
                            if (operation[num] == '+') {
                                temp += values[i][j];
                            } else {
                                temp *= values[i][j];
                            }
                        }
                    }
                }
                if (result[num] != temp) {
                    condition = 0;
                    values[pos1][pos2] = 0;
                    printf("\nError: Illegal value insertion!\n");
                }
            }
            if (condition == 1) {
                int temp;
                alph = 'a';
                for (k = 1; k <= counter; k++) {
                    if (operation[k] == '+') {
                        temp = 0;
                    } else {
                        temp = 1;
                    }
                    for (i = 1; i <= size; i++) {
                        for (j = 1; j <= size; j++) {
                            if (puzzle[i][j] == alph) {
                                if (operation[k] == '+') {
                                    temp += values[i][j];
                                } else {
                                    temp *= values[i][j];
                                }
                            }
                        }
                    }
                    if (result[k] != temp) {
                        condition = 0;
                        break;
                    }
                    alph++;
                }
            }
        }
        if (condition == 0) {
            printf("\n***This is not a valid Kenken puzzle***\n");
        }
    }
    printf("\n***Game solved***\n");
    writePuzzle(puzzle, values);
}
/** @brief Prints the solved kenken puzzle
*
* Creates the puzzle diagram.
* Prints to a txt file the solved puzzle.
*
* @param puzzle[][] 2D Array of characters where the unsolved puzzle is stored using alphabetic characters
* @param values[][] 2D Array of integers where the input from the user is stored. Initially is filled with zeros.
* @return Void.
*/
void writePuzzle(char puzzle[S][S], int values[S][S]) {
    int i, k, j;

    fp = fopen(newName, "w");
    fprintf(fp, "\n\n");
    for (k = 1; k <= size; k++) {
        fprintf(fp, "+------");
    }
    fprintf(fp, "+\n");
    for (i = 1; i <= size; i++) {
        for (j = 1; j <= size; j++) {
            fprintf(fp, "| %2d%1c  ", values[i][j], puzzle[i][j]);
        }
        fprintf(fp, "|\n");
        for (k = 1; k <= size; k++) {
            fprintf(fp, "+------");
        }
        fprintf(fp, "+\n");
    }
    fclose(fp);
    exit(0);
}
/** @brief Checks if all 3 statements are true
*
* Checks if the functions validColumn(),validRow(),validCage() are all true.

* @param index index for the result[] and operation[] arrays
* @param letter of the puzzle[][] array
* @param number number to be tested
* @param row row index
* @param column column index
* @param result[] Array of integers where the results of the operations are stored
* @param operation[] Array of characters where the operators are stored
* @param puzzle[][] 2D Array of characters where the unsolved puzzle is stored using alphabetic characters
* @param values[][] 2D Array of integers where the input from the user is stored. Initially is filled with zeros.
* @return true or false
*/
bool isValid(int index, char letter, int number, int row, int column, int result[],
             char operation[], char puzzle[S][S], int values[S][S]) {
    return validColumn(number, column, values) && validRow(number, row, values)
           && validCage(index, letter, number, row, column, result, operation, puzzle, values);
}

/** @brief Checks if the column is valid
*
* @param number number to be tested
* @param column column index
* @param values[][] 2D Array of integers where the input from the user is stored. Initially is filled with zeros.
* @return true or false
*/
bool validColumn(int number, int column, int values[S][S]) {

    int i, k, j;
    for (i = 1; i <= size; i++) {
        if (values[i][column] == number) {
            return false;
        }
    }
    return true;
}

/** @brief Checks if the row is valid
*
* @param number number to be tested
* @param row row index
* @param values[][] 2D Array of integers where the input from the user is stored. Initially is filled with zeros.
* @return true or false
*/
bool validRow(int number, int row, int values[S][S]) {

    int i, k, j;
    for (i = 1; i <= size; i++) {
        if (values[row][i] == number) {
            return false;
        }
    }
    return true;
}

/** @brief Checks if cage is valid
*
* @param index index for the result[] and operation[] arrays
* @param letter of the puzzle[][] array
* @param number number to be tested
* @param row row index
* @param column column index
* @param result[] Array of integers where the results of the operations are stored
* @param operation[] Array of characters where the operators are stored
* @param puzzle[][] 2D Array of characters where the unsolved puzzle is stored using alphabetic characters
* @param values[][] 2D Array of integers where the input from the user is stored. Initially is filled with zeros.
* @return true or false
*/
bool validCage(int index, char letter, int number, int row, int column, int result[], char operation[],
               char puzzle[S][S], int values[S][S]) {

    int i, k, j, temp;
    char alph;   //character counter
    bool filled = true; //checks if the values array is filled with numbers

    values[row][column]=number;

    for (i = 1; i <= size; i++) {
        for (j = 1; j <= size; j++) {
            if (puzzle[i][j] == letter) {
                if (values[i][j] == 0) {
                    filled = false;

                }
            }
        }
    }


    if (operation[index] == '+') {
        temp = 0;
    } else {
        temp = 1;
    }
    for (i = 1; i <= size; i++) {
        for (j = 1; j <= size; j++) {
            if (puzzle[i][j] == letter) {
                if (operation[index] == '+') {
                    temp += values[i][j];
                } else {
                    temp *= values[i][j];
                }

            }

        }
    }
    if ((result[index] <= temp) && (filled == false) && (operation[index] == '+')) {
        values[row][column]=0;
        return false;
    }
    if ((result[index] < temp) && (filled == false) && (operation[index] == '*')) {
        values[row][column]=0;
        return false;
    }
    if ((result[index] != temp) && (filled == true)) {
        values[row][column]=0;
        return false;
    }



    values[row][column]=0;
    return true;


}
/** @brief Solves the puzzle automatically
*
* Using recursion and the above methods the puzzle is solved.
* Calls writePuzle() to print the puzzle.
*
* @param result[] Array of integers where the results of the operations are stored
* @param operation[] Array of characters where the operators are stored
* @param puzzle[][] 2D Array of characters where the unsolved puzzle is stored using alphabetic characters
* @param values[][] 2D Array of integers where the input from the user is stored. Initially is filled with zeros.
* @return true or false
*/
bool solve(int result[], char operation[], char puzzle[S][S], int values[S][S]) {
    char letter;  //character counter

    int index;  //integer counter
    int i, k, j;
    for (i = 1; i <= size; i++) {
        for (j = 1; j <= size; j++) {
            if (values[i][j] == 0) {
                for (k = 1; k <= size; k++) {
                    index = 1;
                    letter = 'a';
                    while (puzzle[i][j] != letter) {
                        letter++;
                        index++;
                    }
                    if ((isValid(index, letter, k, i, j, result, operation, puzzle, values))) {
                        values[i][j] = k;
                        if (solve(result, operation, puzzle, values)) {
                            return true;
                        } else {
                            values[i][j] = 0;
                        }
                    }
                }
                return false;
            }
        }
    }
    printf("\n***Game solved***");
    writePuzzle(puzzle, values);
    return true;

}

/** @brief Main function
*
* @param argc number of arguments
* @param argv[] points to the command line arguments
* @return 0 if there were no problems
*/
int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Wrong Input\n");
        return 0;
    }
    if ((strcmp(argv[1], "-s") != 0) && (strcmp(argv[1], "-i"))) {
        printf("Wrong Input\n");
        return 0;
    }
    fp = fopen(argv[2], "r");
    if (fp == NULL) {
        printf("Could not open file\n");
        return 0;
    } else {
        strcpy(name, argv[2]);
        strcpy(newName, "out-");
        strcat(newName, name);

        int i, k, j;
        char puzzle[S][S]; //2d array of letters
        char operation[26]; // array of the operators, + or *
        int result[26];  //array of the result of each operation, in each line
        int values[S][S] = {0}; //array of integers for the puzzle,initially 0

        readPuzzle(argv[2], result, operation, puzzle);
        if (strcmp(argv[1], "-i") == 0) {
            play(result, operation, puzzle, values);
        } else {
            solve(result, operation, puzzle, values);
        }
        fclose(fp);
        return 0;

    }
}

