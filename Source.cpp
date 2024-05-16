#include <iostream>
#include <string>
#include <iomanip> // For setw()
#include <cstdlib> // For srand() and rand()
#include <ctime>   // For time()
#include <conio.h> // For _getch()
using namespace std;

// Game board
int board[4][4] = {};

// Function to get the ANSI color code for a given tile value
string getColorForValue(int value) {
    string colorCode = "\033[0m"; // Default color
    switch (value) {
    case 0:    colorCode = "\033[0;8m";  break; // Black
    case 2:    colorCode = "\033[0;32m"; break; // Green
    case 4:    colorCode = "\033[0;36m"; break; // Cyan
    case 8:    colorCode = "\033[0;34m"; break; // Blue
    case 16:   colorCode = "\033[0;35m"; break; // Magenta
    case 32:   colorCode = "\033[0;31m"; break; // Red
    case 64:   colorCode = "\033[0;33m"; break; // Yellow
    case 128:  colorCode = "\033[1;33m"; break; // Bright Yellow
    case 256:  colorCode = "\033[1;32m"; break; // Bright Green
    case 512:  colorCode = "\033[1;36m"; break; // Bright Cyan
    case 1024: colorCode = "\033[1;34m"; break; // Bright Blue
    case 2048: colorCode = "\033[1;35m"; break; // Bright Magenta
    }
    return colorCode;
}

void display(int board[4][4]) {
    for (int i = 0; i < 4; ++i) {
        cout << "_____________________\n|";
        for (int j = 0; j < 4; ++j) {
            string color = getColorForValue(board[i][j]);
            cout << color << setw(4) << board[i][j] << "\033[0m|";
        }
        cout << std::endl;
    }
    cout << "---------------------\n";
}

int determineBiggest(int board[4][4], int biggest) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (board[i][j] > biggest) {
                biggest = board[i][j];
            }
        }
    }
    return biggest;
}

bool isFull(int board[4][4]) {
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {
            if (board[row][col] == 0) {
                return false;
            }
        }
    }
    return true;
}

bool up() {
    bool moved = false;
    bool merged[4][4] = {}; // Keep track of which tiles have merged this turn
    for (int col = 0; col < 4; ++col) { // For each column
        for (int row = 1; row < 4; ++row) { // Skip the top row, as it can't move up
            if (board[row][col] == 0) continue; // Skip empty tiles

            int currentRow = row;
            while (currentRow > 0 && (board[currentRow - 1][col] == 0 ||
                (board[currentRow - 1][col] == board[currentRow][col] && !merged[currentRow - 1][col]))) {
                // Move tile up if the spot above is empty or if the same value and hasn't merged
                if (board[currentRow - 1][col] == board[currentRow][col]) {
                    board[currentRow - 1][col] *= 2; // Double the tile value
                    board[currentRow][col] = 0; // Clear the moving tile
                    merged[currentRow - 1][col] = true; // Mark this tile as having merged
                    moved = true;
                    break; // A tile that has merged can't merge again
                }
                else if (board[currentRow - 1][col] == 0) {
                    board[currentRow - 1][col] = board[currentRow][col]; // Move tile up
                    board[currentRow][col] = 0; // Clear the old spot
                    moved = true;
                }
                currentRow--; // Move to the next row up for the next iteration
            }
        }
    }
    return moved;
}

bool down() {
    bool moved = false;
    bool merged[4][4] = {};
    for (int col = 0; col < 4; ++col) {
        for (int row = 2; row >= 0; --row) {
            if (board[row][col] == 0) continue;
            int currentRow = row;
            while (currentRow < 3 && (board[currentRow + 1][col] == 0 ||
                (board[currentRow + 1][col] == board[currentRow][col] && !merged[currentRow + 1][col]))) {
                if (board[currentRow + 1][col] == board[currentRow][col]) {
                    board[currentRow + 1][col] *= 2;
                    board[currentRow][col] = 0;
                    merged[currentRow + 1][col] = true;
                    moved = true;
                    break;
                }
                else if (board[currentRow + 1][col] == 0) {
                    board[currentRow + 1][col] = board[currentRow][col];
                    board[currentRow][col] = 0;
                    moved = true;
                }
                currentRow++;
            }
        }
    }
    return moved;
}

bool left() {
    bool moved = false;
    bool merged[4][4] = {};
    for (int row = 0; row < 4; ++row) {
        for (int col = 1; col < 4; ++col) {
            if (board[row][col] == 0) continue;
            int currentCol = col;
            while (currentCol > 0 && (board[row][currentCol - 1] == 0 ||
                (board[row][currentCol - 1] == board[row][currentCol] && !merged[row][currentCol - 1]))) {
                if (board[row][currentCol - 1] == board[row][currentCol]) {
                    board[row][currentCol - 1] *= 2;
                    board[row][currentCol] = 0;
                    merged[row][currentCol - 1] = true;
                    moved = true;
                    break;
                }
                else if (board[row][currentCol - 1] == 0) {
                    board[row][currentCol - 1] = board[row][currentCol];
                    board[row][currentCol] = 0;
                    moved = true;
                }
                currentCol--;
            }
        }
    }
    return moved;
}

