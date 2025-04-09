#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>

//Methods
std::string capitalizeFirstLetter(std::string userInput) {
    if (userInput.empty()) return userInput;  // Return if the input string is empty

    for (int i = 0; i < userInput.length(); i++) {
        userInput[i] = std::tolower(userInput[i]);
    }

    userInput[0] = std::toupper(userInput[0]);  // Capitalize the first character
    return userInput;
}

bool isAlphabetical(std::string userInput) {
    if (userInput.empty()) {
        return false;
    }

    for (unsigned int i = 0; i < userInput.size(); i++) {
        if (isalpha(userInput[i])) {
            return false;
        }
    }

    return true;
}

//Windows

int launchGameWindow(int width, int height) {
    sf::RenderWindow gameWindow(sf::VideoMode(width, height), "Minesweeper");

    while (gameWindow.isOpen()) {
        sf::Event event;
        while(gameWindow.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                gameWindow.close();
            }
        }

        gameWindow.clear(sf::Color::Green);
        gameWindow.display();

    }
}


int main() {
    std:: ifstream config;
    config.open("config.cfg");

    //Getting Window Dimensions
    //Gathering config file
    std:: string rowCount, colCount, mineCount;
    if (config.is_open()) {
        std:: getline(config, rowCount);
        std:: getline(config, colCount);
        std:: getline(config, mineCount);

        std:: cerr << "Rows: " << rowCount << std::endl;
        std:: cerr << "Cols: " << colCount << std::endl;
        std:: cerr << "Mine: " << mineCount << "\n" << std::endl;
    } else {
        std:: cerr << "Could not open config file" << std::endl;
        return -1;
    }

    //Getting Dimensions
    //Welcome Window:
    int rows = std:: stoi(rowCount);
    int cols = std:: stod(colCount);

    int width = (rows * 32);
    int height = (cols * 32) + 100;

    std:: cerr << "Height: " << height << std::endl;
    std:: cerr << "Width: " << width << "\n" << std::endl;

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
    int name_x = (width / 2) - 85;
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
                    launchGameWindow(width, height);
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