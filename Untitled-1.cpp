#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <ctime>
using namespace std;
// These are the libraries that the programe wants to start the maze game
void printMatrix(const vector<vector<int>>& matrix, ofstream& fout, int p)
//The function gets a 2D vector , an output file and an integer from the user
{
    fout << "Easy" << endl << endl << p << endl << endl;
    //print "Easy" and the integer "p" in the output file
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
    //this function by using rand() function gives a number betwen min and max
}

int main()
{
	//now lets go for the main func of programe
    int choice;
    cout << "WELCOME TO MAZEEEEEEE" << endl;
    //welcome to user
    do
    {
        cout << "\033[1;31m1.\033[0m Create a New Easy Map" << endl;
        cout << "\033[1;31m2.\033[0m Playground" << endl;
        cout << "\033[1;31m3.\033[0m Solve a Maze" << endl;
        cout << "\033[1;31m4.\033[0m History" << endl;
        cout << "\033[1;31m5.\033[0m Exit" << endl;
        //options of menue that user can choice betweeen them
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
                //check if number of rows is logical or no 
                {
                    cerr << "Invalid input for the number of rows. Please enter a valid number." << endl;
                    return 1;
                }
                cout << "Enter the number of columns: ";
                if (!(cin >> cols) || cols <= 0 || cols > 63)
                {
                	 //check if number of columns is logical or no
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

                int p = rows + cols - 2; //calculate sum of rows and cols - 2
                int lowerLimit = -3, upperLimit = 3;
				// gives random numbers for the maze
                int currentx = 0, currenty = 0, sum = 0;
                for (int k = 0; k < p; k++)
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
                printMatrix(matrix, fout, p);
                cout << "Maze saved successfully" << endl;
                fout.close();
                break;
            }
            case 5:
            {
                cout << "Thanks for playing" << endl;
                break;
            }

            default:
            	// if user enters a number except 1,2,3,4,5 it will print this to wanrn user
                cout << "Invalid choice. Please enter a valid option." << endl;
                break;
        }
    }
    while (choice != 5);
     // print a thank you message if the user didnt choose to exit earlier

    if (choice != 5)
    {
        cout << "Thanks for playing" << endl;
    }
    return 0;
}
