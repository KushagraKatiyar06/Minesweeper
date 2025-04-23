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
    bool mine;
    bool revealed;
    bool flagged;
    int number_of_mines_nearby;
    int row;
    int column;

    std::vector<Tile*> nearby_tiles;
    sf::Sprite sprite;

public:
    //Constructors
    Tile();

    // Setters
    void reveal();
    void toggleFlag();
    void setMine(bool input);
    void setNearbyMines(int input);
    void setSprite(const sf::Texture& texture);
    void setPosition(float x, float y);
    void addNeighbor(Tile* tile);
    int getRow() const;
    int getColumn() const;

    // Getters
    bool getMine() const;
    bool getRevealed() const;
    bool getFlagged() const;
    int getNearbyMines();
    sf::Sprite& getSprite();
    std::vector<Tile*> getNearbyTiles() const;
};


#endif //TILE_H