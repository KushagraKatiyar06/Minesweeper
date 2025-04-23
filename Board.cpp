#include "Board.h"

#include <chrono>
#include <iostream>
#include <ctime>

//Constructors
Board::Board(int columns, int rows, int mines) {
    this->columns = columns;
    this->rows = rows;
    this->mines = mines;

    loadTextures();
    createBoard();
    placeMines();
    setNeighbours();

    std::cerr << "Board sucessfully created\n" << std::endl;
}

void Board:: reset() {
    gameOver = false;
    gameWin = false;
    board.clear();

    createBoard();
    placeMines();
    setNeighbours();

    //std:: cerr << "Board sucessfully reset\n" << std::endl;
}

void Board::loadTextures() {
    //Images
    for (std::string& name : fileNames) {
        sf::Texture texture;
        if (!texture.loadFromFile("images/" + name)) {
            std::cerr << "Error loading texture: " << name << std::endl;
        }
        textures[name] = texture;
        std::cerr << "Loaded " << name << "sucessfully" << std::endl;
    }

    std::cerr<<std::endl;

    //Digits
    for (int i = 1; i <= 8; i++) {
        std::string name = "number_" + std::to_string(i) + ".png";
        sf::Texture texture;
        if (!texture.loadFromFile("images/" + name)) {
            std::cerr << "Error loading texture: " << name << std::endl;
        }
        textures[name] = texture;
        std::cerr << "Loaded " << name << "sucessfully" << std::endl;
    }

    std::cerr<<std::endl;
}

void Board::createBoard() {
    std::vector<std::vector<Tile>> buffer;

    for (int i = 0; i < rows; i++) {
        std::vector<Tile> row;
        for (int j = 0; j < columns; j++) {
            Tile tile;
            tile.setPosition(j * 32, i * 32);
            row.push_back(tile);
        }
        buffer.push_back(row);
    }
    board = buffer;
}


void Board::placeMines() {
    std::srand(time(0));
    int totalTiles = rows * columns;
    std::vector<int> positions;

    while (positions.size() < mines) {
        int randomNum = rand() % totalTiles;

        if (std::find(positions.begin(), positions.end(), randomNum) == positions.end()) {
            positions.push_back(randomNum);
        }
    }

    for (int index : positions) {
        int row = index / columns;
        int column = index % columns;
        board[row][column].setMine(true);
    }

}


void Board::setNeighbours() {
    for (int row = 0; row < rows; ++row) {
        for (int column = 0; column < columns; ++column) {

            for (int rowTravel = -1; rowTravel <= 1; rowTravel++) {
                for (int colTravel = -1; colTravel <= 1; colTravel++) {

                    if (rowTravel == 0 && colTravel == 0) {
                        continue;
                    }

                    int neighborRow = row + rowTravel;
                    int neighborCol = column + colTravel;

                    if (neighborRow >= 0 && neighborRow < rows && neighborCol >= 0 && neighborCol < columns) {
                        Tile* neighbor = &board[neighborRow][neighborCol];
                        board[row][column].addNeighbor(neighbor);
                    }
                }
            }
        }
    }

    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            std::vector<Tile*> neighbors = board[row][column].getNearbyTiles();
            int mineCount = 0;

            for (Tile* neighbor : neighbors) {
                if (neighbor->getMine()) {
                    mineCount++;
                }
            }

            board[row][column].setNearbyMines(mineCount);
        }
    }
}


void Board::draw(sf::RenderWindow& window, bool debug) {
    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < columns; column++) {
            Tile& tile = board[row][column];

            if (tile.getRevealed()) {
                if (tile.getMine()) {
                    tile.setSprite(textures["mine.png"]);
                } else {
                    tile.setSprite(textures["tile_revealed.png"]);
                    if (tile.getNearbyMines() > 0) {
                        std::string numTex = "number_" + std::to_string(tile.getNearbyMines()) + ".png";
                        if (textures.count(numTex)) {
                            tile.setSprite(textures[numTex]);
                        }
                    }
                }
            } else {

                if (tile.getFlagged()) {
                    tile.setSprite(textures["flag.png"]);
                } else if (debug && tile.getMine()) {
                    tile.setSprite(textures["mine.png"]);
                } else {
                    tile.setSprite(textures["tile_hidden.png"]);
                }
            }

            window.draw(tile.getSprite());
        }
    }
}


void Board::reveal(int row, int col) {
    if (gameOver) {
        return;
    }

    if (row < 0 || row >= rows || col < 0 || col >= columns) {
        return;
    }

    Tile& tile = board[row][col];
    if (tile.getRevealed() || tile.getFlagged()) {
        return;
    }

    tile.reveal();

    if (tile.getMine()) {
        setGameOver(true);
        for (auto& row : board) {
            for (auto& tile : row) {
                if (tile.getMine()) {
                    tile.reveal();
                }
            }
        }

        return;
    }

    if (tile.getNearbyMines() == 0 && !tile.getMine()) {
        for (Tile* neighbor : tile.getNearbyTiles()) {
            int nRow = neighbor->getSprite().getPosition().y / 32;
            int nCol = neighbor->getSprite().getPosition().x / 32;


            if (!neighbor->getRevealed() && !neighbor->getMine()) {
                reveal(nRow, nCol);
            }
        }
    }

    checkWin();
}

void Board::checkWin() {
    if (gameOver || gameWin) {
        return;
    }

    int revealedCount = 0;
    int totalTiles = rows * columns;
    int totalMines = mines;

    for (const auto& row : board) {
        for (const Tile& tile : row) {
            if (tile.getRevealed()) {
                revealedCount++;
            }
        }
    }

    if (revealedCount == totalTiles - totalMines) {
        gameWin = true;

        for (auto& row : board) {
            for (auto& tile : row) {
                if (tile.getMine()) {
                    tile.reveal();
                }
            }
        }
    }
}

void Board::setPlayerWinTime(int time) {
    playerWinTime = time;
}

int Board::getPlayerWinTime() const {
    return playerWinTime;
}


bool Board::isGameOver() const {
    return gameOver;
}

void Board::setGameOver(bool value) {
    gameOver = value;
}

Tile& Board::getTile(int row, int col) {
    return board[row][col];
}

sf::Texture& Board:: getTexture(const std:: string& name){
  return textures.at(name);
}


int Board:: getFlags() const {
    int count = 0;
    for (auto row : board) {
        for (auto tile : row) {
            if (tile.getFlagged()) {
                count++;
            }
        }
    }
    return count;
}

bool Board::isGameWon() const {
    return gameWin;
}

void Board::setGameWon(bool value) {
    gameWin = value;
}
