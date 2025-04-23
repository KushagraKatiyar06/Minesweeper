//
// Created by kkati on 4/18/2025.
//

#ifndef BOARD_H
#define BOARD_H

#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <random>
#include <vector>
#include <string>
#include <iostream>
#include "Tile.h"

class Board {
private:
    int rows;
    int columns;
    int mines;

    std::vector<std::vector<Tile>> board;
    std::map<std::string, sf::Texture> textures;
    std::vector<std::string> basicTileNames = {
        "mine.png", "tile_hidden.png", "tile_revealed.png", "flag.png", "face_happy.png"
    };

public:
    Board(int rows, int columns, int mines);

    void loadTextures();
    void createBoard();
    void placeMines();
    void setNeighbours();
    void draw(sf::RenderWindow& window);
    void reveal(int row, int col);
    void reset();
    int getFlags() const;
    Tile& getTile(int row, int col);
    sf::Texture& getTexture(const std:: string &name);
};


#endif //BOARD_H
