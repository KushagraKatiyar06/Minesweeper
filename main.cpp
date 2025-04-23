#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <chrono>
#include "Board.h"


//Methods-----------------------------------------------
void setText(sf::Text &text, float x, float y) {
    sf::FloatRect textRect = text.getGlobalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    text.setPosition(x, y);
}

std::string capitalizeFirstLetter(std::string userInput) {
    if (userInput.empty()) {
        return userInput;
    }
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
    for (int i = 0; i < userInput.length(); i++) {
        if (!isalpha(userInput[i])) {
            return false;
        }
    }
    return true;
}

std::vector<int> readConfigFile() {
    std:: ifstream file("config.cfg");
    std::vector<int> config;

    if (file.is_open()) {
        std::string columns;
        std:: string rows;
        std:: string mines;

        std::getline(file, columns);
        std::getline(file, rows);
        std::getline(file, mines);

        config.push_back(std::stoi(columns));
        config.push_back(std::stoi(rows));
        config.push_back(std::stoi(mines));

        std::cerr <<"Information sucessfully loaded from config file\n" << std::endl;
    } else {
        std::cerr << "Error opening config file" << std::endl;
    }

    return config;
}


//Windows-----------------------------------------------
//Leaderboard Screen **********************
int leaderboardScreen(int playerTime = -1, std::string user = "") {
    std:: ifstream config;
    config.open("config.cfg");

    //Gathering Config File Elements
    std::vector<int> configFile = readConfigFile();
    int columns = configFile[0];
    int rows = configFile[1];

    //Getting Dimensions
    int height = (rows * 16) + 50;
    int width = (columns * 16);
    std:: cerr << "Leadorboard Height: " << height << std::endl;
    std:: cerr << "Leaderboard Width: " << width << "\n" << std::endl;

    sf::RenderWindow leaderboardWindow(sf::VideoMode(width, height), "Minesweeper Leaderboard");
    sf:: Font font;
    if (!font.loadFromFile("font.ttf")) {
        std::cerr << "Error: Could not load font!" << std::endl;
        return -1;
    } else {
        std:: cerr << "Successfully loaded font!" << std::endl;
    }

    //Gathering Leaderboard Data
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

    std::vector<sf::Text> entries;
    for (size_t i = 0; i < std::min((size_t)5, allPlayers.size()); ++i) {
        int totalTime = allPlayers[i].first;
        std::string name = allPlayers[i].second;
        int minutes = totalTime / 60;
        int seconds = totalTime % 60;

        std::stringstream ss;

        std::string minuteStr;
        if (minutes < 10) {
            minuteStr = "0" + std::to_string(minutes);
        } else {
            minuteStr = std::to_string(minutes);
        }

        std::string secondStr;
        if (seconds < 10) {
            secondStr = "0" + std::to_string(seconds);
        } else {
            secondStr = std::to_string(seconds);
        }

        ss << (i + 1);
        ss << ".   ";
        ss << minuteStr;
        ss << ":";
        ss << secondStr;
        ss << "   ";
        ss << name;

        sf::Text entry(ss.str(), font, 20);
        entry.setFillColor(sf::Color::White);
        float x = 200 - entry.getLocalBounds().width / 2;
        float y = 60 + i * 35;
        entry.setPosition(x, y);

        entries.push_back(entry);
    }

    //"LEADERBOARD"
    int leaderboard_x = (width / 2);
    int leaderboard_y = (height / 2) - 120;

    sf:: Text leaderboard_title;
    leaderboard_title.setString("LEADERBOARD");
    leaderboard_title.setFont(font);
    leaderboard_title.setCharacterSize(20);
    leaderboard_title.setFillColor(sf::Color::White);
    leaderboard_title.setPosition(leaderboard_x, leaderboard_y);
    //leaderboard_text.setPosition(sf::Vector2f(leaderboard_x, leaderboard_y));

    while (leaderboardWindow.isOpen()) {
        sf::Event event;
        while (leaderboardWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                leaderboardWindow.close();
        }

        leaderboardWindow.clear(sf::Color::Blue);
        leaderboardWindow.draw(leaderboard_title);
        for (const auto& entry : entries)
            leaderboardWindow.draw(entry);
        leaderboardWindow.display();
    }

    return 0;
}

