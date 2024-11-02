#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define SIZE 8

// Board representation
int board[SIZE][SIZE] = {
    {-2, -3, -4, -5, -6, -4, -3, -2},
    {-1, -1, -1, -1, -1, -1, -1, -1},
    {0,  0,  0,  0,  0,  0,  0,  0},
    {0,  0,  0,  0,  0,  0,  0,  0},
    {0,  0,  0,  0,  0,  0,  0,  0},
    {0,  0,  0,  0,  0,  0,  0,  0},
    {1,  1,  1,  1,  1,  1,  1,  1},
    {2,  3,  4,  5,  6,  4,  3,  2}
};

// Function to display the board
void displayBoard() {
    printf("    1   2   3   4   5   6   7   8\n");
    for (int i = 0; i < SIZE; i++) {
        printf("  +---+---+---+---+---+---+---+---+\n");
        printf("%d |", i + 1);
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) {
                printf("   |");
            } else {
                printf("%3d|", board[i][j]);
            }
        }
        printf("\n");
    }
    printf("  +---+---+---+---+---+---+---+---+\n\n");
    printf("Piece Legend:\n");
    printf("  1 = White Pawn, 2 = White Rook, 3 = White Knight, 4 = White Bishop, 5 = White Queen, 6 = White King\n");
    printf(" -1 = Black Pawn, -2 = Black Rook, -3 = Black Knight, -4 = Black Bishop, -5 = Black Queen, -6 = Black King\n");
}

// Function to validate if a move is within board limits
bool isWithinBounds(int x, int y) {
    return x >= 0 && x < SIZE && y >= 0 && y < SIZE;
}

// Function to check if bot has a capture opportunity with a pawn
bool botHasCaptureOpportunity(int *fromX, int *fromY, int *toX, int *toY) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == -1) { // Bot's pawn
                if (isWithinBounds(i + 1, j + 1) && board[i + 1][j + 1] > 0) {
                    // Capture opportunity to the right diagonal
                    *fromX = i;
                    *fromY = j;
                    *toX = i + 1;
                    *toY = j + 1;
                    return true;
                } else if (isWithinBounds(i + 1, j - 1) && board[i + 1][j - 1] > 0) {
                    // Capture opportunity to the left diagonal
                    *fromX = i;
                    *fromY = j;
                    *toX = i + 1;
                    *toY = j - 1;
                    return true;
                }
            }
        }
    }
    return false;
}

// Bot move function
void botMove() {
    int fromX, fromY, toX, toY;

    // Check for capture opportunity
    if (botHasCaptureOpportunity(&fromX, &fromY, &toX, &toY)) {
        printf("Bot captures a pawn at (%d, %d)!\n", toX + 1, toY + 1);
        board[toX][toY] = board[fromX][fromY];
        board[fromX][fromY] = 0;
        return;
    }

    // If no capture opportunity, make a random pawn move forward
    for (int i = SIZE - 2; i >= 0; i--) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == -1 && board[i + 1][j] == 0) { // Move black pawn forward if empty
                board[i + 1][j] = -1;
                board[i][j] = 0;
                printf("Bot moves pawn from (%d, %d) to (%d, %d)\n", i + 1, j + 1, i + 2, j + 1);
                return;
            }
        }
    }
}

// Function to execute player's move
bool playerMove(int x, int y, int newX, int newY) {
    if (isWithinBounds(x, y) && isWithinBounds(newX, newY) && board[x][y] > 0) {
        // Basic move validation: ensure moving to an empty square or capturing
        if (board[newX][newY] <= 0) {
            board[newX][newY] = board[x][y];
            board[x][y] = 0;
            return true;
        }
    }
    printf("Invalid move. Try again.\n");
    return false;
}

int main() {
    int x, y, newX, newY;

    while (1) {
        displayBoard();

        // Player's move
        printf("Your Move (White):\n");
        printf("Enter your move (format: x y newX newY): ");
        scanf("%d %d %d %d", &x, &y, &newX, &newY);
        if (!playerMove(x - 1, y - 1, newX - 1, newY - 1)) {
            continue;
        }

        // Bot's move
        displayBoard();
        printf("Bot's Move (Black):\n");
        botMove();
    }
    return 0;
}
