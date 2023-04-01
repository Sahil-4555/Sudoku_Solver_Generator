##Sudoku Solver

Run The cpp file Using Command:

`gcc -o sudoku Suodku_Solver.cpp`

The program starts by defining some constants, such as the size of the puzzle and the value assigned to empty cells. It then defines some helper functions for printing the puzzle, checking if a value is valid in a certain position, checking if there are any empty cells, and copying a puzzle.

The fillPuzzle function uses a recursive backtracking algorithm to fill the puzzle with random values. It starts by finding an empty cell and then tries each possible value until it finds one that is valid. If it reaches a dead-end, it backtracks to the previous cell and tries another value.

The solveSudoku function uses the same backtracking algorithm to solve the puzzle. It starts by finding an empty cell and then tries each possible value until it finds one that is valid. If it reaches a dead-end, it backtracks to the previous cell and tries another value. If it has tried all possible values and still hasn't found a solution, it backtracks to the previous cell again.

The program also includes some helper functions for displaying the puzzle and waiting for a certain amount of time. The setCursorPosition function uses the Windows API to set the console cursor position, allowing the program to update the display without flickering. The sleep function uses the std::this_thread::sleep_for function to wait for a certain amount of time.

##Sudoku Generator

<img align="center" alt="GIF" src="assets/choice.png"/>