//Game Screen ******************************************
int gameWindow(int width, int height, int mineCount, std:: string user) {
    std:: cerr << "Launching game window\n" << std::endl;

    //Gathering config file elements
    std::vector<int> configFile = readConfigFile();
    int columns = configFile[0];
    int rows = configFile[1];
    int mines = configFile[2];
    std:: cerr <<"Game Screen Height: " << height << std::endl;
    std:: cerr <<"Game Screen Width: " << width << "\n" << std::endl;

    sf::RenderWindow gameWindow(sf::VideoMode(width, height), "Minesweeper");
    Board board(columns,rows, mineCount);

    //UI Elements
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
    //Mine Counter Digits;
    int digitWidth = 21;

    sf::Texture digitsTexture = board.getTexture("digits.png");
    sf:: Sprite mineDigits[3];
    float mineCount_x = 33;
    float mineCount_y = 32 * (rows + 0.5f) + 16;
    for (int i = 0; i < 3; i++) {
        mineDigits[i].setTexture(digitsTexture);
        mineDigits[i].setTextureRect(sf::IntRect(0, 0, 21, 32));
        mineDigits[i].setPosition(mineCount_x + i * digitWidth, mineCount_y);
    }

    //Stopwatch Digits
    sf::Sprite timerDigits[4];

    float timer_y = 32 * (rows + 0.5f) + 16;
    int timer_x = (columns * 32) - 97;
    int gap = 5;


    for (int i = 0; i < 4; i++) {
        timerDigits[i].setTexture(digitsTexture);
        timerDigits[i].setTextureRect(sf::IntRect(0, 0, 21, 32));

        float offset;
        if (i < 2) {
            offset = i * digitWidth;
        } else {
            offset = i * digitWidth + gap;
        }

        timerDigits[i].setPosition(timer_x + offset, timer_y);
    }

    //Smiley Face
    sf::Sprite smiley;
    smiley.setTexture(board.getTexture("face_happy.png"));
    int smiley_x = ((columns / 2.0) * 32) - 32;
    int smiley_y = (rows + 0.5) * 32;
    smiley.setPosition(smiley_x,smiley_y);

    //Debug
    bool debug = false;
    sf::Sprite debugButton;
    debugButton.setTexture(board.getTexture("debug.png"));
    int debug_x = (columns * 32) - 304;
    int debug_y = (rows + 0.5) * 32;
    debugButton.setPosition(debug_x, debug_y);

    //Pause Button
    sf::Sprite pauseButton;
    pauseButton.setTexture(board.getTexture("pause.png"));
    int pause_x = (columns * 32) - 240;
    int pause_y = (rows + 0.5) * 32;
    pauseButton.setPosition(pause_x, pause_y);

    //Clock
    std::chrono::steady_clock::time_point startTime;
    std::chrono::steady_clock::time_point pauseStartTime;
    std::chrono::seconds pausedDuration{0};
    bool isPaused = false;
    bool timerStarted = false;
    int elapsedSeconds;

    //Leaderboard
    sf::Sprite leaderboard;
    leaderboard.setTexture(board.getTexture("leaderboard.png"));
    float leaderboard_x = (columns * 32) - 176;
    float leaderboard_y = (rows + 0.5) * 32;
    leaderboard.setPosition(leaderboard_x, leaderboard_y);

    //Event Loop
    while (gameWindow.isOpen()) {
        sf::Event event;
        while (gameWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                gameWindow.close();

            // Left click
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (event.type == sf::Event::MouseButtonPressed) {
                    int x = event.mouseButton.x;
                    int y = event.mouseButton.y;

                    if (smiley.getGlobalBounds().contains(x, y)) {
                        board.reset();
                        if (smiley.getGlobalBounds().contains(x, y)) {
                            board.reset();
                            timerStarted = false;
                            startTime = std::chrono::steady_clock::time_point();
                            pauseStartTime = std::chrono::steady_clock::time_point();
                            pausedDuration = std::chrono::seconds{0};
                            isPaused = false;
                            pauseButton.setTexture(board.getTexture("pause.png"));
                        }

                    }

                    if (debugButton.getGlobalBounds().contains(x, y)) {
                        debug = !debug;
                    }

                    if (pauseButton.getGlobalBounds().contains(x, y)) {
                        isPaused = !isPaused;
                        if (isPaused) {
                            pauseStartTime = std::chrono::steady_clock::now();
                            pauseButton.setTexture(board.getTexture("play.png"));
                        } else {
                            pausedDuration += std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - pauseStartTime);
                            pauseButton.setTexture(board.getTexture("pause.png"));
                        }
                    }

                    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                        int x = event.mouseButton.x;
                        int y = event.mouseButton.y;

                        if (leaderboard.getGlobalBounds().contains(x, y)) {
                            bool wasPaused = isPaused;
                            isPaused = true;

                            leaderboardScreen(-1);
                            isPaused = wasPaused;
                        }
                    }


                    if (!board.isGameOver() && !board.isGameWon() && !isPaused) {
                        int row = y / 32;
                        int col = x / 32;

                        if (event.mouseButton.button == sf::Mouse::Left) {
                            if (!timerStarted) {
                                startTime = std::chrono::steady_clock::now();
                                pausedDuration = std::chrono::seconds{0};
                                timerStarted = true;
                            }
                            board.reveal(row, col);
                        }

                        if (event.mouseButton.button == sf::Mouse::Right) {
                            Tile& tile = board.getTile(row, col);
                            if (!tile.getRevealed()) {
                                tile.toggleFlag();
                            }
                        }
                    }
                }
            }

            // Right click
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
                if (board.isGameOver() || board.isGameWon() || isPaused ) {
                    continue;
                }
                int x = event.mouseButton.x;
                int y = event.mouseButton.y;
                int row = y / 32;
                int col = x / 32;

                if (row >= 0 && row < rows && col >= 0 && col < columns) {
                    Tile& tile = board.getTile(row, col);
                    if (!tile.getRevealed()) {
                        tile.toggleFlag();
                    }
                }
            }
        }

        //Rendering Elements
        gameWindow.clear(sf::Color::White);
        board.draw(gameWindow, debug);
        gameWindow.draw(smiley);
        gameWindow.draw(debugButton);
        gameWindow.draw(leaderboard);
        gameWindow.draw(pauseButton);

        for (int i = 0; i < 3; i++) {
            gameWindow.draw(mineDigits[i]);
        }
        int remainingMines = mineCount - board.getFlags();
        bool isNegative = remainingMines < 0;
        int absVal = std::abs(remainingMines);
        std::string countStr = std::to_string(absVal);
        while (countStr.length() < 3) countStr = "0" + countStr;

        for (int i = 0; i < 3; i++) {
            int digit = countStr[i] - '0';
            mineDigits[i].setTextureRect(sf::IntRect(digit * 21, 0, 21, 32));
        }
        if (isNegative) {
            mineDigits[0].setPosition(12, mineCount_y);
            mineDigits[0].setTextureRect(sf::IntRect(10 * 21, 0, 21, 32));
        }

        if (board.isGameOver()) {
            smiley.setTexture(board.getTexture("face_lose.png"));
        } else if (board.isGameWon()) {
            smiley.setTexture(board.getTexture("face_win.png"));
        } else {
            smiley.setTexture(board.getTexture("face_happy.png"));
        }


        if (timerStarted && !board.isGameOver() && !board.isGameWon()) {
            if (!isPaused) {
                auto now = std::chrono::steady_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - startTime - pausedDuration).count();
                elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(now - startTime - pausedDuration).count();
                if (elapsedSeconds > 999) elapsedSeconds = 999;
            }
        }

        int minutes = elapsedSeconds / 60;
        int seconds = elapsedSeconds % 60;
        std::string minuteStr;
        if (minutes < 10) {
            minuteStr = "0" + std::to_string(minutes);
        } else {
            minuteStr = std::to_string(minutes);
        }

        std::string secondStr;
        if (seconds < 10) {
            secondStr = "0" + std::to_string(seconds);
        } else {
            secondStr = std::to_string(seconds);
        }

        for (int i = 0; i < 2; i++) {
            int digit = minuteStr[i] - '0';
            timerDigits[i].setTextureRect(sf::IntRect(digit * 21, 0, 21, 32));
        }
        for (int i = 0; i < 2; i++) {
            int digit = secondStr[i] - '0';
            timerDigits[i + 2].setTextureRect(sf::IntRect(digit * 21, 0, 21, 32));
        }
        for (int i = 0; i < 4; i++) {
            gameWindow.draw(timerDigits[i]);
        }

        gameWindow.display();
    }
}

