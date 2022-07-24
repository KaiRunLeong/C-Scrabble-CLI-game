#ifndef BOARD
#define BOARD

#define ESC "\033["
#define WHITE_TEXT "0"
#define RED_BACKGROUND "41"
#define RESET "\33[m"

#include "Tile.h"
#include <vector>
#include <iostream>

enum rowLetter {
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O
};

class Board {
public:
    Board();
    ~Board();

    void printBoard(std::ostream& ostream, bool color);

    bool placeTile(Tile* tile, char row, int col);

    // Checks if board is empty
    bool isEmpty();

    Tile* getTile(int row, int col);

    std::vector<std::vector<Tile*>> getBoard();
    
private:
    std::vector<std::vector<Tile*>> board;

};

#endif