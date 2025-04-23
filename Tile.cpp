#include "Tile.h"

// Setters
void Tile::reveal() {
    revealed = true;
}

void Tile::toggleFlag() {
    flagged = !flagged;
}

void Tile::setMine(bool input) {
    mine = input;
}

void Tile::setNearbyMines(int input) {
    number_of_mines_nearby = input;
}

void Tile::setSprite(const sf::Texture& texture) {
    sprite.setTexture(texture);
}

void Tile::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

void Tile::addNeighbor(Tile* tile) {
    nearby_tiles.push_back(tile);
}

// Getters
sf::Sprite& Tile::getSprite() {
    return sprite;
}

bool Tile::getMine() const {
    return mine;
}

bool Tile::getRevealed() const {
    return revealed;
}

bool Tile::getFlagged() const {
    return flagged;
}

std::vector<Tile*> Tile::getNearbyTiles() const {
    return nearby_tiles;
}

int Tile::getNearbyMines() {
    return number_of_mines_nearby;
}
