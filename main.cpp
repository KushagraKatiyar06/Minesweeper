#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include "Board.h"


//Methods
void setText(sf::Text &text, float x, float y) {
    sf::FloatRect textRect = text.getGlobalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    text.setPosition(x, y);
}

std::string capitalizeFirstLetter(std::string userInput) {
    if (userInput.empty()) return userInput;

    for (int i = 0; i < userInput.length(); i++) {
        userInput[i] = std::tolower(userInput[i]);
    }

    userInput[0] = std::toupper(userInput[0]);
    return userInput;
}

bool isAlphabetical(std::string userInput) {
    if (userInput.empty()) {
        return false;
    }

    for (unsigned int i = 0; i < userInput.size(); i++) {
        if (!isalpha(userInput[i])) {
            return false;
        }
    }
    return true;
}

//Windows
//Game Screen
int launchGameWindow(int width, int height, int mineCount) {
    std:: ifstream config;
    config.open("config.cfg");

    //Gathering config file
    std:: string colCount, rowCount;
    if (config.is_open()) {
        std:: getline(config, colCount);
        std:: getline(config, rowCount);

        std:: cerr << "Cols: " << colCount << std::endl;
        std:: cerr << "Rows: " << rowCount <<  "\n" << std::endl;
        config.close();

    } else {
        std:: cerr << "Could not open config file" << std::endl;
        return -1;
    }

    //Getting Dimensions
    int rows = std:: stoi(rowCount);
    int cols = std:: stoi(colCount);

    sf::RenderWindow gameWindow(sf::VideoMode(width, height), "Minesweeper Game");
    Board board(rows,cols, mineCount);

    //UI
    //Smiley Face
    sf::Sprite smiley;
    smiley.setTexture(board.getTexture("face_happy.png"));

    int smiley_x = ((cols / 2.0) * 32) - 32;
    int smiley_y = (rows + 0.5) * 32;
    smiley.setPosition(smiley_x,smiley_y);



    while (gameWindow.isOpen()) {
        sf::Event event;
        while (gameWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                gameWindow.close();

            // Left click to reveal a tile
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                int x = event.mouseButton.x;
                int y = event.mouseButton.y;

                int row = y / 32;
                int col = x / 32;

                board.reveal(row, col);

                //Reset when pressing smiley
                if (smiley.getGlobalBounds().contains(x,y)) {
                    std:: cout << "RESET BOARD!" << std::endl;
                    board.reset();
                }
            }

            // Right click to toggle flag
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
                int x = event.mouseButton.x;
                int y = event.mouseButton.y;

                int row = y / 32;
                int col = x / 32;

                if (row >= 0 && row < rows && col >= 0 && col < cols) {
                    Tile& tile = board.getTile(row, col);
                    if (!tile.getRevealed()) {
                        tile.toggleFlag();
                    }
                }
            }


        }

        gameWindow.clear(sf::Color::White);

        board.draw(gameWindow);
        gameWindow.draw(smiley);

        gameWindow.display();
    }


    //Loading Textures
    //Happy Face
    /*
    sf::Texture happy_face;
    if (!happy_face.loadFromFile("images/face_happy.png")) {
        std::cerr << "Could not load face image" << std::endl;
    }
    sf:: Sprite happy_face_sprite;
    happy_face_sprite.setTexture(happy_face);
    int happy_face_x = ((cols / 2.0) * 32) - 32;
    int happy_face_y = (rows + 0.5) * 32;
    happy_face_sprite.setPosition(happy_face_x, happy_face_y);
*/

}

