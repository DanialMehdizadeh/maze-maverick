#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <ctime>
#include <dirent.h>
#include <sstream>
#include <conio.h>
#include <windows.h>
#include <algorithm>
#include <string>
#include <chrono>
//used libraries

using namespace std;



//PLAYGROUND
bool isValidCell(int x, int y, vector<vector<int>>& matrix, vector<vector<bool>>& visited)
//check 3 conditions for each cell of matrix
{
    return (x >= 0 && x < matrix.size()) && (y >= 0 && y < matrix[0].size()) && (visited[x][y] == 0) && (matrix[x][y] != 0);
}

void SetConsoleColor(int val)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), val);
}

void gotoxy(int x, int y)
//function to go to the first place 
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void printMatrix(int currentRow, int currentCol, vector<vector<int>>& matrix, vector<vector<bool>>& visited, int stepsLeft, int sum)
{
	//function that print the matrix again and again with diffrent color situations
    system("cls");
    gotoxy(0, 0);

    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[i].size(); j++)
        {
            if (visited[i][j])
            {
                SetConsoleColor(12);
            }
            if (matrix[i][j] == 0)
            {
                SetConsoleColor(8);
            }
            if (i == currentRow && j == currentCol)
            {
                SetConsoleColor(10);
            }
            cout << setw(2) << matrix[i][j] << " ";
            SetConsoleColor(7);
        }
        cout << endl;
    }
    cout << stepsLeft << " Steps Left !" << endl;
    cout << "Sum so far is " << sum << endl;
}
///////////////////////////

//USERS
struct User
{
    string username;
    int totalGames;
    int totalWins;
    double lastWinTime;
    double totalGameTime;
};
void saveUser(const User& user)
{
    string filename = "./Users/" + user.username + ".txt";
    ofstream outfile(filename);

    if (outfile.is_open())
    {
        outfile << user.username << endl;
        outfile << user.totalGames << endl;
        outfile << user.totalWins << endl;
        outfile << user.totalGameTime << endl;
        outfile << user.lastWinTime << endl;
        outfile.close();
    }
    else
    {
        cout << "Error: Unable to open file for user " << user.username << endl;
    }
}
User readUser(const string& username)
{
    User user;
    string filename = "./Users/" + username + ".txt";
    ifstream infile(filename);

    if (infile.is_open())
    {
        infile >> user.username;
        infile >> user.totalGames;
        infile >> user.totalWins;
        infile >> user.totalGameTime;
        infile >> user.lastWinTime;
        infile.close();
    }
    else
    {
        user.username = username;
        user.totalGames = 0;
        user.totalWins = 0;
        user.totalGameTime = 0;
        user.lastWinTime = 0;
    }
    return user;
}
///////////////////////////

//History
string getCurrentDateAsString()
{
    time_t now = time(nullptr);
    tm *localTime = localtime(&now);
    stringstream ss;
    ss << put_time(localTime, "%Y-%m-%d");
    return ss.str();
}
///////////////////////////


//SOLVING THE MAZE
bool isSafe(int x, int y, vector<vector<int>>& maze)
{
	//to check can we go to other neighbours of cell or not
    return (x >= 0 && x < maze.size() && y >= 0 && y < maze[0].size() && maze[x][y] != 0);
}

bool solveMaze(int x, int y, int sum, vector<vector<int>>& maze, vector<vector<int>>& solved)
{
    if (x == maze.size() - 1 && y == maze[0].size() - 1)
    {
        if (sum == maze[x][y])
        {
            solved[x][y] = 1;
            return true;
        }
        //if it reached last cell and the cell values equals to sum its the right way
        else
        {
            return false;
        }
    }

    if (isSafe(x, y, maze))
    {
        solved[x][y] = 1;
        sum += maze[x][y];

        if (solveMaze(x + 1, y, sum, maze, solved))
        {
            return true;
        }
        if (solveMaze(x, y + 1, sum, maze, solved))
        {
            return true;
        }
        solved[x][y] = 0;
        return false;
    }

    return false;
}
void printMaze(vector<vector<int>>& maze, vector<vector<int>>& solved)
{
    for (int i = 0; i < maze.size(); i++)
    {
        for (int j = 0; j < maze[i].size(); j++)
        {
            if (solved[i][j] == 1)
            {
                SetConsoleColor(10);
            }
            if (maze[i][j] == 0)
            {
                SetConsoleColor(8);
            }
            cout << setw(2) << maze[i][j] << " ";
            SetConsoleColor(7);
        }
        cout << endl;
    }
}
////////////////////////

