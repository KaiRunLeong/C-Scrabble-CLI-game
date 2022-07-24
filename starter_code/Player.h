#ifndef PLAYER
#define PLAYER

#include "LinkedList.h"
#include "Tile.h"
#include "utils.h"

#include <iostream>


#define MAX_HAND_SIZE   7

class Player {
public:
    Player(int playerNo);
    Player(Player& other);
    ~Player();
    
    LinkedList* getHand();

    void addToHand(Tile* tile);

    void setScore(int score);
    void setPlayerNo(int playerNo);
    void setPlayerName(std::string username);
    void incrementPassCount();

    int getPlayerNo();
    std::string getName();
    int getScore();
    int getPassCount();

private:
    int playerNo;
    std::string name;
    int score;
    LinkedList* tileRack;
    int passCount;
};


#endif