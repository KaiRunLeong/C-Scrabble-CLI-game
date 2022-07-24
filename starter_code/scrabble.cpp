#include "LinkedList.h"
#include "Player.h"
#include "utils.h"
#include "Tile.h"
#include "Board.h"


#include <fstream>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <sstream>
#include <stdio.h>
#include <cstring>
#include <algorithm>
#include <chrono>

#define NEW_GAME_OPTION       1
#define LOAD_GAME_OPTION      2
#define CREDITS_OPTION        3
#define EXIT_OPTION           4
#define NUM_OF_PLAYERS        2
#define BINGO_SCORE           50
#define MAX_PASS_COUNT        2
#define MAX_BAGTILE_SIZE      98

#define WELCOME_MESSAGE       "Welcome to Scrabble!\n--------------------"
#define EXIT_MESSAGE          "Goodbye!"
#define INVALID_INPUT_MESSAGE "Invalid input"
#define SAVE_FOLDER_PATH      "saves/"
#define SAVE_FILE_EXTENSION   ".txt"

//TO BE DELETED IF NEEDED
#define SCRABLLETILES_TXT     "ScrabbleTiles.txt"
#define NUMBER_OF_TILE        98
#define MAX_HAND_SIZE         7
#define NUM_OF_VALID_LETTERS  26
#define NUM_OF_VALID_ROWS     22
#define MAX_NUM_OF_PLAYERS    4


// Pre game
void printMainMenu();
void createNewPlayer(Player* player);
void initializePlayerRack(Player* player, LinkedList* bagTiles, std::string bagTilesFile);
void initializeBagOfTiles(LinkedList* bagTiles, std::string bagTilesFile);

// Utils
bool checkPlaceKeyword(std::string playerAction);
bool checkTile(std::string playerAction, LinkedList* playerHand);
bool checkAtKeyword(std::string playerAction);
bool checkValidPosition(std::string playerAction);
bool checkReplaceKeyword(std::string playerAction);
int returnIntColumn(std::string playerAction);
bool stringIsAlphabetic(std::string string);
bool stringIsUppercase(std::string string);
bool readOneTile(std::ifstream& file, Letter* letter, Value* value);
int getNumberOfPlayers(Player** players);
int getNumberOfPlayersFromLoad(std::ifstream& saveFile);

// Gameplay
void saveGame(Player** players, Board* board, LinkedList* bagTiles , std::string& currentPlayerName , std::string& filename);
void loadGame(std::ifstream& saveFile, Board* board, LinkedList* bagTiles, Player** players, int* player_turn, int numberOfPlayers);
void playGame(Board* board, LinkedList* bagTiles, Player** arrayOfPlayers, int* player_turn);
void printGameEndMessage(Player** players);
void cleanUp(Board* board, LinkedList* bagTiles, Player** players, int* player_turn);
void addTileToPlayer(Player* player, LinkedList* bagTiles);
bool checkTileInPlayerHand(Player* player, char letter);