vector<vector<int>> readMatrixFromFile(const string& filePath, int& numRows, int& numCols, int& pathLength, string& fileName)
{
    vector<vector<int>> matrix;

    ifstream inputFile(filePath);
    if (!inputFile.is_open())
    {
        cerr << "Error opening file: " << filePath << endl;
        return matrix;
    }
    // Read the first line from the file and store it as the file name

    getline(inputFile, fileName);
    inputFile.ignore(1, '\n');//ignore first line
    inputFile.ignore(1, '\n');//ignore secod line
    string difficulty;//the difficulty of maze
    getline(inputFile, difficulty);
    inputFile >> pathLength;// Read the path length from the file
    
    string line;
    while (getline(inputFile, line)) // Read each line from the file and construct the matrix
    {
        if (line != "")
        {
            vector<int> row;
            istringstream iss(line);

            int num;
            while (iss >> num)
            {
                row.push_back(num);
            }

            // Update the number of columns based on the first row
            if (numRows == 0)
            {
                numCols = row.size();
            }

            matrix.push_back(row);
            numRows++;
        }
    }

    inputFile.close();
    return matrix;
}

void listOrInput(string& filePath)
{
    int choice;
    cout << "\033[1;31m1.\033[0m Choose from Existing Maps" << endl;
    cout << "\033[1;31m2.\033[0m Import a Custom Map" << endl;
    cout << "Enter your choice: ";
    cin >> choice;
    while (cin.fail() || choice < 1 || choice > 2)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid choice. Please enter a valid option (1-2): ";
        cin >> choice;
    }
    switch (choice)
    {
        case 1:
        {
            string folderPath = "./Maps/";

            DIR *dir = opendir(folderPath.c_str());

            if (dir)
            {
                vector<string> fileNames;

                struct dirent *entry;
                int index = 1;
                while ((entry = readdir(dir)) != nullptr)
                {
                    // Exclude . and ..
                    if (string(entry->d_name) != "." && string(entry->d_name) != "..")
                    {
                        cout << "\033[93m" << index << ". " << "\033[0m" << entry->d_name << endl;
                        fileNames.push_back(entry->d_name);
                        index++;
                    }
                }

                closedir(dir);

                cout << "Choose a file by entering its index: ";
                int choice;
                cin >> choice;

                if (choice >= 1 && choice <= static_cast<int>(fileNames.size()))
                {
                    string selectedFile = fileNames[choice - 1];
                    filePath = folderPath + selectedFile;
                }
                else
                {
                    cerr << "Invalid choice." << endl;
                    return;
                }
            }
            else
            {
                cerr << "Error opening directory: " << folderPath << endl;
                return;
            }
            break;
        }
        case 2:
        {
            cout << "Please Enter the Path to the Maze :" << endl;
            cin.ignore(1, '\n');
            getline(cin, filePath);
            break;
        }
        default:
            cerr << "Invalid choice." << endl;
    }
    
}
void saveMatrix(const vector<vector<int>>& matrix, ofstream& fout, int pathLength, string& filename)
//The function gets a 2D vector , an output file and an integer from the user
{
    fout << filename << endl << endl << "Easy" << endl << endl << pathLength << endl << endl;
    //print "Easy" and the integer pathLength in the output file
    for (int i = 0; i < matrix.size(); i++)
    {
        for (int j = 0; j < matrix[i].size(); j++)
        {
            cout << setw(2) << matrix[i][j] << " ";
            fout << setw(2) << matrix[i][j] << " ";
        }
        cout << endl << endl;
        fout << endl << endl;
    }
    //The usage of these two for is to prints the elements of the matrix to both the output file and the console
    //setw is to be sure that it has enough width of 2 for characters
    fout << endl;
}

