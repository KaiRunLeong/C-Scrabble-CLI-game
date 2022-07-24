#include "Player.h"


Player::Player(int playerNo){
    this->playerNo = playerNo;
    this->score = 0;
    this->passCount = 0;
    tileRack = new LinkedList();
}

Player::~Player(){
    delete tileRack;
}

void Player::addToHand(Tile* tile) {
    this->tileRack->addBack(tile);
}

LinkedList* Player::getHand() {
    return tileRack;
}

void Player::setPlayerName(std::string username){
    this->name = username;
}

void Player::setScore(int score){
    this->score = score;
}

void Player::setPlayerNo(int playerNo){
    this->playerNo = playerNo;
}

void Player::incrementPassCount(){
    this->passCount++;
}

std::string Player::getName(){
    return this->name;
}

int Player::getPlayerNo(){
    return this->playerNo;
}

int Player::getScore(){
    return this->score;
}

int Player::getPassCount(){
    return this->passCount;
}