#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define ROWS 6
#define COLS 7
#define PORT 4444

// same base code for the board and logic 
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
            int r = row + dr*i;
            int c = col + dc*i;
            if (r < 0 || r >= ROWS || c < 0 || c >= COLS || board[r][c] != player)
                break;
            count++;
        }
        for (int i = 1; i < 4; i++) {
            int r = row - dr*i;
            int c = col - dc*i;
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
    int server_fd, client_fd;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

    char board[ROWS][COLS];
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            board[r][c] = ' ';

    printf("Loading Player A\n");

    // create the socket  
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // bind 
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    // listen for any connections to the created socket
    listen(server_fd, 1);
    printf("Server waiting on port %d...\n", PORT);

    // accept client (playerb)
    client_fd = accept(server_fd, (struct sockaddr *)&address, &addrlen);
    printf("PlayerB connected!\n");

    char currentPlayer = 'A';
    int moves = 0;
    int maxMoves = ROWS * COLS;

    while (moves < maxMoves) {
        printBoard(board);

        if (currentPlayer == 'A') {
            int col;
            printf("\nPlayer A, choose a column (1–7): ");
            scanf("%d", &col);
            col--;

            int row = dropChecker(board, col, 'A');
            if (row == -1) {
                printf("Column full. Try again.\n");
                continue;
            }

            // update the board and current player 
            write(client_fd, board, ROWS * COLS);
            write(client_fd, &currentPlayer, 1);

            if (checkWin(board, row, col, 'A')) {
                printf("\nPlayer A wins!\n");
                break;
            }

        } else { 
            printf("\nPlayer B's turn...\n");

            // update the board and current player 
            write(client_fd, board, ROWS * COLS);
            write(client_fd, &currentPlayer, 1);

            int colB;
            read(client_fd, &colB, sizeof(int));
            int row = dropChecker(board, colB, 'B');

            if (checkWin(board, row, colB, 'B')) {
                printBoard(board);
                printf("\nPlayer B wins!\n");
                break;
            }
        }

        currentPlayer = (currentPlayer == 'A') ? 'B' : 'A';
        moves++;
    }

    close(client_fd);
    close(server_fd);

    return 0;
}