#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define ROWS 6
#define COLS 7

// prints the board 
void printBoard(char board[ROWS][COLS]) {
    printf("\n");
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            char display = (board[r][c] == ' ') ? '.' : board[r][c];
            printf(" %c", display);
        }
        printf("\n");
    }
    printf(" -------------\n");
    printf(" 1 2 3 4 5 6 7\n");
}

// responsible for ensuring the checker drops to the lowest available spot in the selected column
int dropChecker(char board[ROWS][COLS], int col, char player) {
    for (int r = ROWS - 1; r >= 0; r--) {
        if (board[r][col] == ' ') {
            board[r][col] = player;
            return r;
        }
    }
    return -1; 
}

// Called to check if the parameters for a win are true 
int checkWin(char board[ROWS][COLS], int row, int col, char player) {
    int directions[4][2] = {{0,1},{1,0},{1,1},{1,-1}}; 

    for (int d = 0; d < 4; d++) {
        int count = 1;
        int dr = directions[d][0];
        int dc = directions[d][1];

        for (int i = 1; i < 4; i++) {
            int r = row + dr*i, c = col + dc*i;
            if (r < 0 || r >= ROWS || c < 0 || c >= COLS || board[r][c] != player)
                break;
            count++;
        }
        for (int i = 1; i < 4; i++) {
            int r = row - dr*i, c = col - dc*i;
            if (r < 0 || r >= ROWS || c < 0 || c >= COLS || board[r][c] != player)
                break;
            count++;
        }
        if (count >= 4)
            return 1;
    }
    return 0;
}

// easy bot implementation - looks for empty places and randomly picks one, then it builds up until it wins. If it is blocked it then looks for other places and repeating the cycle.
int EasyBot(char board [ROWS][COLS]){
    int valid = 0;
    int col;
    while(!valid){
        col=rand()%COLS;
        if(board[0][    col]==' ')
        valid=1;
    }
    return col;
}

// Medium bot implementation
int MediumBot(char board[ROWS][COLS], char botPlayer, char humanPlayer) {
    // Begins by checkign if the bot can win with this move 
    for (int col = 0; col < COLS; col++) {
        if (board[0][col] == ' ') {
            int row = dropChecker(board, col, botPlayer);
            if (row != -1) {
                if (checkWin(board, row, col, botPlayer)) {
                    board[row][col] = ' '; 
                    return col; 
                }
                board[row][col] = ' '; 
            }
        }
    }
    // if it cant win then it will check whether its opponent is about to win, and blocks it. 
    for (int col = 0; col < COLS; col++) {
        if (board[0][col] == ' ') {
            int row = dropChecker(board, col, humanPlayer);
            if (row != -1) {
                if (checkWin(board, row, col, humanPlayer)) {
                    board[row][col] = ' ';
                    return col; 
                }
                board[row][col] = ' ';
            }
        }
    }

    // looks for its own checker to stack but only if the checker is at row 3, 4, or 5 (as it cant win if it wants to stack 4)
    for (int r = ROWS - 1; r > 0; r--) { 
        for (int col = 0; col < COLS; col++) {
            if (board[r][col] == botPlayer) {
                if (r <= 2) continue; 
                if (board[r - 1][col] == ' ') {
                    return col; 
                }
            }
        }
    }

    // this is the starting point where if none of the terms above apply, it calls the easybot
    return EasyBot(board);
}

int main() {
    setvbuf(stdout, NULL, _IONBF, 0);
    srand(time(NULL)); 

    char board[ROWS][COLS];
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            board[r][c] = ' ';

    char playerAName[20], playerBName[20];
    int modes;
    printf("Welcome to Connect Four!\n");
    printf("Select Mode: \n");
    printf("1. Player vs Player \n");
    printf("2. Player vs Easy Bot \n");
    printf("3. Player vs Medium Bot \n");
    printf("Choice: ");
    scanf("%d", &modes);

    printf("Player A name: ");
    scanf("%19s", playerAName);

    if (modes == 1) {
        printf("Player B name: ");
        scanf("%19s", playerBName);
    }
    else if (modes == 2) {
        snprintf(playerBName, sizeof(playerBName), "Easy Bot");
        printf("You are playing against %s\n", playerBName);
    }
    else if (modes == 3) {
        snprintf(playerBName, sizeof(playerBName), "Medium Bot");
        printf("You are playing against %s\n", playerBName);
    }

    char currentPlayer = 'A'; // 'A' = PlayerA, 'B' = Bot or PlayerB
    int moves = 0, maxMoves = ROWS * COLS;
    printBoard(board);

    while (moves < maxMoves) {
        int col, row;

        if ((modes == 2 || modes == 3) && currentPlayer == 'B') {
            if (modes == 2)
                col = EasyBot(board);
            else
                col = MediumBot(board, 'B', 'A');

            row = dropChecker(board, col, 'B');
            printBoard(board);
            printf("\n%s chose column %d\n", playerBName, col + 1);
        }
        else {
            if (currentPlayer == 'A')
                printf("\n%s, choose a column (1-7): ", playerAName);
            else
                printf("\n%s, choose a column (1-7): ", playerBName);

            if (scanf("%d", &col) != 1) {
                int ch;
                while ((ch = getchar()) != '\n' && ch != EOF) {}
                printf("Invalid input! Try again.\n");
                continue;
            }
            col--;

            if (col < 0 || col >= COLS) {
                printf("Invalid column. Try again.\n");
                continue;
            }

            row = dropChecker(board, col, currentPlayer);
            if (row == -1) {
                printf("Column is full. Try another.\n");
                continue;
            }
            printBoard(board);
        }

        moves++;
        if (checkWin(board, row, col, currentPlayer)) {
            if (currentPlayer == 'A')
                printf("\n%s wins!\n", playerAName);
            else
                printf("\n%s wins!\n", playerBName);
            return 0;
        }

        currentPlayer = (currentPlayer == 'A') ? 'B' : 'A';
    }

    printf("\nIt's a draw!\n");
    return 0;
}