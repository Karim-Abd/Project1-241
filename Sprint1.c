#include <stdio.h>
#include <stdlib.h>

#define ROWS 6
#define COLS 7

void printBoard(char board[ROWS][COLS]) {
    printf("\n");
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            char display = (board[r][c] == ' ') ? '.' : board[r][c];
            printf(" %c", display);
        }
        printf("\n");
    }
    printf("--------------\n");
    printf(" 1 2 3 4 5 6 7\n");
}

int dropChecker(char board[ROWS][COLS], int col, char player) {
    for (int r = ROWS - 1; r >= 0; r--) {
        if (board[r][col] == ' ') {
            board[r][col] = player;
            return r;
        }
    }
    return -1; 
}

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

int main() {
    char board[ROWS][COLS];
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            board[r][c] = ' ';

    char player = 'A';
    int moves = 0;
    int maxMoves = ROWS * COLS;
    printf("Welcome to Connect Four!(Two Players)\n");
    printBoard(board);

    while (moves < maxMoves) {
        int col;
        printf("\nPlayer %c, choose a column (1-7): ", player);
        fflush(stdout);

    if (scanf(" %d", &col) != 1) {
    int ch;
    while ((ch =getchar()) != '\n' && ch != EOF) {}
    printf("Invalid input! Please enter a number between 1 and 7.\n");
    continue;
}
    col--; 

if (col < 0 || col >= COLS) {
    printf("Invalid column. Please try again.\n");
    continue;
}

        int row = dropChecker(board, col, player);
        if (row == -1) {
            printf("Column is full. Try another.\n");
            continue;
        }

        printBoard(board);
        moves++;

        if (checkWin(board, row, col, player)) {
            printf("\nPlayer %c wins!\n", player);
            return 0;
        }

        player = (player == 'A') ? 'B' : 'A';
    }

    printf("\nIt's a draw!\n");
    return 0;
}