int main(int argc, char**argv){
   int* player_turn;

   //board object
   Board* board;

   //bagTile
   LinkedList* bagTiles;

   // Print welcome message once
   std::cout << WELCOME_MESSAGE << std::endl;

   // Print main menu
   printMainMenu();

   //Ask for menu option
   int input;
   std::cin >> input;
   
   if(input == NEW_GAME_OPTION){   //Start new game

      /*
      ----------------------------------------------------
      Initialization of a new game

      INITIALIZE: player object 2x

      ----------------------------------------------------
      */
      int numberOfPlayers;
      std::cout << "ENTER NUMBER OF PLAYERS PLAYING (UP TO 4): ";
      std::cin >> numberOfPlayers;
      Player* arrayOfPlayers[numberOfPlayers+1]; //Allocate one more element for nullptr

      //Create new Player objects
      for(int i = 0; i < numberOfPlayers; i++){
         Player* player = new Player(i);
         arrayOfPlayers[i] = player;
         if(i < numberOfPlayers){
            arrayOfPlayers[i+1] = nullptr;
         }
      }

      //Initialize board object
      board = new Board();

      //Initialize bagTile
      bagTiles = new LinkedList();

      initializeBagOfTiles(bagTiles, SCRABLLETILES_TXT);// INITIALIZE ALL 98 TILES FIRST

      std::cout << "Starting a New Game\n" << std::endl;

      /*
      ----------------------------------------------------

      1) Print necessary prompt
      2) Ask user for name

      ----------------------------------------------------
      */

      std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n'); //flushes new line out of buffer 

      for(int i = 0; i < numberOfPlayers; i++){
         initializePlayerRack(arrayOfPlayers[i], bagTiles, SCRABLLETILES_TXT); //INITIALIZE PLAYERS TILE RACK (HAND)
         createNewPlayer(arrayOfPlayers[i]);
      }

      player_turn = new int(0);

      playGame(board, bagTiles, arrayOfPlayers, player_turn);

   }else if(input == LOAD_GAME_OPTION){

      std::string filename;

      std::cout << "\nEnter the filename from which to load a game" << std::endl;
      std::cout << "> ";
      std::cin >> filename;

      filename = SAVE_FOLDER_PATH + filename + SAVE_FILE_EXTENSION;

      std::ifstream file(filename);

      if(file.good()){
         if(!(file.peek() == std::ifstream::traits_type::eof())){
            std::cout << "\nScrabble game successfully loaded\n" << std::endl;

            board = new Board();
            bagTiles = new LinkedList();

            // get number of players from file
            int numberOfPlayers = getNumberOfPlayersFromLoad(file);
            Player* arrayOfPlayers[numberOfPlayers+1];

            for(int i = 0; i < numberOfPlayers; i++){
               Player* player = new Player(i);
               arrayOfPlayers[i] = player;
               if(i < numberOfPlayers){
                  arrayOfPlayers[i+1] = nullptr;
               }
            }
            
            player_turn = new int(0);

            loadGame(file, board, bagTiles, arrayOfPlayers, player_turn, numberOfPlayers);
            playGame(board, bagTiles, arrayOfPlayers, player_turn);       

         }
      }
      else
         std::cout << "Save file does not exist" << std::endl;


   }else if(input == CREDITS_OPTION){
      std::cout   << "\nName: Kai Run Leong\n"
                  << "Student ID: s3862092\n"
                  << "Email: s3862092@student.rmit.edu.au\n\n"
                  << "Name: Sarah Bylmakers\n"
                  << "Student ID: s3736964\n"
                  << "Email: s3736964@student.rmit.edu.au\n\n"
                  << "Name: Tung Tran\n"
                  << "Student ID: s3275175\n"
                  << "Email: s3275175@student.rmit.edu.au\n\n"
                  << "Name: Abdirashid Haji\n" 
                  << "Student ID: s3742348\n"
                  << "Email: s3742348@student.rmit.edu.au"
                  << std::endl << std::endl;
         
   }else if(input == EXIT_OPTION){

      std::cout << EXIT_MESSAGE << std::endl;
         
   }

   return EXIT_SUCCESS;
}

void cleanUp(Board* board, LinkedList* bagTiles, Player** arrayOfPlayers, int* player_turn) {

   delete bagTiles;
   delete board;

   for(int i = 0; i < NUM_OF_PLAYERS; i++)
      delete arrayOfPlayers[i];

   delete player_turn;
}

int getNumberOfPlayers(Player** players){
   int numberOfPlayers = 0;

   for(int i = 0; i < MAX_NUM_OF_PLAYERS && players[i] != nullptr; i++){
      if(players[i] != nullptr){
         numberOfPlayers++;
      }
   }
   return numberOfPlayers;
}

int getNumberOfPlayersFromLoad(std::ifstream& saveFile){
   std::string line;
   bool reachedBoardSection = false;
   int numberOfPlayers = 0;

   if(saveFile.is_open()){
      for(int i = 0; i < MAX_NUM_OF_PLAYERS && reachedBoardSection == false; i++){
         std::getline(saveFile, line);
         if(line.substr(0,4) == "    "){
            reachedBoardSection = true;
         }else{
            std::getline(saveFile, line); //SKIP SCORE
            std::getline(saveFile, line); //SKIP HAND
            numberOfPlayers++;
         }
         
      }
   }
   saveFile.clear();
   saveFile.seekg(0);
   return numberOfPlayers;
}

