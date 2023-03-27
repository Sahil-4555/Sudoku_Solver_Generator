#include <iostream>
#include <string>
#include <array>
#include <stdlib.h>
#include <algorithm>
#include <math.h>
#include <chrono>
#include <random>
#include <time.h>
#include <thread>
#include <windows.h>
using namespace std;

const int PUZZLE_SIZE = 9;
const int EMPTY_VALUE = 0;
array<int, 9> values{1, 2, 3, 4, 5, 6, 7, 8, 9};
int numberofSolution = 1;

void setCursorPosition(int x, int y)
{
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    cout.flush();
    COORD coord = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(hOut, coord);
}

void sleep(int miliseconds)
{
    this_thread::sleep_for(chrono::milliseconds(miliseconds));
}

void printPuzzle(int puzzle[PUZZLE_SIZE][PUZZLE_SIZE], bool clear = true)
{
    if (clear)
    {
        setCursorPosition(0, 0);
    }
    string text, separator, padding;

    for (int i = 0; i < PUZZLE_SIZE; i++)
    {
        text = "|";
        separator = " -";
        padding = "|";
        for (int j = 0; j < PUZZLE_SIZE; j++)
        {
            string value = puzzle[i][j] == EMPTY_VALUE ? " " : to_string(puzzle[i][j]);
            text += "  " + value + "  |";
            separator += "------";
            padding += "     |";
            if (j % 3 == 2 && j != PUZZLE_SIZE - 1)
            {
                text += "|";
                padding += "|";
            }
        }
        if (i != 0 && i % 3 == 0)
        {
            replace(separator.begin(), separator.end(), '-', '=');
        }
        cout << separator << "\n";
        cout << padding << "\n";
        cout << text << "\n";
        cout << padding << "\n";
    }
    cout << separator << "\n";
}

bool isValid(int puzzle[PUZZLE_SIZE][PUZZLE_SIZE], int row, int col, int value)
{
    // Check If In This Row Already Has Value.
    for (int c = 0; c < PUZZLE_SIZE; c++)
    {
        if (puzzle[row][c] == value)
            return false;
    }

    // Check if This Column Already Has Valued.
    for (int r = 0; r < PUZZLE_SIZE; r++)
    {
        if (puzzle[r][col] == value)
            return false;
    }

    // Check If Current Grid Already Has Valued.
    int startRow = floor(row / 3) * 3, startCol = floor(col / 3) * 3;
    for (int r = startRow; r < startRow + 3; r++)
    {
        for (int c = startCol; c < startCol + 3; c++)
        {
            if (puzzle[r][c] == value)
                return false;
        }
    }
    return true;
}

bool hasEmptyCell(int puzzle[PUZZLE_SIZE][PUZZLE_SIZE])
{
    for (int r = 0; r < PUZZLE_SIZE; r++)
    {
        for (int c = 0; c < PUZZLE_SIZE; c++)
        {
            if (puzzle[r][c] == EMPTY_VALUE)
                return true;
        }
    }
    return false;
}

void copyPuzzle(int origin[PUZZLE_SIZE][PUZZLE_SIZE], int (&copy)[PUZZLE_SIZE][PUZZLE_SIZE])
{
    for (int r = 0; r < PUZZLE_SIZE; r++)
    {
        for (int c = 0; c < PUZZLE_SIZE; c++)
        {
            copy[r][c] = origin[r][c];
        }
    }
}

bool fillPuzzle(int (&puzzle)[PUZZLE_SIZE][PUZZLE_SIZE])
{
    int row, col;
    for (int i = 0; i < PUZZLE_SIZE * PUZZLE_SIZE; i++)
    {
        row = floor(i / PUZZLE_SIZE);
        col = i % PUZZLE_SIZE;
        if (puzzle[row][col] == EMPTY_VALUE)
        {
            unsigned seed = chrono::system_clock::now().time_since_epoch().count();
            shuffle(values.begin(), values.end(), default_random_engine(seed));
            for (int j = 0; j < PUZZLE_SIZE; j++)
            {
                if (isValid(puzzle, row, col, values[j]))
                {
                    puzzle[row][col] = values[j];
                    if (!hasEmptyCell(puzzle) || fillPuzzle(puzzle))
                    {
                        return true;
                    }
                }
            }
            break;
        }
    }
    puzzle[row][col] = EMPTY_VALUE;
    return false;
}

