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

void Board::draw(sf::RenderWindow& window, bool debug) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < columns; c++) {
            Tile& tile = board[r][c];

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
                    tile.setSprite(textures["mine.png"]);  // ← show hidden mines
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
        revealAllMines();  // implement this next
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
    if (gameOver || gameWin) return;  // don’t check after game ends

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
        revealAllMines();  // show all mines (optional)
    }
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

void Board:: reset() {
    gameOver = false;
    gameWin = false;
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

void Board::revealAllMines() {
    for (auto& row : board) {
        for (auto& tile : row) {
            if (tile.getMine()) {
                tile.reveal();
            }
        }
    }
}

bool Board::isGameWon() const {
    return gameWin;
}

void Board::setGameWon(bool value) {
    gameWin = value;
}