void playGame(Board* board, LinkedList* bagTiles, Player** arrayOfPlayers, int* player_turn){

   std::cin.ignore (std::numeric_limits<std::streamsize>::max(), '\n'); //flushes new line out of buffer 
   
   std::cout << "Let's Play!\n" << std::endl;

   /*
      ----------------------------------------------------
      Start of new game (Keep repeating until quit)

      INITIALIZE: board object

      For each round: 1) Show the name of current player
                        2) Scores of both players
                        3) State of the board
                        4) Tiles in player's hand
                        5) User prompt

      ----------------------------------------------------
   */
   //As long as the player does not quit, program will keep running
   bool quit = false;

   // Checking for last tile placement
   bool lastUpdate = false;

   // INITIAL PRINT OF BOARD STATE AND PLAYER INFO

   // (1) SHOW THE NAME OF CURRENT PLAYER
   std::cout << arrayOfPlayers[*player_turn]->getName() << ", " << "it's your turn" << std::endl;

   int numberOfPlayers = getNumberOfPlayers(arrayOfPlayers);  
   // (2) SHOW SCORES OF BOTH PLAYERS
   for(int i = 0; i < numberOfPlayers; i++){
   std::cout << "Score for " << arrayOfPlayers[i]->getName() << ": " << arrayOfPlayers[i]->getScore() << std::endl;
   }

   // (3) STATE OF THE BOARD
   board->printBoard(std::cout, true);

   // (4) SHOW TILES IN PLAYER"S HAND
   std::cout << "\nYour hand is" << std::endl;

   for(int i = 0; i < arrayOfPlayers[*player_turn]->getHand()->size(); i++){
      std::cout << ESC << WHITE_TEXT << ";" << RED_BACKGROUND << "m";
      std::cout << arrayOfPlayers[*player_turn]->getHand()->get(i)->getLetter() << "-" << arrayOfPlayers[*player_turn]->getHand()->get(i)->getValue();
      if(i < arrayOfPlayers[*player_turn]->getHand()->size() - 1){
         std::cout << RESET;
         std::cout << ", ";
      }
   }
   std::cout << RESET;
   while(quit == false && !std::cin.eof()){

      // tracking previous tile placement positions
      // initially set to invalid number npos
      size_t prevRow = std::string::npos;
      size_t prevCol = std::string::npos;

      // (5) User Prompt
      std::string playerAction;
      int tileCount = 0;
      bool invalidInput = false;
      std::cout << std::endl;

      //Player to play tile(s) of the board until place Done
      while (playerAction != "place Done" && tileCount != MAX_HAND_SIZE && !std::cin.eof() && quit == false) {
         
         if(invalidInput){
            std::cout << INVALID_INPUT_MESSAGE << std::endl;
            invalidInput = false;
         }
         std::cout << "> ";
         std::getline(std::cin, playerAction);
         
         if(checkPlaceKeyword(playerAction) == false && // Ensure place command is used
            checkTile(playerAction, arrayOfPlayers[*player_turn]->getHand()) == true &&
            checkAtKeyword(playerAction) == false &&
            playerAction.substr(11, playerAction.length() - 1).length() > 1 && // Ensure that the player enters a position. E.G (G5)
            isalpha(playerAction[11]) &&  // Ensures that the player doesnt enter an invalid row
            isdigit(playerAction[12])){ // Ensure that a player doesnt enter a invalid col
            
            // variables used to hold player input
            char letterChosen = playerAction[6];
            char positionLetter = playerAction[11];
            int position = returnIntColumn(playerAction);
            
            //get Tile chosen
            LinkedList* playerHand = arrayOfPlayers[*player_turn]->getHand();
            bool stop = false;
            int score = arrayOfPlayers[*player_turn]->getScore();
            std::vector<std::vector<Tile*>> currentBoard = board->getBoard();

            // store the max char col that a player can choose
            char maxRowChar = indexToCharRow((int)currentBoard.size() - 1);

            // start of loop to get and place tile
            for(int i = 0; i < playerHand->size() && stop == false; i++){
               if(playerHand->get(i)->getLetter() == letterChosen){
                  // Ensure that there is no tile in the position chosen
                  if(position < (int)currentBoard.at(0).size() && // Ensures that the player doesnt enter a col greater than board size
                     positionLetter <= maxRowChar && // Ensure that the player doesnt enter a row greater than board size
                     currentBoard[charRowToIndex(positionLetter)][position] == nullptr){ // Ensure that the position chosen has no other tile on it
                     // Ensure that the move is legal
                     if(legalMove(prevCol, prevRow, charRowToIndex(positionLetter),position,board)){
                        Tile* tileToPlace = new Tile(*playerHand->get(i));
                        board->placeTile(tileToPlace, positionLetter, position);
                        tileCount++;
                        score += playerHand->get(i)->getValue();
                        arrayOfPlayers[*player_turn]->setScore(score);
                        playerHand->remove(i);
                        if(bagTiles->size() != 0)
                           addTileToPlayer(arrayOfPlayers[*player_turn], bagTiles);
                        stop = true;
                        prevRow =  charRowToIndex(positionLetter);
                        prevCol = position;
                     }
                     else
                        invalidInput = true;
                  }
                  else
                     invalidInput = true;
               }
            }
         }else if(checkReplaceKeyword(playerAction) == false && playerAction.size() < 10 && checkTileInPlayerHand(arrayOfPlayers[*player_turn], playerAction[8]) == true){
            //Find tile from player's hand
            bool stopLoop = false;
            for(int i = 0; i < arrayOfPlayers[*player_turn]->getHand()->size() && stopLoop == false; i++){
               if(playerAction[8] == arrayOfPlayers[*player_turn]->getHand()->get(i)->getLetter()){
                  //Remove tile from player's hand
                  Tile* tileToBeReplaced = new Tile(*arrayOfPlayers[*player_turn]->getHand()->get(i));
                  arrayOfPlayers[*player_turn]->getHand()->remove(i);
                  //Placed removed tile into bag of tiles
                  bagTiles->addBack(tileToBeReplaced);
                  //get random tile from bag of tiles
                  addTileToPlayer(arrayOfPlayers[*player_turn], bagTiles);
                  stopLoop = true;
                  // next players turn.
                  playerAction = "place Done";
               }
            }
         }else if(playerAction.substr(0, 5) == "save "){ // Player can choose to save game
            if(playerAction.substr(5, playerAction.length() - 1) != ""){
               std::string file = playerAction.substr(5, playerAction.length() - 1);
               std::string name = arrayOfPlayers[*player_turn]->getName();
               saveGame(arrayOfPlayers,board, bagTiles,name,file);
               std::cout << "\nGame successfully saved\n" << std::endl;
            }
            else
               invalidInput = true;
         }else if(playerAction == "pass"){ //a player can pass on their turn
            playerAction = "place Done";
            arrayOfPlayers[*player_turn]->incrementPassCount();
            if(arrayOfPlayers[*player_turn]->getPassCount() == MAX_PASS_COUNT)
               quit = true;
         }
         else {
            if(!std::cin.eof())
               invalidInput = true;
            else
               quit = true;
         }

         // Ensuring game ends when a players hand is empty
         if(arrayOfPlayers[*player_turn]->getHand()->size() == 0){
            quit = true;
            lastUpdate = true;
         }

      }

      if(tileCount == MAX_HAND_SIZE){
         std::cout << "\nBINGO!!!\n" << std::endl;
         int score = arrayOfPlayers[*player_turn]->getScore();
         score += BINGO_SCORE;
         arrayOfPlayers[*player_turn]->setScore(score);
         tileCount = 0;
      }
      
      if(*player_turn < getNumberOfPlayers(arrayOfPlayers) - 1){
         *player_turn += 1;
      }else{
         *player_turn = 0;
      }

      if(quit == false || lastUpdate == true){

         // LOOPING PRINT OF BOARD AND PLAYER INFO

         // (1) SHOW THE NAME OF CURRENT PLAYER
         std::cout << arrayOfPlayers[*player_turn]->getName() << ", " << "it's your turn" << std::endl;
         
         // (2) SHOW SCORES OF BOTH PLAYERS
         for(int i = 0; i < getNumberOfPlayers(arrayOfPlayers); i++){
         std::cout << "Score for " << arrayOfPlayers[i]->getName() << ": " << arrayOfPlayers[i]->getScore() << std::endl;
         }

         // (3) STATE OF THE BOARD
         board->printBoard(std::cout, true);

         // (4) SHOW TILES IN PLAYER"S HAND
         std::cout << "\nYour hand is" << std::endl;
         for(int i = 0; i < arrayOfPlayers[*player_turn]->getHand()->size(); i++){
            std::cout << ESC << WHITE_TEXT << ";" << RED_BACKGROUND << "m";
            std::cout << arrayOfPlayers[*player_turn]->getHand()->get(i)->getLetter() << "-" << arrayOfPlayers[*player_turn]->getHand()->get(i)->getValue();
            if(i < arrayOfPlayers[*player_turn]->getHand()->size() - 1){
               std::cout << RESET;
               std::cout << ", ";
            }
            std::cout << RESET;
         }
      }
   }

   printGameEndMessage(arrayOfPlayers);

   cleanUp(board, bagTiles, arrayOfPlayers, player_turn);
}

