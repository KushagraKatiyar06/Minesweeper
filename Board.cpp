#include "Board.h"
#include <iostream>
#include <ctime>

Board::Board(int rows, int columns, int mines) {
    this->rows = rows;
    this->columns = columns;
    this->mines = mines;

    loadTextures();
    createBoard();
    placeMines();
    setNeighbours();
}

void Board::loadTextures() {
    for (const std::string& name : basicTileNames) {
        sf::Texture tex;
        if (!tex.loadFromFile("images/" + name)) {
            std::cerr << "Error loading texture: " << name << std::endl;
        }
        textures[name] = tex;
    }

    for (int i = 1; i <= 8; i++) {
        std::string name = "number_" + std::to_string(i) + ".png";
        sf::Texture tex;
        if (!tex.loadFromFile("images/" + name)) {
            std::cerr << "Error loading texture: " << name << std::endl;
        }
        textures[name] = tex;
    }
}

void Board::createBoard() {
    board.resize(rows, std::vector<Tile>(columns));
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            board[i][j].setPosition(j * 32, i * 32);
        }
    }
}

void Board::placeMines() {
    std::srand(time(0));
    int placed = 0;
    while (placed < mines) {
        int r = rand() % rows;
        int c = rand() % columns;
        if (!board[r][c].getMine()) {
            board[r][c].setMine(true);
            placed++;
        }
    }
}

void Board::setNeighbours() {
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < columns; ++col) {
            for (int dRow = -1; dRow <= 1; ++dRow) {
                for (int dCol = -1; dCol <= 1; ++dCol) {
                    if (dRow == 0 && dCol == 0) continue;
                    int nRow = row + dRow;
                    int nCol = col + dCol;
                    if (nRow >= 0 && nRow < rows && nCol >= 0 && nCol < columns) {
                        board[row][col].addNeighbor(&board[nRow][nCol]);
                    }
                }
            }
        }
    }

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < columns; ++col) {
            int count = 0;
            for (Tile* neighbor : board[row][col].getNearbyTiles()) {
                if (neighbor->getMine()) count++;
            }
            board[row][col].setNearbyMines(count);
        }
    }
}

void Board::draw(sf::RenderWindow& window) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            Tile& tile = board[i][j];
            if (tile.getRevealed() == true) {
                tile.setSprite(textures["tile_revealed.png"]);
                if (tile.getMine() == true) {
                    tile.setSprite(textures["mine.png"]);
                } else if (tile.getNearbyTiles().size() > 0) {
                    std::string numberTexture = "number_" + std::to_string(tile.getNearbyMines()) + ".png";
                    if (textures.count(numberTexture) == true) {
                        tile.setSprite(textures[numberTexture]);
                    }
                }
            } else {
                tile.setSprite(textures["tile_hidden.png"]);
                if (tile.getFlagged()) {
                    tile.setSprite(textures["flag.png"]);
                }
            }
            window.draw(tile.getSprite());
        }
    }
}

void Board::reveal(int row, int col) {
    if (row < 0 || row >= rows || col < 0 || col >= columns) return;

    Tile& tile = board[row][col];
    if (tile.getRevealed() || tile.getFlagged()) return;

    tile.reveal();


    if (tile.getNearbyMines() == 0 && !tile.getMine()) {
        for (Tile* neighbor : tile.getNearbyTiles()) {
            int nRow = neighbor->getSprite().getPosition().y / 32;
            int nCol = neighbor->getSprite().getPosition().x / 32;


            if (!neighbor->getRevealed()) {
                reveal(nRow, nCol);
            }
        }
    }

}

Tile& Board::getTile(int row, int col) {
    return board[row][col];
}

sf::Texture& Board:: getTexture(const std:: string& name){
  return textures.at(name);
}

void Board:: reset() {
    board.clear();

    createBoard();
    placeMines();
    setNeighbours();
}

int Board:: getFlags() const {
    int count = 0;
    for (auto row : board) {
        for (auto col : row) {
            if (col.getFlagged()) {
                count++;
            }
        }
    }
    return count;
}