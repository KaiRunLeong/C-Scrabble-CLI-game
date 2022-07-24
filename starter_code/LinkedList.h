
#ifndef ASSIGN2_LINKEDLIST_H
#define ASSIGN2_LINKEDLIST_H

#include "Node.h"

class LinkedList {
public:

   LinkedList();
   ~LinkedList();
   LinkedList(LinkedList& other);

   int size();

   Tile* get(int index);

   //Add tile to the end of the list
   void addBack(Tile* letter);

   //Add tile to the front of the list
   void addFront(Tile* letter);

   void removeBack();
   void removeFront();
   void remove(int index);

   void clear();

private:
   Node* head;
};

#endif // ASSIGN2_LINKEDLIST_H
