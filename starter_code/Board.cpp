#include "Board.h"
#include "utils.h"

#include <iostream>
#include <iomanip>


#define MAX_COL 15
#define MAX_ROW 15

#define NUM_OF_CHAR 62

Board::Board(){
    board = std::vector<std::vector<Tile*>>(MAX_ROW, std::vector<Tile*>(MAX_COL));

    for(int i = 0; i < MAX_ROW; i++){
        for(int x = 0; x < MAX_COL; x++){
            board[i][x] = nullptr;
        }
    }
}

Board::~Board(){
    for(int i = 0; i < MAX_ROW; i++){
        for(int x = 0; x < MAX_COL; x++){
            delete board[i][x];
            board[i][x] = nullptr;
        }
    }
}

std::vector<std::vector<Tile*>> Board::getBoard(){
    return this->board;
}

void Board::printBoard(std::ostream& ostream, bool color){

    // Rows start at A
    char gridLetter = 'A';

    ostream << "    ";

    for(int i = 0; i < MAX_COL; i++){
        if(i > 9)
            ostream << i << "  ";
        else
            ostream << i << "   " ;
    }

    ostream << "\n  ";
    ostream << std::setfill('-') << std::setw(NUM_OF_CHAR);
    ostream << "\n";

    for(int i = 0; i < MAX_ROW; i++)
    {
        ostream << gridLetter++ << " |";

        for(int j = 0; j < MAX_COL; j++)
        {
            if(color == true){
                if(board[i][j] != nullptr){
                    std::cout << ESC << WHITE_TEXT << ";" << RED_BACKGROUND << "m";
                    ostream  << " " << board[i][j]->getLetter() << " " << RESET <<"|";
                    std::cout << RESET;
                }else{
                    ostream << "   " << "|";
                }
            }else{
                if(board[i][j] != nullptr){                  
                    ostream  << " " << board[i][j]->getLetter() << " " <<"|";
                }else{
                    ostream << "   " << "|";
                }
            }
        }
        ostream << std::endl;
    }
}

bool Board::placeTile(Tile* tile, char row, int col){
    bool success = true;

    if(charRowToIndex(row) != -1)
        board[charRowToIndex(row)][col] = tile;
    else
        success = false;

    return success;
}

bool Board::isEmpty(){
    bool isEmpty = true;
    
    for(int i = 0; i < MAX_ROW; i++){
        for(int x = 0; x < MAX_COL; x++){
            if(board[i][x] != nullptr)
                isEmpty = false;
        }
    }

    return isEmpty;
}

Tile* Board::getTile(int row, int col){
    Tile* tile = nullptr;

    if((row < MAX_ROW && row >= 0) && (col < MAX_COL && col >= 0)){
        if(board[row][col] != nullptr)
            tile = board[row][col];
    }

    return tile;
}