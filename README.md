Minesweeper
A classic Minesweeper game built with C++ and the SFML library.

Features
Classic Gameplay: Enjoy the traditional Minesweeper experience.

Configurable Board: The game board's dimensions and number of mines are loaded from a config.cfg file, making it easy to customize the difficulty.

Leaderboard: Keep track of your best times with an in-game leaderboard.

Graphical Interface: The game uses the SFML library for a graphical interface, including textures for tiles, numbers, and faces.

Prerequisites
To build and run this project, you will need:

A C++ compiler (e.g., g++)

CMake

SFML (Simple and Fast Multimedia Library)

Make sure to configure your SFML installation path in the CMakeLists.txt file.

How to Build and Run
Clone the repository:

git clone https://github.com/your-username/minesweeper.git
cd minesweeper

Configure and build the project with CMake:

mkdir build
cd build
cmake ..
make

Note: The make command might be cmake --build . on some systems.

Run the executable:

./Minesweeper

Customization
The game's dimensions and mine count are controlled by the config.cfg file. You can edit this file to change the game's difficulty.

Example config.cfg:

25
16
50

This would create a board with 25 columns, 16 rows, and 50 mines.

File Structure
main.cpp: Contains the main game loop, welcome screen logic, and helper functions.

Board.h / Board.cpp: Manages the game board, including placing mines, revealing tiles, and checking for win/loss conditions.

Tile.h / Tile.cpp: Represents a single tile on the board, handling its state (revealed, flagged, mine, etc.).

CMakeLists.txt: CMake build script for the project.

images/: Directory for all the game's sprite assets.

Acknowledgment
This project uses the SFML library for graphics, audio, and window management.
