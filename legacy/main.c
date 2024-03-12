#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 12
#define MINES 12

char board[SIZE][SIZE];
char hiddenBoard[SIZE][SIZE];

void initializeBoard() {
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            board[i][j] = '-';
            hiddenBoard[i][j] = '-';
        }
    }
}

void printBoard(char b[SIZE][SIZE]) {
    for(int i = 0; i < SIZE; i++) {
        for(int j = 0; j < SIZE; j++) {
            printf("%c ", b[i][j]);
        }
        printf("\n");
    }
}

void placeMines() {
    srand(time(NULL));
    for(int i = 0; i < MINES; i++) {
        int row = rand() % SIZE;
        int col = rand() % SIZE;
        hiddenBoard[row][col] = '*';
    }
}

int countMines(int row, int col) {
    int count = 0;
    for(int i = row - 1; i <= row + 1; i++) {
        for(int j = col - 1; j <= col + 1; j++) {
            if(i >= 0 && i < SIZE && j >= 0 && j < SIZE) {
                if(hiddenBoard[i][j] == '*') {
                    count++;
                }
            }
        }
    }
    return count;
}

void revealBoard(int row, int col) {
    if(row < 0 || row >= SIZE || col < 0 || col >= SIZE || board[row][col] != '-') {
        return;
    }
    int count = countMines(row, col);
    board[row][col] = count + '0';
    if(count == 0) {
        for(int i = row - 1; i <= row + 1; i++) {
            for(int j = col - 1; j <= col + 1; j++) {
                revealBoard(i, j);
            }
        }
    }
}

int main() {
    initializeBoard();
    placeMines();
    printf("Minesweeper\n\n");
    printBoard(board);
    printf("\n");

    int row, col;
    while(1) {
        printf("Enter row and column to reveal (0-indexed): ");
        scanf("%d %d", &row, &col);
        if(row < 0 || row >= SIZE || col < 0 || col >= SIZE) {
            printf("Invalid input! Please try again.\n");
            continue;
        }
        if(hiddenBoard[row][col] == '*') {
            printf("Game Over! You hit a mine.\n");
            break;
        }
        revealBoard(row, col);
        printBoard(board);
        printf("\n");
    }

    return 0;
}
