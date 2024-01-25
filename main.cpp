#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <ctime>
#include <dirent.h>
#include <sstream>
#include <string>

using namespace std;

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

void listOrInput(string& filePath)
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
            string filePath;
            cout << "Please Enter the Path to the Maze :" << endl;
            cin.ignore(1, '\n');
            getline(cin, filePath);
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
	//now lets go for the main function of program
    int choice;
    cout << "WELCOME TO MAZEEEEEEE" << endl;
    //welcome to user
    do
    {
        cout << "\033[1;31m   MAIN MENU\033[0m"<< endl;
        cout << "\033[1;31m1.\033[0m Create a New Easy Map" << endl;
        cout << "\033[1;31m2.\033[0m Playground" << endl;
        cout << "\033[1;31m3.\033[0m Solve a Maze" << endl;
        cout << "\033[1;31m4.\033[0m History" << endl;
        cout << "\033[1;31m5.\033[0m Exit" << endl;
        //options of menu that user can choose between them
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice)
        {
            case 1:
            	 //Creating a new maze
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
                fout.close();
                break;
            }
            case 2:
            {
                string filePath = "";
                listOrInput(filePath);
                int numRows, numCols, pathLength;
                vector <vector<int>> matrix=readMatrixFromFile(filePath, numRows, numCols, pathLength);
                for (const auto& row : matrix)
                {
                    for (int num : row) {
                        cout << setw(2) << num << " ";
                    }
                    cout << endl;
                }
                break;
                
            }
            case 5:
            {
                cout << "Thanks for playing" << endl;
                break;
            }

            default:
            	// if user enters a number except 1,2,3,4,5 it will print this to warn user
                cout << "Invalid choice. Please enter a valid option." << endl;
                break;
        }
    }
    while (choice != 5);
    

    return 0;
}   