//Start Screen
int LaunchStartScreen() {
    std:: ifstream config;
    config.open("config.cfg");

    //Getting Window Dimensions
    //Gathering config file
    std:: string colCount, rowCount, mineCount;
    if (config.is_open()) {
        std:: getline(config, colCount);
        std:: getline(config, rowCount);
        std:: getline(config, mineCount);

        std:: cerr << "Cols: " << colCount << std::endl;
        std:: cerr << "Rows: " << rowCount << std::endl;
        std:: cerr << "Mine: " << mineCount << "\n" << std::endl;
    } else {
        std:: cerr << "Could not open config file" << std::endl;
        return -1;
    }

    //Getting Dimensions
    int rows = std:: stoi(rowCount);
    int cols = std:: stoi(colCount);
    int mines = std :: stoi(mineCount);

    int height = (rows * 32) + 100;
    int width = (cols * 32);

    std:: cerr << "SC Height: " << height << std::endl;
    std:: cerr << "SC Width: " << width << "\n" << std::endl;

    sf::RenderWindow welcomeWindow(sf::VideoMode(width, height), "Minesweeper");

    //Text
    sf:: Font font;

    if (!font.loadFromFile("font.ttf")) {
        std::cerr << "Error: Could not load font!" << std::endl;
        return -1;
    } else {
        std:: cerr << "Successfully loaded font!" << std::endl;
    }

    //(Visual) Welcome to Minesweeper
    int welcome_x = (width / 2) - 150;
    int welcome_y = (height / 2) - 150;

    sf:: Text welcome;
    welcome.setString("WELCOME TO MINESWEEPER!");
    welcome.setFont(font);
    welcome.setCharacterSize(24);
    welcome.setFillColor(sf::Color::White);
    welcome.setPosition(sf::Vector2f(welcome_x, welcome_y));

    sf:: FloatRect textBounds = welcome.getLocalBounds();
    sf::RectangleShape underline(sf::Vector2f(textBounds.width, 2));
    underline.setFillColor(sf::Color::White);
    underline.setPosition(welcome_x, welcome_y + 30);

    //(Visual) Enter your name
    int name_x = (width / 2) - 75;
    int name_y = (height / 2) - 75;

    sf :: Text name;
    name.setString("Enter your name:\t");
    name.setFont(font);
    name.setCharacterSize(20);
    name.setFillColor(sf::Color::White);
    name.setPosition(name_x, name_y);

    //(Behavior) Enter your name
    int input_x = (width / 2) - 45;
    int input_y = (height / 2) - 45;

    std:: string userInput;
    sf:: Text inputText;
    inputText.setFont(font);
    inputText.setCharacterSize(18);
    inputText.setFillColor(sf::Color::White);
    inputText.setPosition(input_x, input_y);

    //Cursor
    sf:: RectangleShape cursor(sf::Vector2f(2, 24));
    cursor.setFillColor(sf::Color::White);
    cursor.setPosition(sf::Vector2f(input_x + inputText.getLocalBounds().width, input_y));
    sf::Clock clock;
    bool showCursor = true;

    while(welcomeWindow.isOpen()) {
        sf::Event event;
        while(welcomeWindow.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                welcomeWindow.close();
            }

            if (event.type == sf::Event::TextEntered) {
                char enteredChar = static_cast<char>(event.text.unicode);

                if (event.text.unicode == 8 && userInput.length() > 0) {
                    userInput.pop_back();
                }

                else if (userInput.length() < 10 && ((enteredChar >= 'A' && enteredChar <= 'Z') || (enteredChar >= 'a' && enteredChar <= 'z'))) {
                    userInput += enteredChar;
              }

                if (userInput.length() >= 1) {
                    userInput = capitalizeFirstLetter(userInput);
                }

                inputText.setString(userInput);
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) {
                if (!userInput.empty()) {
                    welcomeWindow.close();
                    //LaunchLeaderBoard(width, height);
                    launchGameWindow(width, height, mines);
                    return 0;
                }
            }
        }

        if (clock.getElapsedTime().asSeconds() >= 0.5f) {
            showCursor = !showCursor;
            clock.restart();
        }
        cursor.setPosition(sf::Vector2f(input_x + inputText.getLocalBounds().width, input_y));

        welcomeWindow.clear(sf::Color::Blue);
        welcomeWindow.draw(welcome);
        welcomeWindow.draw(underline);
        welcomeWindow.draw(name);
        welcomeWindow.draw(inputText);

        if (showCursor) {
            welcomeWindow.draw(cursor);
        }

        welcomeWindow.display();
    }

    return 0;
}