bool solveSudoku(int puzzle[PUZZLE_SIZE][PUZZLE_SIZE], bool visualize = false)
{
    int row, col;
    for (int i = 0; i < PUZZLE_SIZE * PUZZLE_SIZE; i++)
    {
        row = floor(i / PUZZLE_SIZE);
        col = i % PUZZLE_SIZE;
        if (puzzle[row][col] == EMPTY_VALUE)
        {
            for (int value = 1; value <= PUZZLE_SIZE; value++)
            {
                if (isValid(puzzle, row, col, value))
                {
                    puzzle[row][col] = value;
                    if (visualize)
                    {
                        sleep(100);
                        printPuzzle(puzzle);
                    }
                    if (!hasEmptyCell(puzzle))
                    {
                        numberofSolution++;
                        if (visualize)
                        {
                            sleep(100);
                            printPuzzle(puzzle);
                            return true;
                        }
                        break;
                    }
                    else if (solveSudoku(puzzle, visualize))
                    {
                        return true;
                    }
                }
            }
            break;
        }
    }
    puzzle[row][col] = EMPTY_VALUE;
    if (visualize)
    {
        sleep(100);
        printPuzzle(puzzle);
    }
    return false;
}

void generatePuzzle(int (&puzzle)[PUZZLE_SIZE][PUZZLE_SIZE], int difficulty = 1)
{
    for (int i = 0; i < PUZZLE_SIZE; i++)
    {
        for (int j = 0; j < PUZZLE_SIZE; j++)
        {
            puzzle[i][j] = EMPTY_VALUE;
        }
    }

    // Start Remove Cells From a FullFilled Sudoku Puzzle
    fillPuzzle(puzzle);
    srand((unsigned)time(0));
    int attempt = difficulty;
    while (attempt > 0)
    {
        // Generate Random Values Between 0 to 8
        int row = floor(rand() % PUZZLE_SIZE);
        int col = floor(rand() % PUZZLE_SIZE);
        while (puzzle[row][col] == EMPTY_VALUE)
        {
            row = floor(rand() % PUZZLE_SIZE);
            col = floor(rand() % PUZZLE_SIZE);
        }
        int backupValue = puzzle[row][col];
        puzzle[row][col] = EMPTY_VALUE;
        numberofSolution = 0;
        solveSudoku(puzzle);

        // if by set this cell to empty, the sudoku puzzle will not have only one solution then we're not going to remove the cell
        if (numberofSolution != 1)
        {
            puzzle[row][col] = backupValue;
            attempt--;
        }
    }
}

int main(int, char **)
{
    cout << "1. Do You Want To Generate Sudoku\n";
    cout << "2. Do You Want To Enter Suodku\n";
    int task;
    cin >> task;
    if (task == 1)
    {
        int puzzle[PUZZLE_SIZE][PUZZLE_SIZE];
        int origin[PUZZLE_SIZE][PUZZLE_SIZE];
        int diff;
        cout << "Enter Difficulty Level Between 1 to 5: ";
        cin >> diff;
        if (diff > 5 || diff < 1)
        {
            cout << "Enter Difficulty Level In Between Between 1 to 5 Only!";
            return 0;
        }
        generatePuzzle(puzzle, diff);
        copyPuzzle(puzzle, origin);
        system("cls");
        printPuzzle(puzzle);
        string run;
        cout << "Do You Want To Run Algorithm To Solve This Puzzle? (Y/n) ";
        cin >> run;
        if (run == "n" || run == "N")
        {
            cout.flush();
            return 0;
        }
        solveSudoku(puzzle, true);
        cout.flush();
        return 0;
    }
    else if (task == 2)
    {
        cout<<"Enter Your Sudoku\n";
        int puzzle[PUZZLE_SIZE][PUZZLE_SIZE];
        for (int i = 0; i < PUZZLE_SIZE; i++)
        {
            string s;
            cin >> s;
            for (int j = 0; j < PUZZLE_SIZE; j++)
            {
                if (s[j] == '.')
                {
                    puzzle[i][j] = EMPTY_VALUE;
                }
                else
                {
                    puzzle[i][j] = s[j] - '0';
                }
            }
        }
        system("cls");
        printPuzzle(puzzle);
        string run;
        cout << "Do You Want To Run Algorithm To Solve This Puzzle? (Y/n) ";
        cin >> run;
        if (run == "n" || run == "N")
        {
            cout.flush();
            return 0;
        }
        solveSudoku(puzzle, true);
        cout.flush();
        return 0;
    }
    else
    {
        cout<<"Enter Proper Choice!";
        return 0;
    }
}