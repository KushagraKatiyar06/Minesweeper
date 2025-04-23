//
// Created by kkati on 4/18/2025.
//

#ifndef TILE_H
#define TILE_H

#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Tile {
private:
    bool mine = false;
    bool revealed = false;
    bool flagged = false;
    int number_of_mines_nearby = 0;

    std::vector<Tile*> nearby_tiles;
    sf::Sprite sprite;

public:
    // Setters
    void reveal();
    void toggleFlag();
    void setMine(bool input);
    void setNearbyMines(int input);
    void setSprite(const sf::Texture& texture);
    void setPosition(float x, float y);
    void addNeighbor(Tile* tile);

    // Getters
    sf::Sprite& getSprite();
    bool getMine() const;
    bool getRevealed() const;
    bool getFlagged() const;
    std::vector<Tile*> getNearbyTiles() const;
    int getNearbyMines();
};


#endif //TILE_H
