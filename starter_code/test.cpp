#include "LinkedList.h"
#include <iostream>

#define EXIT_SUCCESS    0

int main(void) {
    LinkedList* list = new LinkedList();
    Tile* tileA1 = new Tile('A', 1);
    Tile* tileB3 = new Tile('B', 1);
    Tile* tileC3 = new Tile('C', 3);
    
    std::cout << tileA1->getLetter() << std::endl;
    std::cout << tileB3->getLetter() << std::endl;
    std::cout << tileC3->getLetter() << std::endl;
    
    std::cout << "-------------------" << std::endl;

    list->addBack(tileA1);
    list->addBack(tileB3);
    list->addFront(tileC3);

    std::cout << "-------------------" << std::endl;
    
    for(int i = 0; i < list->size(); i++){
        std::cout << list->get(i)->getLetter() << std::endl;
    }
   

   delete list;
}