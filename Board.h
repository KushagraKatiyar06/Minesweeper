//
// Created by kkati on 4/18/2025.
//

#ifndef BOARD_H
#define BOARD_H

#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <vector>
#include <string>
#include "Tile.h"

class Board {
private:
    int rows;
    int columns;
    int mines;
    bool gameOver = false;
    bool gameWin = false;
    int playerWinTime = -1;

    std::vector<std::vector<Tile>> board;
    std::map<std::string, sf::Texture> textures;
    std::vector<std::string> fileNames = {
        "mine.png", "tile_hidden.png", "tile_revealed.png",
        "flag.png", "face_happy.png", "face_lose.png",
        "digits.png", "face_win.png", "debug.png",
        "pause.png", "play.png", "leaderboard.png"
    };

public:
    Board(int columns, int rows, int mines);

    void loadTextures();
    void createBoard();
    void placeMines();
    void revealAllMines();
    void setNeighbours();
    void draw(sf::RenderWindow& window, bool debug);
    void reveal(int row, int col);
    void reset();
    int getFlags() const;
    bool isGameOver() const;
    void setGameOver(bool value);
    Tile& getTile(int row, int col);
    sf::Texture& getTexture(const std:: string &name);
    bool isGameWon() const;
    void setGameWon(bool value);
    void checkWin();
    int getPlayerWinTime() const;
    void setPlayerWinTime(int time);



};


#endif //BOARD_H