int getRandomNumber(int min, int max)
{
    return rand() % (max - min + 1) + min;
    //this function by using rand() function gives a number between min and max
}

int main()
{
	//clear everything
    system("cls");
    //to start from the first
    gotoxy(0, 0);
	//now lets go for the main function of program
    int choice;
    cout << "\033[1;31mWELCOME TO MAZEEEEEEE\033[0m" << endl;
    //welcome to user
    do
    {
        cout << "\033[1;31m   MAIN MENU\033[0m"<< endl;
        cout << "\033[1;31m1.\033[0m Create a New Easy Map" << endl;
        cout << "\033[1;31m2.\033[0m Playground" << endl;
        cout << "\033[1;31m3.\033[0m Solve a Maze" << endl;
        cout << "\033[1;31m4.\033[0m History" << endl;
        cout << "\033[1;31m5.\033[0m User Information" << endl;
        cout << "\033[1;31m6.\033[0m Exit" << endl;
        //options of menu that user can choose between them
        cout << "Enter your choice: ";
        cin >> choice;
        while (cin.fail() || choice < 1 || choice > 6)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice. Please enter a valid option (1-6): ";
            cin >> choice;
        }
        switch (choice)
        {

            //Creating a new maze
            case 1:
            {
                srand(static_cast<unsigned int>(time(0)));

                int rows, cols;
                cout << "Enter the number of rows: ";
                if (!(cin >> rows) || rows <= 0)
                //check if number of rows is logical or not
                {
                    cerr << "Invalid input for the number of rows. Please enter a valid number." << endl;
                    return 1;
                }
                cout << "Enter the number of columns: ";
                if (!(cin >> cols) || cols <= 0 || cols > 63)
                {
                	 //check if number of columns is logical or not
                    cerr << "Invalid input for the number of columns. Please enter a valid number." << endl;
                    return 1;
                }
				// Get the filename for the maze
                string fileName;
                cout << "Enter the name of the maze: "<< endl;
                cin.ignore(1, '\n');
                getline(cin, fileName);
                // Open the output file
                ofstream fout("./Maps/" + fileName + ".txt");
                if (!fout.is_open())
                {
                    cerr << "Error opening the file!" << endl;
                    return 1;
                }
                //check if the file is valid or not

                vector<vector<int>> matrix(rows, vector<int>(cols, 0));
                vector<vector<int>> visited(rows, vector<int>(cols, 0));
                

                int pathLength = rows + cols - 2; //pathLength for the easy maze formula
                int lowerLimit = -3, upperLimit = 3;
				// gives random numbers for the maze
                int currentx = 0, currenty = 0, sum = 0;
                for (int k = 0; k < pathLength; k++)
                {
                	// gives random values between -3 and 3
                    int randomValue = rand() % (7) - 3;
                    //check if number wont be 0
                    while (randomValue == 0)
                    {
                        randomValue = rand() % (7) - 3;
                    }
                    matrix[currentx][currenty] = randomValue;
                    sum += randomValue;
                    //marke one cell that checked
                    visited[currentx][currenty] = 1;
					// going to the next cell
                    if (currentx == rows - 1)
                    {
                        currenty++;
                        //we just can go down
                    }
                    else if (currenty == cols - 1)
                    {
                        currentx++;
                        //we just can go right
                    }
                    else
                    {
                    	// if we are not at last row or coloumn
                        int randomNumber = rand();
                        //get a random number betwen 0 and 1
                        if (randomNumber % 2 == 0)
                        {
                            currentx++;
                        }
                        else
                        {
                            currenty++;
                        }
                    }
                }
                matrix[rows - 1][cols - 1] = sum;
                //the last matrix is the sum
                visited[rows - 1][cols - 1] = 1;
                //and it is checked
				// Add blocks to the maze
                int countZero = 0;
                for (const auto& row : matrix)
                // this is a loop to calculate number of 0 numbers in maze
                {
                    for (int value : row)
                    {
                        if (value == 0)
                        {
                            countZero++;
                        }
                    }
                }
                int blocks = getRandomNumber(2, 5);
                // give a random number between 2 and 5
                if (countZero < 2)
                {
                    cerr << "Can't print blocks";
                    return 1;
                }
                while (blocks > countZero)
                {
                    blocks = getRandomNumber(2, 5);
                }

                for (int i = 0; i < blocks; i++)
                {
                    int randomRowIndex = getRandomNumber(0, rows - 1);
                    int randomColIndex = getRandomNumber(0, cols - 1);

                    while (matrix[randomRowIndex][randomColIndex] != 0 || visited[randomRowIndex][randomColIndex] == 1)
                    {
                        randomRowIndex = getRandomNumber(0, rows - 1);
                        randomColIndex = getRandomNumber(0, cols - 1);
                    }

                    matrix[randomRowIndex][randomColIndex] = 0;
                    visited[randomRowIndex][randomColIndex] = 1;
                }

                for (int i = 0; i < rows; i++)
                {
                    for (int j = 0; j < cols; j++)
                    {
                        if (visited[i][j] == 0)
                        {
                            int randomValue = rand() % (7) - 3;
                            while (randomValue == 0)
                            {
                                randomValue = rand() % (7) - 3;
                            }
                            matrix[i][j] = randomValue;
                            visited[i][j] = 1;
                        }
                    }
                }
				// Print and save the maze
                saveMatrix(matrix, fout, pathLength, fileName);
                cout << "Maze saved successfully" << endl;
                cout << "Press any key to continue" << endl;
                char next = _getch();
                //if user type anything doesnt show it on screen
                system("cls");
                gotoxy(0, 0);
                fout.close();
                break;
            }
            //Playground
            case 2:
            {
                string filePath = "";
                string fileName = "";
                listOrInput(filePath);
                int numRows, numCols, pathLength;
                vector <vector<int>> matrix=readMatrixFromFile(filePath, numRows, numCols, pathLength, fileName);
                vector<vector<bool>> visited(matrix.size(), vector<bool>(matrix[0].size(), false));

                string username;
                cout << "Enter your username here: " << endl;
                cin.ignore(1, '\n');
                getline(cin, username);
                

                visited[0][0] = true;

                int sum = 0;
                int currentRow = 0;
                int currentCol = 0;
                int stepsLeft = pathLength;
                system("cls");
                auto start = std::chrono::high_resolution_clock::now();
                //for the time duration

                while (!(currentRow == matrix.size()-1 && currentCol == matrix[0].size()-1) && (isValidCell(currentRow-1, currentCol, matrix, visited)||isValidCell(currentRow, currentCol-1, matrix, visited))||isValidCell(currentRow+1, currentCol, matrix, visited)||isValidCell(currentRow, currentCol+1, matrix, visited))
                {
                    if (stepsLeft == 0)
                    {
                        break;
                    }
                    printMatrix(currentRow, currentCol, matrix, visited, stepsLeft, sum+matrix[currentRow][currentCol]);

                    char key = _getch();

                    switch (key)
                    //check conditions of w a s d keys
                    {
                        case 'w':
                        	//go up
                            if (isValidCell(currentRow-1, currentCol, matrix, visited))
                            {
                                sum += matrix[currentRow][currentCol];
                                currentRow--;
                                stepsLeft--;
                            }
                            break;
                        case 'a':
                        	//go left
                            if (isValidCell(currentRow, currentCol-1, matrix, visited))
                            {
                                sum += matrix[currentRow][currentCol];
                                currentCol--;
                                stepsLeft--;
                            }
                            break;
                        case 's':
                        	//go down
                            if (isValidCell(currentRow+1, currentCol, matrix, visited))
                            {
                                sum += matrix[currentRow][currentCol];
                                currentRow++;
                                stepsLeft--;
                            }
                            break;
                        case 'd':
                        	//go right
                            if (isValidCell(currentRow, currentCol+1, matrix, visited))
                            {
                                sum += matrix[currentRow][currentCol];
                                currentCol++;
                                stepsLeft--;
                            }
                            break;
                        default:
                            break;
                    }

                    visited[currentRow][currentCol] = true;
                }
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
                //for endint timer and calculate time between start and time
                printMatrix(currentRow, currentCol, matrix, visited, stepsLeft, sum);
                User user = readUser(username);
                string history = getCurrentDateAsString() + " user(" + username + ")" + " in (" + fileName + ")" + " map ";

                if (currentRow == matrix.size()-1 && currentCol == matrix[0].size()-1)
                {
                	//if we reach to the last cell of matrix
                    if (sum == matrix[currentRow][currentCol])
                    {
                    	//if sum value is equal to the last cell you won else you lost
                        cout << "You WON!!" << endl;
                        user.totalGames++;
                        user.totalWins++;
                        user.totalGameTime += duration.count();
                        user.lastWinTime = duration.count();
                        history += "WON in " + to_string(duration.count()) +" seconds.";
                    }
                    else
                    {
                        cout << "You LOST!!" << endl;
                        user.totalGames++;
                        user.totalGameTime += duration.count();
                        history += "LOST in " + to_string(duration.count()) +" seconds.";
                    }
                }
                else
                {
                	//if you didnt reach to last cell you are already lost
                    cout << "You Lost" << endl;
                    user.totalGames++;
                    user.totalGameTime += duration.count();
                    history += "LOST in " + to_string(duration.count()) +" seconds.";
                }

                ifstream historyIn("./Stats/History.txt");
                vector<string> oldHistory;
                string line;
                while (getline(historyIn, line))
                {
                    oldHistory.push_back(line);
                }
                historyIn.close();

                ofstream historyOut("./Stats/History.txt");
                historyOut << history << "\n";
                for (const auto& originalHistory : oldHistory)
                {
                    historyOut << originalHistory << "\n";
                }
                historyOut.close();
                

                saveUser(user);
                Sleep(2);
                cout << "Press any key to continue" << endl;
                char next = _getch();
                system("cls");
                gotoxy(0, 0);
                break;
                
            }
            //Solving the Maze
            case 3:
            {
            	//for solving a maze by programe
                string filePath = "";
                string fileName = "";
                listOrInput(filePath);
                int numRows, numCols, pathLength;
                vector <vector<int>> maze=readMatrixFromFile(filePath, numRows, numCols, pathLength, fileName);
                vector<vector<int>> solved(maze.size(), vector<int>(maze[0].size(), 0));
                solved[0][0] = 1;

                if (!solveMaze(0, 0, 0, maze, solved))
                {
                    printMaze(maze, solved);
                    cout << "Solution doesn't exist";
                    break;
                }
                else
                {
                    printMaze(maze, solved);
                }
                cout << "Press any key to continue" << endl;
                char next = _getch();
                system("cls");
                gotoxy(0, 0);
                break;

            }
            //History
            case 4:
            {
                ifstream historyIn("./Stats/History.txt");
                vector<string> History;
                string line;
                int i = 0;
                while (i < 10 && getline(historyIn, line))
                {
                    History.push_back(line);
                    i++;
                }
                int index = 1;
                for (const auto& topTenHistory : History)
                {
                    cout << "\033[94m" << index << ". " << "\033[0m" << "\033[96m" << topTenHistory << "\033[0m" << "\n";
                    index++;
                }
                cout << "Press any key to continue" << endl;
                char next = _getch();
                system("cls");
                gotoxy(0, 0);
                break;
            }
            //User Information
            case 5:
            {
            	//this case is about user information
                string username;
                cout << "Please enter your username below :" << endl;
                cin.ignore(1, '\n');
                getline(cin, username);
                User user = readUser(username);
                //by going to structure of user and getting values of it , prints them
                cout << "\033[92m" << "Username: " << "\033[0m" << user.username << endl;
                cout << "\033[92m" << "Total Games: " << "\033[0m" << user.totalGames << endl;
                cout << "\033[92m" << "Total Wins: " << "\033[0m" << user.totalWins << endl;
                cout << "\033[92m" << "Last Win Time: " << "\033[0m" << user.lastWinTime << endl;
                cout << "\033[92m" << "Total Game Time: " << "\033[0m" << user.totalGameTime << endl;
                cout << "Press any key to continue" << endl;
                char next = _getch();
                system("cls");
                gotoxy(0, 0);
                break;

            }
            //Exiting game
            case 6:
            {
                cout << "Thanks for playing" << endl;
                break;
            }
        }
    }
    while (choice != 6);

    Sleep(1000);

    return 0;
}   
