
#include "Tile.h"

Tile::Tile(Letter _letter, Value _value)
{
    this->letter = _letter;
    this->value = _value;
}

Tile::Tile(Tile& other) {
    this->letter = other.letter;
    this->value = other.value;
}

Tile::~Tile() {
}

Letter Tile::getLetter() {
    return letter;
}

Value Tile::getValue() {
    return value;
}