bool checkTileInPlayerHand(Player* player, char letter){
   bool exist = false;
   for(int i = 0; i < player->getHand()->size() && exist == false; i++){
      if(player->getHand()->get(i)->getLetter() == letter){
         exist = true;
      }
   }
   return exist;
}

int returnIntColumn(std::string playerAction){
   std::string str_number;
   int number;
   if(playerAction[13] != '\0'){
      str_number = playerAction.substr(12,13);
      number = stoi(str_number);
   }else{
      str_number = playerAction.substr(12,12);
      number = stoi(str_number);
   }
   return number;
}

bool checkValidPosition(std::string playerAction){
   bool found = false;
   bool valid = false;
   int numOfValidRows = 15;
   char validRows[15] = {'A','B','C','D','E','F','G','H', 'I','J','K', 'L','M','N','O'};

   for(int i = 0; i < numOfValidRows && found == false; i++){
      if(validRows[i] == playerAction[11]){
         found = true;
      }
   }
   
   std::string str_number;
   int number;
   if(playerAction[13] != '\0'){
      str_number = playerAction.substr(12,13);
      number = stoi(str_number);
      if(number > 9 && number < 15){
         valid = true;
      }
   }else{
      str_number = playerAction.substr(12,12);
      number = stoi(str_number);
      if(number > -1 && number < 10){
         valid = true;
      }
   }
   return valid;
}

