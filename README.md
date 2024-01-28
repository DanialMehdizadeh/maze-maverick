# maze-maverick

This is a console-based maze game written in C++. The game allows users to create new mazes, play existing mazes or already created mazes by user, solve mazes automatically, and view the history of games.

## How the maze works?

Mazes of this type are made up of ints; each non-zero number represents the value of that cell in the maze, and each 0 represents a blocked path. The solution to this maze is such that we must go from the start point (which is located in the top left corner) to the end point (which is located in the bottom right corner), in such a way that the sum of all the numbers of the traversed path (without passing through 0) equals the value of the end point (which is located in the bottom right corner).

## Features

1. **Create a New Easy Map**: This feature allows users to create a new maze. Users can specify the number of rows and columns, and the name of the maze. The maze is then saved to a file in the "./Maps/" directory.

2. **Playground**: This feature allows users to play a maze game. Users can choose a maze from existing maps or import a custom map. Then the user uses wasd keys to play the game. The game tracks the user's progress and time taken to solve the maze.

3. **Solve a Maze**: This feature allows users to solve a maze automatically. Users can choose a maze from existing maps or import a custom map. The solution, if it exists, is then displayed.

4. **History**: This feature allows users to view the history of games. The top 10 entries from the history are displayed.

5. **User Information**: This feature allows users to view their game statistics, including total games played, total wins, and total game time.

## Usage

To run the game, compile and run the main.cpp file in cmd using the command g++ main.cpp -o main or compile and run the mainWithMusic.cpp file in cmd using the command                          g++ -o mainWithMuisc mainWithMuisc.cpp -lwinmm. 
A menu will be displayed with the available options. Enter the number corresponding to the option you want to choose. Use wasd keys to play in the playground.

This project was created by Danial Mehdizadeh and Alireza Ahmadi for a basic programming class in January of 2024.
