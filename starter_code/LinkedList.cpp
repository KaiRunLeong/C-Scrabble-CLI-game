#include <iostream>
#include "LinkedList.h"
#include <stdexcept>

LinkedList::LinkedList() {
   this->head = nullptr;
}

LinkedList::~LinkedList() {
   clear();
}

LinkedList::LinkedList(LinkedList& other){
   
   head = nullptr;

   int otherSize = other.size();

   for(int i =0; i < otherSize; i++){
      Tile* tile = new Tile(*other.get(i));
      addBack(tile);
   }
}

// For Doubly linked list
// ENSURE THAT ALL UNUSED NODE HAS BEEN FREED FROM MEMORY
int LinkedList::size(){
   int length = 0;

   Node* current = head;
   while(current != nullptr) {
      length++;
      current = current->next;
   }
   return length;
}

Tile* LinkedList::get(int index){
   Node* currNode = this->head;
   Tile* returnTile = nullptr;

   try{
      for(int i = 0; i <= index; i++){
         if(currNode != nullptr && i <= index){
            returnTile = currNode->tile;

            // This if statement will prevent memory fault from occuring
            // as it ensures that the currNode never points to a nullptr
            if(currNode->next != nullptr || ((i == index) && (currNode->next == nullptr))){
               currNode = currNode->next;
            }else{
               throw(currNode);
            }
         }
      }
    }catch(Node* node){
      throw std::out_of_range ("LINKED LIST: OUT OF RANGE");
    }
    return returnTile;
}


void LinkedList::addBack(Tile* tile){
   Node* newNode = new Node(tile, nullptr, nullptr);

   if(this->head == nullptr){
        this->head = newNode;
   }else{
      Node* current = this->head;
         while(current->next != nullptr) {
            current = current->next;
         }
         current->next = newNode;
         current->next->prev = current;
      
   }
}

void LinkedList::addFront(Tile* tile){
   Node* node = new Node(tile, nullptr, nullptr);
   if(this->head == nullptr){
        this->head = node;
    }else{
        Node* tempNode = this->head;
        this->head = node;
        node->next = tempNode;
        tempNode->prev = this->head;
    }

}

void LinkedList::remove(int index){
   if(index == 0){
      Node* tempNode = this->head->next;
      delete this->head;
      this->head = tempNode;
      if(size() > 1)
         this->head->prev = nullptr;
   }else{
      Node* nodeBeforePosition = this->head;
      Node* toDelete;
      for(int i = 0; i < index-1; i++){
         nodeBeforePosition = nodeBeforePosition->next;
      }
      toDelete = nodeBeforePosition->next;
      nodeBeforePosition->next = nodeBeforePosition->next->next;
      if(nodeBeforePosition->next != nullptr)
         nodeBeforePosition->next->prev = nodeBeforePosition;
      delete toDelete;
   }
}

void LinkedList::removeBack(){
   if(this->head != nullptr){
      Node* currNode = this->head;
      Node* prevs = nullptr;

      while(currNode->next != nullptr){
         prevs = currNode;
         currNode = currNode->next;
      }

      if(prevs == nullptr){
         head = nullptr;
      }else{
         prevs->next = nullptr;
      }

      delete currNode;
      currNode = nullptr;
   }
}

void LinkedList::removeFront(){
   if(this->head != nullptr){
      if(size() == 1){
         delete this->head;
         this->head = nullptr;
      }else{
         Node* tempNode = this->head->next;
         delete this->head;
         this->head = tempNode;
         this->head->prev = nullptr;
      }
   }
}

void LinkedList::clear() {
   while(this->head != nullptr){
      removeFront();
   }
}