// Function to check for replace key word
bool checkReplaceKeyword(std::string playerAction){
   bool error = false;
   char placeArray[8] = {'r','e','p', 'l', 'a', 'c', 'e', ' '};
        
   for(int i = 0; i < 8 && error == false; i++){
      if(playerAction[i] != placeArray[i]){
         error = true;
      }
   }
   return error;
}

// Function to check for the place key word
bool checkPlaceKeyword(std::string playerAction){
   bool error = false;
   char placeArray[6] = {'p', 'l', 'a', 'c', 'e', ' '};
        
   for(int i = 0; i < 6 && error == false; i++){
      if(playerAction[i] != placeArray[i]){
         error = true;
      }
   }
   return error;
}

bool checkTile(std::string playerAction, LinkedList* playerHand){
   bool foundTile = false;
   int numOfTileInHand = playerHand->size(); 
   char availableTilesInHand[numOfTileInHand];


   for(int i = 0; i < numOfTileInHand; i++){
      availableTilesInHand[i] = playerHand->get(i)->getLetter();
   }

   for(int i = 0; i < numOfTileInHand && foundTile == false; i++){
      if(playerAction[6] == availableTilesInHand[i]){
         foundTile = true;
      }
   }
   return foundTile;
}

// Function to check for the 'at' key word
bool checkAtKeyword(std::string playerAction){
   bool error = false;
   char atArray[4] = {' ', 'a', 't', ' '};        
   int secondaryIndex = 7;
   for(int i = 0; secondaryIndex < 11 && error == false; i++){
      if(playerAction[secondaryIndex] != atArray[i]){
         error = true;
      }
      secondaryIndex++;
   }
   return error;
}

void createNewPlayer(Player* player){
   std::string username;
   bool valid = false;

   while(valid == false){
      std::cout << "Enter a new name for player " << player->getPlayerNo() + 1 << " " << " (uppercase characters only)"<<std::endl;
      std::cout << "> ";
      std::cin >> username;
      std::cout << std::endl;
      
      if(stringIsAlphabetic(username) == true && stringIsUppercase(username) == true){
         valid = true;
      }else{
         std::cout << INVALID_INPUT_MESSAGE << std::endl;
      }
   }

   player->setPlayerName(username);
}

bool stringIsAlphabetic(std::string string) {
   bool isAlpha = true;
   int length = string.length();
   for(int i = 0; i < length; i++){
      if(!isalpha(string.at(i)))
         isAlpha = false;
   }

   return isAlpha;
}

bool stringIsUppercase(std::string string) {
   bool isUppercase = true;
   int length = string.length();
   for(int i = 0; i < length; i++){
      if(!isupper(string.at(i)))
         isUppercase = false;
   }

   return isUppercase;
}

