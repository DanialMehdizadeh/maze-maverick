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

using namespace std;
//PLAYGROUND
bool isValidCell(int x, int y, vector<vector<int>>& matrix, vector<vector<bool>>& visited)
{
    return (x >= 0 && x < matrix.size()) && (y >= 0 && y < matrix[0].size()) && (visited[x][y] == 0) && (matrix[x][y] != 0);
}

void SetConsoleColor(int val)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), val);
}

void gotoxy(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void printMatrix(int currentRow, int currentCol, vector<vector<int>>& matrix, vector<vector<bool>>& visited, int stepsLeft, int sum)
{
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
vector<vector<int>> readMatrixFromFile(const string& filename, int& numRows, int& numCols, int& pathLength)
{
    vector<vector<int>> matrix;

    ifstream inputFile(filename);
    if (!inputFile.is_open())
    {
        cerr << "Error opening file: " << filename << endl;
        return matrix;
    }

    string Name;
    getline(inputFile, Name);
    inputFile >> pathLength;
    
    string line;
    while (getline(inputFile, line))
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

void listOrInput(string& filePath, string& fileName)
{
    int choice;
    cout << "\033[1;31m1.\033[0m Choose from Existing Maps" << endl;
    cout << "\033[1;31m2.\033[0m Import a Custom Map" << endl;
    cout << "Enter your choice: ";
    cin >> choice;
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
                    fileName = selectedFile;
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
            for (int i = filePath.length()-1; filePath[i] != '/'; i--)
            {
                fileName += filePath[i];
            }
            reverse(fileName.begin(), fileName.end());
            break;
        }
        default:
            cerr << "Invalid choice." << endl;
    }
    
}
// These are the libraries that the program wants to start the maze game
void saveMatrix(const vector<vector<int>>& matrix, ofstream& fout, int pathLength)
//The function gets a 2D vector , an output file and an integer from the user
{
    fout << "Easy" << endl << endl << pathLength << endl << endl;
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
    system("cls");
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
                string filename;
                cout << "Enter the name of the maze: "<< endl;
                cin.ignore(1, '\n');
                getline(cin, filename);
                // Open the output file
                ofstream fout("./Maps/" + filename + ".txt");
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
                    visited[currentx][currenty] = 1;
					// going to the next cell
                    if (currentx == rows - 1)
                    {
                        currenty++;
                    }
                    else if (currenty == cols - 1)
                    {
                        currentx++;
                    }
                    else
                    {
                        if (getRandomNumber(0, 1) == 0)
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
                visited[rows - 1][cols - 1] = 1;
				// Add blocks to the maze
                int countZero = 0;
                for (const auto& row : matrix)
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
                saveMatrix(matrix, fout, pathLength);
                cout << "Maze saved successfully" << endl;
                cout << "Press any key to continue" << endl;
                char next = _getch();
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
                listOrInput(filePath, fileName);
                string username;
                cout << "Enter your username here: " << endl;
                cin.ignore(1, '\n');
                getline(cin, username);
                int numRows, numCols, pathLength;

                vector <vector<int>> matrix=readMatrixFromFile(filePath, numRows, numCols, pathLength);
                vector<vector<bool>> visited(matrix.size(), vector<bool>(matrix[0].size(), false));
                visited[0][0] = true;

                int sum = 0;
                int currentRow = 0;
                int currentCol = 0;
                int stepsLeft = pathLength;
                system("cls");
                auto start = std::chrono::high_resolution_clock::now();

                while (!(currentRow == matrix.size()-1 && currentCol == matrix[0].size()-1) && (isValidCell(currentRow-1, currentCol, matrix, visited)||isValidCell(currentRow, currentCol-1, matrix, visited))||isValidCell(currentRow+1, currentCol, matrix, visited)||isValidCell(currentRow, currentCol+1, matrix, visited))
                {
                    if (stepsLeft == 0)
                    {
                        break;
                    }
                    printMatrix(currentRow, currentCol, matrix, visited, stepsLeft, sum+matrix[currentRow][currentCol]);

                    char key = _getch();

                    switch (key)
                    {
                        case 'w':
                            if (isValidCell(currentRow-1, currentCol, matrix, visited))
                            {
                                sum += matrix[currentRow][currentCol];
                                currentRow--;
                                stepsLeft--;
                            }
                            break;
                        case 'a':
                            if (isValidCell(currentRow, currentCol-1, matrix, visited))
                            {
                                sum += matrix[currentRow][currentCol];
                                currentCol--;
                                stepsLeft--;
                            }
                            break;
                        case 's':
                            if (isValidCell(currentRow+1, currentCol, matrix, visited))
                            {
                                sum += matrix[currentRow][currentCol];
                                currentRow++;
                                stepsLeft--;
                            }
                            break;
                        case 'd':
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
                printMatrix(currentRow, currentCol, matrix, visited, stepsLeft, sum);
                User user = readUser(username);
                string history = getCurrentDateAsString() + " user(" + username + ")" + " in (" + fileName + ")" + " map ";

                if (currentRow == matrix.size()-1 && currentCol == matrix[0].size()-1)
                {
                    if (sum == matrix[currentRow][currentCol])
                    {
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
            //History
            case 4:
            {
                ifstream historyIn("./Stats/History.txt");
                vector<string> History;
                string line;
                int i=0;
                while (i<10 && getline(historyIn, line))
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
            case 5:
            {
                string username;
                cout << "Please enter your username below :" << endl;
                cin.ignore(1, '\n');
                getline(cin, username);
                User user = readUser(username);
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
    

    return 0;
}   
