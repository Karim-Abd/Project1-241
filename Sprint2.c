#include <stdio.h>
#include <stdlib.h>
#include <time.h>;
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
    printf(" -------------\n");
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
int EasyBot(char board [ROWS][COLS]){
    int valid = 0;
    int col;
    while(!valid){
        col=rand()%COLS;
        if(board[0][col]==' ')
        valid=1;
    }
    return col;
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
    fflush(stdout);
    printf("Select Mode: \n");
    printf("1. Player vs Player \n");
    printf("2. Player vs Easy Bot \n");
    printf("Choice: ");
    scanf("%d",&modes);
    printf("Player A name: ");
    scanf("%19s", playerAName);
    if(modes==1){
    printf("Player B name: ");
    scanf("%19s", playerBName);
    }
    else{
        snprintf(playerBName,sizeof(playerBName),"Easy Bot");
        printf("You will play against %s", playerBName);
    }
    
    char player = 'A';
    int moves = 0;
    int maxMoves = ROWS * COLS;
    printBoard(board);

    while (moves < maxMoves) {
        int col,row;
        if (modes == 2 && player == 'B') {
            col = EasyBot(board);
            row = dropChecker(board, col, player);
            printBoard(board);
            printf("\n%s chose column %d\n", playerBName, col + 1);
        } 
        else{
      if (player == 'A')
    printf("\nPlayer %s, choose a column (1-7): ", playerAName);
    else
    printf("\nPlayer %s, choose a column (1-7): ", playerBName);
fflush(stdout);

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

         row = dropChecker(board, col, player);
        if (row == -1) {
            printf("Column is full. Try another.\n");
            continue;
        }
        printBoard(board);
    }
        
        moves++;

        if (checkWin(board, row, col, player)) {
         if (modes == 2 && player == 'B')
        printf("\n%s wins!\n", playerBName);
         else if (player == 'A')
        printf("\n%s wins!\n", playerAName);
        else
        printf("\n%s wins!\n", playerBName);
        return 0;
        }

        player = (player == 'A') ? 'B' : 'A';
    }

    printf("\nIt's a draw!\n");
    return 0;
}