void initializeBagOfTiles(LinkedList* bagTiles, std::string bagTilesFile){
   //Open the file to read in the tiles
   std::ifstream file(bagTilesFile);
   std::vector<Tile*> tiles;

   if(file.is_open()){

      //Loop to read in the set number of 98 tiles until EOF
      int numRead = 0;
      while(!file.eof() && numRead < MAX_BAGTILE_SIZE) {
         Letter letter = ' ';
         Value value = 0;
         bool readSuccess = false;
         readSuccess = readOneTile(file, &letter, &value);

         if(!file.eof() && readSuccess) {
            //Add the tile into the BagTiles linkedList
            Tile* tile = new Tile(letter, value);
            tiles.push_back(tile);
         }
      }

      // obtain a time-based seed:
      unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

      // randomize bagTiles from begin to end taking in a function to
      // generate random number

      std::shuffle(tiles.begin(), tiles.end(), std::default_random_engine(seed));
      for (int i = 0; i < (int)tiles.size() ; ++i)
         bagTiles->addBack(tiles.at(i));

      file.close();
   }
}

void addTileToPlayer(Player* player, LinkedList* bagTiles){
   Tile* chosen = new Tile(*bagTiles->get(0));
   player->addToHand(chosen);
   bagTiles->removeFront();
}

void initializePlayerRack(Player* player, LinkedList* bagTiles, std::string bagTilesFile){
   //Open the file to read in the tiles
   std::ifstream file(bagTilesFile);

   if(file.is_open()) {
   
      if(bagTiles->size() != 0){

         for (int i = 0; i < MAX_HAND_SIZE; i++) {
            Tile* chosen = new Tile(*bagTiles->get(0)); 
            player->addToHand(chosen);
            bagTiles->removeFront();
         }

      }

      file.close();
   }
}

void printMainMenu() {
   std::cout   << "Menu\n----\n"
               << "1. New Game\n"
               << "2. Load Game\n"
               << "3. Credits (Show student information)\n"
               << "4. Quit\n"
               << std::endl;
}

void saveGame(Player** players, Board* board, LinkedList* bagTiles , std::string& currentPlayerName , std::string& filename){

   // create write stream for given filename with folder path and extension
   std::ofstream saveFile(SAVE_FOLDER_PATH + filename + SAVE_FILE_EXTENSION);

   // Check if file can be opened
   if(saveFile.is_open()){
      /*
      ----------------------------------------------------
      Loop to write information of each player

      For each loop:    1) write player name
                        2) write player score
                        3) write player hand

      ----------------------------------------------------
      */
      for(int i = 0; i < getNumberOfPlayers(players); i++){
         saveFile << players[i]->getName() << std::endl;
         saveFile << players[i]->getScore() << std::endl;
         for (int j = 0; j < players[i]->getHand()->size(); j++){
            saveFile << players[i]->getHand()->get(j)->getLetter() << "-" << players[i]->getHand()->get(j)->getValue();
            if(j < players[i]->getHand()->size() - 1)
               saveFile << ", ";
         }
         saveFile << std::endl;
      }

      // prints the board to the save file stream
      board->printBoard(saveFile, false);

      /*
      ----------------------------------------------------
      Loop to write information of Tiles in the bag

      For each loop:    1) write tile letter
                        2) write tile value

      ----------------------------------------------------
      */
      
      for(int i = 0; i < bagTiles->size(); i++){
         saveFile << bagTiles->get(i)->getLetter() << "-" << bagTiles->get(i)->getValue();
         if(i < bagTiles->size() - 1)
            saveFile << ", ";
      }

      saveFile << std::endl;

      // write the current players name
      saveFile << currentPlayerName;

      saveFile.close();
   }
}

