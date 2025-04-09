//
// Created by kkati on 4/8/2025.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>

class Tile {
  public:
    bool isMine = false;
    bool revealed = false;
    bool flagged = false;
    sf::RectangleShape shape;




  };
