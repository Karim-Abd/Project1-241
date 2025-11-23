#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define ROWS 6
#define COLS 7
#define PORT 4444

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

int main() {
    int sockfd;
    struct sockaddr_in serv_addr;

    printf("Starting Connect 4 Client (Player B)...\n");

    // create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // bind server 
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    // connect to hhe same port: 4444
    connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    printf("Connected to server.\n");

    char board[ROWS][COLS];
    char currentPlayer;

    while (1) {
        // Read board
        ssize_t b = read(sockfd, board, ROWS * COLS);
        if (b <= 0) break;

        // Read currentPlayer
        read(sockfd, &currentPlayer, 1);

        printBoard(board);

        if (currentPlayer == 'B') {
            int col;
            printf("\nPlayer B, choose a column (1–7): ");
            scanf("%d", &col);
            col--;

            write(sockfd, &col, sizeof(int));
        }
    }

    close(sockfd);
    return 0;
}