void loadGame(std::ifstream& saveFile, Board* board, LinkedList* bagTiles, Player** players, int* player_turn, int numberOfPlayers){

   std::string line;

   if(saveFile.is_open()){

      // Getting player information section

      /*
      ----------------------------------------------------
      Loop to read each players information and set them to the
      data structure provided

      For each loop:    1) read player name from file
                           1.1) set player name
                        2) read player score from file
                           2.1) set player score
                        3) read player hand from file
                           3.1) split string of tiles
                           3.2) split letter and value of tile
                           3.3) set player hand from newly created tile

      ----------------------------------------------------
      */

      for(int i = 0; i < numberOfPlayers; i++){
         // reading and setting player name and score
         std::getline(saveFile, line);
         players[i]->setPlayerName(line);

         std::getline(saveFile, line);
         int score = stoi(line);
         
         players[i]->setScore(score);
         // setting playerNo
         players[i]->setPlayerNo(i);

         std::getline(saveFile, line);

         // variable to help split each tile using delimiter ', '
         std::string delimiter = ", ";
         size_t pos = 0;
         char letter = '\0';
         int value = 0;
         std::string segment;
         std::vector<std::string> vecTile;

         // Loop to split string of tiles and add them to the players hand
         while ((pos = line.find(delimiter)) != std::string::npos) {

            std::stringstream ssTile(line.substr(0, pos));

            while(std::getline(ssTile,segment, '-'))
               vecTile.push_back(segment);
            
            letter = vecTile.front().front();
            value = stoi(vecTile.back());

            Tile* tile = new Tile(letter, value);
            players[i]->addToHand(tile);
            line.erase(0, pos + delimiter.length());
            vecTile.clear();
         }

         letter = line.front();
         // char to int
         value = line.back() - '0';
         Tile* lastTile = new Tile(letter, value);
         players[i]->addToHand(lastTile);
      }
     
      // Getting board information section
      std::getline(saveFile, line);

      // Go to the next line until first char row of board is found
      while((line.front() != 'A')){
         std::getline(saveFile, line);
      }

      std::string row;

      // Getting col and row length from board vector size
      int colBoardLength = board->getBoard().at(0).size();
      int rowBoardLength = board->getBoard().size();

      // for each loop to get each placed letter on the board in the file
      // and adds it to the passed in board.
      for(int i = 0; i < rowBoardLength; i++){
         std::stringstream ssLine(line);
         std::getline(ssLine,row, '|');
         for(int x = 0; x < colBoardLength; x++){
            std::getline(ssLine,row, '|');
            int rowLength = row.length();
            for(int y = 0; y < rowLength; y++){
               if(!isspace(row[y]) && y != 0) {
                  Tile* tile = new Tile(row[y],LETTER_VALUES.at(row[y]));
                  board->placeTile(tile,line.front(),x);
               }
            }
         }
         std::getline(saveFile, line);
      }

      // Getting bagTile information

      std::string delimiter = ", ";
      size_t pos = 0;
      char letter = '\0';
      int value = 0;
      std::string segment;
      std::vector<std::string> vecTile;

      // for each loop it gets a tile from the line of tiles
      // and adds it to the bag tiles linked list
      while ((pos = line.find(delimiter)) != std::string::npos) {

         std::stringstream ssTile(line.substr(0, pos));

         while(std::getline(ssTile,segment, '-'))
            vecTile.push_back(segment);
         
         letter = vecTile.front().front();
         value = stoi(vecTile.back());

         Tile* tile = new Tile(letter, value);
         bagTiles->addBack(tile);
         line.erase(0, pos + delimiter.length());
         vecTile.clear();
      }

      letter = line.front();
      // char to int
      value = line.back() - '0';
      Tile* lastTile = new Tile(letter, value);
      bagTiles->addBack(lastTile);

      // Getting playerTurn

      std::getline(saveFile, line);
      
      for(int i = 0; i < numberOfPlayers; i++){
         if(players[i]->getName() == line)
            *player_turn = players[i]->getPlayerNo();
      }
      saveFile.close();
   }

}

void printGameEndMessage(Player** players){

   int highestScore = 0;
   int winnerIndex = 0;

   std::cout << "\nGame over" << std::endl;

   for(int i = 0; i < getNumberOfPlayers(players); i++){
      std::cout   << "Score for player "
                  << players[i]->getName()
                  << ": "
                  <<  players[i]->getScore()
                  << std::endl;
   }

   // Get the winner of the game
   for(int i = 0; i < getNumberOfPlayers(players); i++){
      if(players[i]->getScore() > highestScore){
         highestScore = players[i]->getScore();
         winnerIndex = i;
      }
   }

   std::cout << "Player " << players[winnerIndex]->getName() << " won!\n" << std::endl;
   
   std::cout << EXIT_MESSAGE << std::endl;
}

bool readOneTile(std::ifstream& file, Letter* letter, Value* value) {
   
   //read letter
   Letter readLetter = 'A';
   bool readSuccess = true;

   file >> readLetter;

   if(isalpha(readLetter)){
      *letter = readLetter;
   } else{
      readSuccess = false;
      std::cout << "Incorrect letter!" << std::endl;
   }

   //read value
   file >> *value;

   return readSuccess;
}