bool right() {
    bool moved = false;
    bool merged[4][4] = {};
    for (int row = 0; row < 4; ++row) {
        for (int col = 2; col >= 0; --col) {
            if (board[row][col] == 0) continue;
            int currentCol = col;
            while (currentCol < 3 && (board[row][currentCol + 1] == 0 ||
                (board[row][currentCol + 1] == board[row][currentCol] && !merged[row][currentCol + 1]))) {
                if (board[row][currentCol + 1] == board[row][currentCol]) {
                    board[row][currentCol + 1] *= 2;
                    board[row][currentCol] = 0;
                    merged[row][currentCol + 1] = true;
                    moved = true;
                    break;
                }
                else if (board[row][currentCol + 1] == 0) {
                    board[row][currentCol + 1] = board[row][currentCol];
                    board[row][currentCol] = 0;
                    moved = true;
                }
                currentCol++;
            }
        }
    }
    return moved;

}

void addRandomTile() {
    int emptyCount = 0;

    // First pass: count the number of empty spots
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (board[i][j] == 0) {
                ++emptyCount;
            }
        }
    }

    if (emptyCount == 0) return; // Board is full

    // Generate a random position for the new tile
    int tilePos = rand() % emptyCount;
    int tileValue = 0;
    int chnace = rand() % 10 + 1;
    if (chnace < 2) {
        tileValue = 4;
    }
    else {
        tileValue = 2;
    }

    // Second pass: place the new tile in the randomly chosen empty spot
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (board[i][j] == 0) {
                if (tilePos == 0) { // This is the chosen spot
                    board[i][j] = tileValue;
                    return; // Tile placed, exit the function
                }
                --tilePos; // Not yet at the chosen empty spot
            }
        }
    }
}

int main() {

    int biggest = 0; // Biggest number on board

    // Initialize board for the start of game. This chooses 2 random spots on the board to put a 2 or 4.
    srand(static_cast<unsigned int>(time(0))); // Seed for random generation

    int randRow = rand() % 4;
    int randCol = rand() % 4;
    int randRow2 = rand() % 4;
    int randCol2 = rand() % 4;

    while ((randRow == randRow2) && (randCol == randCol2)) {
        int randRow2 = rand() % 4;
        int randCol2 = rand() % 4;
    }

    board[randRow][randCol] = 2;
    int chnace = rand() % 10 + 1;
    if (chnace < 2) {
        board[randRow2][randCol2] = 4;
    }
    else {
        board[randRow2][randCol2] = 2;
    }

    // Game logic
    bool playing = true;
    while (playing == true) {
        cout << "--------------------\n| Welcome to 2048! |\n--------------------\n\n";

        cout << "Press arrow keys to move the numbers\n";
        display(board);
        cout << "\n\n";

        char c;
        bool didMove = false;
        while (biggest != 2048) { // Infinite loop
            int c = _getch(); // Read a character
            switch (c) {
            case 0:
            case 224: // Arrow keys are preceded by a 0 or 224
                switch (_getch()) { // Now read the real value
                    case 72: { // up
                        didMove = up();
                        break;
                    }
                    case 80: { // down
                        didMove = down();
                        break;
                    }
                    case 75: { // left
                        didMove = left();
                        break;
                    }
                    case 77: { // right
                        didMove = right();
                        break;
                    }
                }
                break;
            }
            if ((isFull(board) == false) && (didMove == true)) {
                addRandomTile();
            }
            else if ((isFull(board) == false) && (didMove == false)) {
                continue;
            }
            else {
                cout << "\nGame Over\n\n";

                int ans;
                cout << "Enter 1 to play again\nEnter 2 to stop playing\nEnter here: ";
                while (!(cin >> ans) || (ans != 1 && ans != 2)) {
                    // Clear error state if input failed
                    if (cin.fail()) {
                        cin.clear(); // Clear the fail bit
                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore bad input
                    }

                    cout << "Invalid input. Please enter 1 or 2: ";
                }

                if (ans == 1) {
                    cout << "\n\n\n\n\n\n\n\n\n\n";
                    main();
                }
                else { return 0; }
            }
            biggest = determineBiggest(board, biggest);
            cout << biggest << endl;
            display(board);
            cout << "\n\n";

        }
        if (biggest == 2048) {
            cout << "\nYou win!\n\n";
        }
        else {
            cout << "\nYou Lose :(\n\n";
        }

        char choice;
        cout << "Do you want to play again? (y/n): ";
        cin >> choice;

        while (choice != 'Y' && choice != 'y' && choice != 'N' && choice != 'n') {
            cout << "\nError: " << choice << " is an invalid input\n\n";
            cout << "Do you want to play again? (y/n): ";
            cin >> choice;
        }

        if (choice == 'n' || choice == 'N') {
            cout << "\nThanks for playing!\n\n";
            playing = false;
        }
    }
    return 0;
}