//Leaderboard Screen
int LaunchLeaderBoard() {
    std:: ifstream config;
    config.open("config.cfg");

    //Getting Window Dimensions
    //Gathering config file
    std:: string colCount, rowCount, mineCount;
    if (config.is_open()) {
        std:: getline(config, colCount);
        std:: getline(config, rowCount);
        std:: getline(config, mineCount);

        std:: cerr << "Cols: " << colCount << std::endl;
        std:: cerr << "Rows: " << rowCount << std::endl;
        std:: cerr << "Mine: " << mineCount << "\n" << std::endl;
    } else {
        std:: cerr << "Could not open config file" << std::endl;
        return -1;
    }

    //Getting Dimensions
    int rows = std:: stoi(rowCount);
    int cols = std:: stoi(colCount);

    int height = (rows * 16) + 50;
    int width = (cols * 16);

    std:: cerr << "LB Height: " << height << std::endl;
    std:: cerr << "LB Width: " << width << "\n" << std::endl;

    std:: ifstream leaderboard;
    leaderboard.open("leaderboard.txt");
    std::vector<std::pair<int, std::string>> allPlayers;

    if (!leaderboard.is_open()) {
        std:: cerr <<"Unable to open leaderboard file." << std::endl;
        return -1;
    }

    std:: string currentLine;
    while (std::getline(leaderboard, currentLine)) {
        int minutes = std::stoi(currentLine.substr(0,2));
        int seconds = std::stoi(currentLine.substr(3,2));
        std:: string name = currentLine.substr(6);

        int totalTime = minutes * 60 + seconds;
        //std:: cerr << minutes << ":" << seconds << ", " << name << std::endl;
        allPlayers.push_back(std::make_pair(totalTime, name));
    }

    leaderboard.close();
    std::sort(allPlayers.begin(), allPlayers.end());

    for (auto& player : allPlayers) {
        int time = player.first;
        std::string name = player.second;

        int minutes = time / 60;
        int seconds = time % 60;

        std::cout << (minutes < 10 ? "0" : "") << minutes << ":"
                  << (seconds < 10 ? "0" : "") << seconds << " - "
                  << name << "\n" << std::endl;
    }


    sf::RenderWindow leaderboardWindow(sf::VideoMode(width, height), "Minesweeper Leaderboard");

    sf:: Font font;

    if (!font.loadFromFile("font.ttf")) {
        std::cerr << "Error: Could not load font!" << std::endl;
        return -1;
    } else {
        std:: cerr << "Successfully loaded font!" << std::endl;
    }

    //(Visual) LEADERBOARD
    int leaderboard_x = (width / 2);
    int leaderboard_y = (height / 2) - 120;

    sf:: Text leaderboard_text;
    leaderboard_text.setString("LEADERBOARD");
    leaderboard_text.setFont(font);
    leaderboard_text.setCharacterSize(20);
    leaderboard_text.setFillColor(sf::Color::White);
    setText(leaderboard_text, leaderboard_x, leaderboard_y);

    //leaderboard_text.setPosition(sf::Vector2f(leaderboard_x, leaderboard_y));

    std::vector<sf::Text> entries;
    for (size_t i = 0; i < std::min((size_t)5, allPlayers.size()); ++i) {
        int totalTime = allPlayers[i].first;
        std::string name = allPlayers[i].second;
        int minutes = totalTime / 60;
        int seconds = totalTime % 60;

        std::stringstream ss;
        ss << (i + 1) << ".   "
           << (minutes < 10 ? "0" : "") << minutes << ":"
           << (seconds < 10 ? "0" : "") << seconds
           << "   " << name;

        sf::Text entry(ss.str(), font, 20);
        entry.setFillColor(sf::Color::White);
        float x = 200 - entry.getLocalBounds().width / 2;
        float y = 60 + i * 35;
        entry.setPosition(x, y);

        entries.push_back(entry);
    }
    

    while (leaderboardWindow.isOpen()) {
        sf::Event event;
        while (leaderboardWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                leaderboardWindow.close();
        }

        leaderboardWindow.clear(sf::Color::Blue);
        leaderboardWindow.draw(leaderboard_text);
        for (const auto& entry : entries)
            leaderboardWindow.draw(entry);
        leaderboardWindow.display();
    }


    return 0;
}

int main() {

    /*

    //  testing
    int cols = 25;
    int rows = 16;
    int mines = 50;

    int height = (rows * 32) + 100;
    int width = (cols * 32);

    Board testBoard(rows, cols, mines);

    sf::RenderWindow window(sf::VideoMode(width, height), "Board Test");
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);
        testBoard.draw(window);  // Draw board
        window.display();
    }

    return 0;

    */

    LaunchLeaderBoard();
    LaunchStartScreen();
    return 0;
}