//Start Screen ******************************************
int welcomeScreen() {
    std:: cerr <<"Launching Welcome Window\n" << std::endl;
    std:: ifstream config;

    //Gathering config file elements
    std::vector<int> configFile = readConfigFile();
    int columns = configFile[0];
    int rows = configFile[1];
    int mines = configFile[2];
    //Dimensions
    int height = (rows * 32) + 100;
    int width = (columns * 32);

    std:: cerr <<"Start Screen Height: " << height << std::endl;
    std:: cerr <<"Start Screen Width: " << width << "\n" << std::endl;

    sf::RenderWindow welcomeWindow(sf::VideoMode(width, height), "Minesweeper");

    //Text
    sf:: Font font;
    if (!font.loadFromFile("font.ttf")) {
        std::cerr << "Error: Could not load font!\n" << std::endl;
        return -1;
    } else {
        std:: cerr << "Successfully loaded font!\n" << std::endl;
    }

    //"WELCOME TO MINESWEEPER"
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

    //"ENTER YOUR NAME"
    int name_x = (width / 2) - 75;
    int name_y = (height / 2) - 75;
    sf :: Text name;
    name.setString("Enter your name:\t");
    name.setFont(font);
    name.setCharacterSize(20);
    name.setFillColor(sf::Color::White);
    name.setPosition(name_x, name_y);

    //Input field
    int input_x = (width / 2) - 45;
    int input_y = (height / 2) - 45;
    std:: string userInput;
    sf:: Text inputText;
    inputText.setFont(font);
    inputText.setCharacterSize(18);
    inputText.setFillColor(sf::Color::White);
    inputText.setPosition(input_x, input_y);

    //->Cursor
    sf:: RectangleShape cursor(sf::Vector2f(2, 24));
    cursor.setFillColor(sf::Color::White);
    cursor.setPosition(sf::Vector2f(input_x + inputText.getLocalBounds().width, input_y));
    sf::Clock clock;
    int elapsedSeconds;
    bool showCursor = true;

    //Event Loop
    while(welcomeWindow.isOpen()) {
        sf::Event event;
        while(welcomeWindow.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                welcomeWindow.close();
            }

            if (event.type == sf::Event::TextEntered) {
                char enteredChar = event.text.unicode;
                if (event.text.unicode == 8 && userInput.length() > 0) {
                    userInput.pop_back();
                } else if (userInput.length() < 10 && ((enteredChar >= 'A' && enteredChar <= 'Z') || (enteredChar >= 'a' && enteredChar <= 'z'))) {
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
                    gameWindow(width, height, mines, userInput);
                    return 0;
                }
            }
        }

        //Blinking Cursor
        if (clock.getElapsedTime().asSeconds() >= 0.5f) {
            showCursor = !showCursor;
            clock.restart();
        }
        //Update cursor position
        cursor.setPosition(sf::Vector2f(input_x + inputText.getLocalBounds().width, input_y));

        //Rendering Visuals
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

    welcomeScreen();
    return 0;
}