#include "utils.h"

#define LAST_ROW_CHAR   'O'
#define NUM_MOVEMENTS   4
#define NUM_SURROUND    2
#define MAX_ALPHA       26


int charRowToIndex(char c) {
    int intRow = -1;

    if(c <= LAST_ROW_CHAR && c != ' ') {
        if(c == 'A')
            intRow = A;
        else if(c == 'B')
            intRow = B;
        else if(c == 'C')
            intRow = C;
        else if(c == 'D')
            intRow = D;
        else if(c == 'E')
            intRow = E;
        else if(c == 'F')
            intRow = F;
        else if(c == 'G')
            intRow = G;
        else if(c == 'H')
            intRow = H;
        else if(c == 'I')
            intRow = I;
        else if(c == 'J')
            intRow = J;
        else if(c == 'K')
            intRow = K;
        else if(c == 'L')
            intRow = L;
        else if(c == 'M')
            intRow = M;
        else if(c == 'N')
            intRow = N;
        else if(c == 'O')
            intRow = O;
    }

    return intRow;
}

char indexToCharRow(int index) {

    // starting row
    char c = 'A';

    if(index < MAX_ALPHA){
        for(int i = 0; i < index; i++){
            c++;
        }
    }

    return c;
}

bool legalMove(size_t prevCol, size_t prevRow, int chosenRow, int chosenCol, Board* board) {

    int colBoardLength = board->getBoard().at(0).size();
    int rowBoardLength = board->getBoard().size();
    
    // Tile can only be placed increasing or decreasing by 1
    // if Tile is in the same row or col there is not movement 0
    int colMovements[NUM_MOVEMENTS] = {-1, 1, 0, 0};
    int rowMovements[NUM_MOVEMENTS] = {0, 0, -1, 1};

    bool legalMove = false;

    // Booleans to individually check if movement is legal
    bool posCheck = false;
    bool connectLetterCheck = false;
    
    // check if there is a previous tile placed
    // we use npos to indicate that there is no prev tile placed and as such
    // any move is legal
    if(prevCol != std::string::npos && prevRow != std::string::npos){

        // Checking for legal x and y movement based on previous placement

        for(int i = 0; i < NUM_MOVEMENTS; i++){
            if(chosenRow == (int)prevRow + rowMovements[i] && chosenCol == (int)prevCol + colMovements[i])
                posCheck = true;
        }
        
        // Checks for connected letters when there is no previous placement

        for(int i = 0; i < NUM_MOVEMENTS; i++){
            int x = chosenRow + rowMovements[i];
            int y = chosenCol + colMovements[i];
            Tile* tile = board->getTile(x,y);
            if(tile != nullptr){
                connectLetterCheck = true;
            }
        }
    }
    else {
        // At this point it is currently a new players turn
        // check if board is empty. if it is then any placement is valid
        if(!board->isEmpty()){

            std::vector<int> tileXpos;
            std::vector<int> tileYpos;

            // Getting the row and col of all tiles on board

            for(int x = 0; x < rowBoardLength; x++){
                for(int y = 0; y < colBoardLength; y++){
                    Tile* tile = board->getTile(x,y);
                    if(tile != nullptr){
                        tileXpos.push_back(x);
                        tileYpos.push_back(y);
                    }
                }
            }

            // checking if tile placed will be in the path of a
            // already placed tile to be connected

            for(int i = 0; i < (int)tileXpos.size(); i++){
                if(chosenRow == tileXpos.at(i) || chosenCol == tileYpos.at(i))
                    connectLetterCheck = true;
            }
        }
        else
            legalMove = true;
    }

    if(connectLetterCheck || posCheck)
        legalMove = true;
            

    return legalMove;

}