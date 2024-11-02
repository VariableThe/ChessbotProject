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

// Function to validate player's pawn move
bool isValidPawnMove(int x, int y, int newX, int newY) {
    // Ensure the move is within bounds
    if (!isWithinBounds(newX, newY)) {
        return false;
    }
    // Pawn moves one step forward
    if (newX == x - 1 && newY == y && board[newX][newY] == 0) {
        return true;
    }
    // Pawn captures diagonally
    if (newX == x - 1 && (newY == y - 1 || newY == y + 1) && board[newX][newY] < 0) {
        return true;
    }
    return false;
}

// Function to validate rook move
bool isValidRookMove(int x, int y, int newX, int newY) {
    if (!isWithinBounds(newX, newY)) {
        return false;
    }
    if (x == newX) { // Horizontal move
        int step = (newY > y) ? 1 : -1;
        for (int j = y + step; j != newY; j += step) {
            if (board[x][j] != 0) {
                return false; // Path is blocked
            }
        }
        return board[newX][newY] <= 0; // Valid if destination is empty or contains opponent
    } else if (y == newY) { // Vertical move
        int step = (newX > x) ? 1 : -1;
        for (int i = x + step; i != newX; i += step) {
            if (board[i][y] != 0) {
                return false; // Path is blocked
            }
        }
        return board[newX][newY] <= 0; // Valid if destination is empty or contains opponent
    }
    return false;
}

// Function to validate knight move
bool isValidKnightMove(int x, int y, int newX, int newY) {
    if (!isWithinBounds(newX, newY)) {
        return false;
    }
    int dx = abs(newX - x);
    int dy = abs(newY - y);
    return (dx == 2 && dy == 1) || (dx == 1 && dy == 2); // L-shaped move
}

// Function to validate bishop move
bool isValidBishopMove(int x, int y, int newX, int newY) {
    if (!isWithinBounds(newX, newY)) {
        return false;
    }
    if (abs(newX - x) == abs(newY - y)) { // Diagonal move
        int stepX = (newX > x) ? 1 : -1;
        int stepY = (newY > y) ? 1 : -1;
        for (int i = x + stepX, j = y + stepY; i != newX; i += stepX, j += stepY) {
            if (board[i][j] != 0) {
                return false; // Path is blocked
            }
        }
        return board[newX][newY] <= 0; // Valid if destination is empty or contains opponent
    }
    return false;
}

// Function to validate queen move
bool isValidQueenMove(int x, int y, int newX, int newY) {
    return isValidRookMove(x, y, newX, newY) || isValidBishopMove(x, y, newX, newY);
}

// Function to validate king move
bool isValidKingMove(int x, int y, int newX, int newY) {
    if (!isWithinBounds(newX, newY)) {
        return false;
    }
    int dx = abs(newX - x);
    int dy = abs(newY - y);
    return (dx <= 1 && dy <= 1) && (board[newX][newY] <= 0); // One square in any direction
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
    if (!isWithinBounds(x, y) || !isWithinBounds(newX, newY)) {
        printf("Invalid move: Out of bounds. Try again.\n");
        return false;
    }

    // Ensure player is moving their own piece
    if (board[x][y] <= 0) {
        printf("Invalid move: No valid piece at the selected position. Try again.\n");
        return false;
    }

    // Validate move based on piece type
    switch (board[x][y]) {
        case 1: // White pawn
            if (isValidPawnMove(x, y, newX, newY)) {
                board[newX][newY] = board[x][y];
                board[x][y] = 0;
                return true;
            }
            break;
        case 2: // White rook
            if (isValidRookMove(x, y, newX, newY)) {
                board[newX][newY] = board[x][y];
                board[x][y] = 0;
                return true;
            }
            break;
        case 3: // White knight
            if (isValidKnightMove(x, y, newX, newY)) {
                board[newX][newY] = board[x][y];
                board[x][y] = 0;
                return true;
            }
            break;
        case 4: // White bishop
            if (isValidBishopMove(x, y, newX, newY)) {
                board[newX][newY] = board[x][y];
                board[x][y] = 0;
                return true;
            }
            break;
        case 5: // White queen
            if (isValidQueenMove(x, y, newX, newY)) {
                board[newX][newY] = board[x][y];
                board[x][y] = 0;
                return true;
            }
            break;
        case 6: // White king
            if (isValidKingMove(x, y, newX, newY)) {
                board[newX][newY] = board[x][y];
                board[x][y] = 0;
                return true;
            }
            break;
    }

    printf("Invalid move: Unsupported piece movement. Try again.\n");
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
