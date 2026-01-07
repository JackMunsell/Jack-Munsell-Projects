/*
 * Sudoku Game and Solver
 * ------------------------------------------------------------
 * DESCRIPTION:
 *  Interactive Sudoku program with multiple difficulty levels,
 *  validation rules, hints, and an optional AI solver.
 *
 * HOW TO COMPILE:
 *      gcc sudoku.c -o sudoku
 *
 * HOW TO RUN:
 *      ./sudoku
 *
 * GAME OPTIONS:
 *  1) Play Sudoku without hints
 *  2) Play Sudoku with hint suggestions
 *  3) Let the AI automatically solve the puzzle
 *
 * GAME FLOW:
 *  - Program generates a valid completed Sudoku board
 *  - Cells are removed depending on difficulty
 *  - Player enters row, column, and number values
 *  - Input is checked for validity and Sudoku rules
 *  - Program detects win condition
 *
 * TECHNICAL NOTES:
 *  - Uses backtracking recursion for solving
 *  - Ensures all puzzles remain solvable
 *  - Includes hint system + AI solver
 *
 * Author: Jack Munsell
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h> 

#define N 9
#define SUBGRID_SIZE 3

void AhardSudokuBoard(int grid[N][N])
{
 int HardSudokuBoard[N][N] = {
    {5, 3, 0, 0, 7, 0, 0, 0, 0},
    {6, 0, 0, 1, 9, 5, 0, 0, 0},
    {0, 9, 8, 0, 0, 0, 0, 6, 0},
    {8, 0, 0, 0, 6, 0, 0, 0, 3},
    {4, 0, 0, 8, 0, 3, 0, 0, 1},
    {7, 0, 0, 0, 2, 0, 0, 0, 6},
    {0, 6, 0, 0, 0, 0, 2, 8, 0},
    {0, 0, 0, 4, 1, 9, 0, 0, 5},
    {0, 0, 0, 0, 8, 0, 0, 7, 9}
 };
 for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            grid[i][j] = HardSudokuBoard[i][j];
}

void AhardSudokuBoardSolution(int grid[N][N])
{
  int HardSudokuBoardSolution[N][N] = {
    {5, 3, 4, 6, 7, 8, 9, 1, 2},
    {6, 7, 2, 1, 9, 5, 3, 4, 8},
    {1, 9, 8, 3, 4, 2, 5, 6, 7},
    {8, 5, 9, 7, 6, 1, 2, 4, 3},
    {4, 2, 6, 8, 5, 3, 7, 9, 1},
    {7, 1, 3, 9, 2, 4, 8, 5, 6},
    {9, 6, 1, 5, 3, 7, 2, 8, 4},
    {2, 8, 7, 4, 1, 9, 6, 3, 5},
    {3, 4, 5, 2, 8, 6, 1, 7, 9}
 };

 grid=HardSudokuBoardSolution;
}


void AEasySudokuBoard(int grid[N][N]) {
    int EasySudokuBoard[N][N] = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };

    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            grid[i][j] = EasySudokuBoard[i][j];
}


void AEasySudokuBoardSolution(int grid[N][N])
{
    int EasySudokuBoardSolution[N][N] = {
    {5, 3, 4, 6, 7, 8, 9, 1, 2},
    {6, 7, 2, 1, 9, 5, 3, 4, 8},
    {1, 9, 8, 3, 4, 2, 5, 6, 7},
    {8, 5, 9, 7, 6, 1, 2, 4, 3},
    {4, 2, 6, 8, 5, 3, 7, 9, 1},
    {7, 1, 3, 9, 2, 4, 8, 5, 6},
    {9, 6, 1, 5, 3, 7, 2, 8, 4},
    {2, 8, 7, 4, 1, 9, 6, 3, 5},
    {3, 4, 5, 2, 8, 6, 1, 7, 9}};
    
    grid=EasySudokuBoardSolution;
}

// Function to print the grid
void printGrid(int grid[N][N]) {
     printf("  0 1 2|3 4 5|6 7 8 \n");
    for (int row = 0; row < N; row++) {
        if (row % SUBGRID_SIZE == 0)
	    printf(" +-----+-----+-----+\n"); 
	    
        for (int col = 0; col < N; col++)
	{
	 if (col==0)
	   printf("%d|",row); 
	 if (col > 0 && col % SUBGRID_SIZE ==0)
	   printf("|");
	 if (col % SUBGRID_SIZE == SUBGRID_SIZE-1)  
	   {
	    if (grid[row][col]!=0) 
              printf("%d", grid[row][col]);  
	     else
	      printf("."); 
	   }
	   else
	    if (grid[row][col]!=0)
	     printf("%d ", grid[row][col]); 
	    else
	     printf(". ");
	 if (col==N-1)
	   printf("|");
	}
        printf("\n");
    }
    printf(" +-----+-----+-----+\n");  
}

int isSafe(int board[N][N], int row, int col, int num) {
    // Check row
    for (int i = 0; i < N; i++) {
        if (board[row][i] == num) {
            return 0; // Not safe
        }
    }

    // Check column
    for (int i = 0; i < N; i++) {
        if (board[i][col] == num) {
            return 0; // Not safe
        }
    }

    // Check 3x3 box
    int boxRowStart = row - row % SUBGRID_SIZE;
    int boxColStart = col - col % SUBGRID_SIZE;
    for (int i = 0; i < SUBGRID_SIZE; i++) {
        for (int j = 0; j < SUBGRID_SIZE; j++) {
            if (board[boxRowStart + i][boxColStart + j] == num) {
                return 0; // Not safe
            }
        }
    }

    return 1; // Safe
}

void shuffle(int *array, int size) 
{ 
  for (int i = size - 1; i > 0; i--) 
   { 
    int j = rand() % (i + 1); 
    int temp = array[i]; 
    array[i] = array[j]; array[j] = temp; 
   } 
}

int isOnlyPlacementInBlock(int board[N][N], int row, int col, int num) {
    // Check if the number is in the row or column
    for (int i = 0; i < N; i++) {
        if (board[row][i] == num || board[i][col] == num) {
            return 0; // Number exists in row or column
        }
    }

    // Check 3x3 block to see if there's only one valid position for num
    int boxRowStart = row - row % SUBGRID_SIZE;
    int boxColStart = col - col % SUBGRID_SIZE;
    int count = 0; // Count valid positions for num in the block
    int validRow = -1, validCol = -1; // Store the position if valid

    for (int i = 0; i < SUBGRID_SIZE; i++) {
        for (int j = 0; j < SUBGRID_SIZE; j++) {
            int r = boxRowStart + i;
            int c = boxColStart + j;
            if (board[r][c] == 0 && isSafe(board, r, c, num)) {
                count++;
                validRow = r;
                validCol = c;
            }
        }
    }

    // If count is 1, it means num can only go in one position in the block
    return (count == 1) ? 1 : 0;
}

// Function to fill the Sudoku board
int fillSudoku(int board[N][N]) {
    // Find an empty cell
    int row, col;
    for (row = 0; row < N; row++) {
        for (col = 0; col < N; col++) {
            if (board[row][col] == 0) {
                break;
            }
        }
        if (col < N) {
            break;
        }
    }

    // If no empty cell, board is filled
    if (row == N) {
        return 1;
    }

    int numbers[N] = {1,2,3,4,5,6,7,8,9};
    shuffle(numbers,N);
    
    // Try filling the cell with numbers 1 to 9
    for (int i=0; i<N; i++) {
         int num=numbers[i];
        if (isSafe(board, row, col, num)) {
            board[row][col] = num;

            if (fillSudoku(board)) {
                return 1;
            }

            board[row][col] = 0; // Backtrack if solution not found
        }
    }

    return 0;
}

void removeCells(int board[N][N], int difficulty) 
{ 
  int totalCells = N * N; 
  int cellsToRemove; // Set the number of cells to remove based on difficulty level 
  switch (difficulty) 
  { 
    case 1:  cellsToRemove = 35; break; // Easy 
    case 2:  cellsToRemove = 45; break; // Medium 
    case 3:  cellsToRemove = 55; break; // Hard 
    case 4:  cellsToRemove = 60; break; // Expert 
    default: cellsToRemove = 35; break; // Default to easy 
    break; 
   } 
   while (cellsToRemove > 0) 
   { 
    int row = rand() % N; 
    int col = rand() % N; // Only remove the cell if it's not already removed 
    if (board[row][col] != 0) 
    { 
     board[row][col] = 0; cellsToRemove--; 
    } 
   } 
}

bool isSolved(int grid[N][N])
{
 for(int row=0; row<N; row++)
   for(int col=0; col<N; col++)
     if (grid[row][col]==0)
        return false;

 return true;
}

void findPossibleValues(int grid[N][N], int row, int col) {
    if (grid[row][col] != 0) {
        printf("Cell (%d, %d) is already filled with %d\n", row, col, grid[row][col]);
        return;
    }

    printf("Possible values for cell (%d, %d): ", row, col);
    bool found = false;
    for (int num = 1; num <= 9; num++) {
        if (isSafe(grid, row, col, num)) {
            printf("%d ", num);
            found = true;
        }
    }
    if (!found) {
        printf("No possible values");
    }
    printf("\n");
}

bool isSolvable(int board[N][N]) {
    int tempBoard[N][N];
    // Copy the original board to tempBoard
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            tempBoard[i][j] = board[i][j];
        }
    }

    // Find an empty cell
    int row, col;
    bool emptyCellFound = false;
    for (row = 0; row < N; row++) {
        for (col = 0; col < N; col++) {
            if (tempBoard[row][col] == 0) {
                emptyCellFound = true;
                break;
            }
        }
        if (emptyCellFound) {
            break;
        }
    }

    // If no empty cell, the board is solved
    if (!emptyCellFound) {
        return true;
    }

    // Try numbers 1 to 9 in the empty cell
    for (int num = 1; num <= 9; num++) {
        if (isSafe(tempBoard, row, col, num)) {
            tempBoard[row][col] = num;

            // Recursively check if the board can be solved
            if (isSolvable(tempBoard)) {
                return true;
            }

            // Backtrack if not solvable
            tempBoard[row][col] = 0;
        }
    }

    return false; // No solution found
}

void hintBot(int grid[N][N]){
    for(int i = 0; i <= 8; i++){
        for (int j = 0; j <= 8; j++){
            for(int k = 1; k < 10; k++){
                if (grid[i][j]==0){
                    findPossibleValues(grid,i,j);
                    return;
                }
            } 
        }
    }
}

bool aiBot(int grid[N][N]){
    int row, col;
    bool isEmpty = false;
    for (row = 0; row < N; row++){
        for (col = 0; col < N; col++){
            if (grid[row][col] == 0){
                isEmpty = true;
                break;
            }
        }
        if (isEmpty) break;
    }
    if (!isEmpty) return true;
    for (int num = 1; num <= 9; num++){
        if (isSafe(grid, row, col, num)){
            grid[row][col] = num;
            printGrid(grid);
            if (aiBot(grid)) return true;
            grid[row][col] = 0;
        }
    }
    return false;
}



// Main function
int main() {  
 	 
    struct timespec ts;

    clock_t start = clock(), end;

    if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
        perror("clock_gettime");
        return EXIT_FAILURE;
    }

    srand(ts.tv_nsec); 
	
    int grid_solved[N][N] = {0}; // Initialize a 9x9 grid 
    int grid_unsolved[N][N] = {0}; //remove a few numbers 
         
    printf("Sudoku Puzzle:\n");

    int difficulty=-1;
    
    while(difficulty<1 || difficulty>4)
    {
     printf("Type in difficulty number '1'(easy), '2'(medium), '3'(hard), '4'(expert) here: ");
     scanf("%d",&difficulty);
    }
 
    int count=0;
    int try_max_nr_boards=100;
    
    fillSudoku(grid_solved);  // Create Sudoku board
    
    while(1)
    {
     for(int i=0; i<N; i++)
      for(int j=0; j<N; j++)
       grid_unsolved[i][j]=grid_solved[i][j];
     
     removeCells(grid_unsolved,difficulty); 

     if (isSolvable(grid_unsolved))
        break;
     
     count++;  
     if (count==try_max_nr_boards)
         break; 
    }
    
    int row=-1,col=-1,number=-1;
    bool valid_input=false;

    int playerchoice = 1;

    printf("1. To play sudoku without hints.\n");
    printf("2. To play sudoku with hints\n");
    printf("3. To let the AI solve it\n");
    
    scanf("%d", &playerchoice);
    
    if(isSolvable(grid_unsolved)){
        switch(playerchoice){
            case 1:
                printf("You chose playing without hints.\n");
                while(!isSolved(grid_unsolved))
                {
                    printf("\n");
                    printGrid(grid_unsolved);
                    printf("row (0 to 8): ");
                    scanf("%d",&row);
                    printf("col (0 to 8): ");
                    scanf("%d",&col);
                    printf("num (1 to 9): ");
                    scanf("%d",&number);
                    if (row>=0 && row<=8 && col>=0 && col<=8) 
                    if (number>0 && number<10) 
                        if (grid_unsolved[row][col]==0)
                        {
                        if (isSafe(grid_unsolved,row,col,number))
                        {
                    if(grid_solved[row][col]==number)
                    {
                        printf("Valid move, nice!!!\n");
                            grid_unsolved[row][col]=number;
                        valid_input=true;
                    } else
                    {
                        printf("BUG!\n");
                    }
                } 
                }
                
                if (!valid_input)
                    printf("Error: Your input was invalid. Try again. \n");
                
                valid_input=false;
                }
                printGrid(grid_unsolved);
                break;
            case 2:
                printf("You chose playing with hints.\n");
                while(!isSolved(grid_unsolved))
                {
                    printf("\n");
                    printGrid(grid_unsolved);
                    hintBot(grid_unsolved);
                    printf("row (0 to 8): ");
                    scanf("%d",&row);
                    printf("col (0 to 8): ");
                    scanf("%d",&col);
                    printf("num (1 to 9): ");
                    scanf("%d",&number);
                    if (row>=0 && row<=8 && col>=0 && col<=8) 
                    if (number>0 && number<10) 
                        if (grid_unsolved[row][col]==0)
                    {
                        if (isSafe(grid_unsolved,row,col,number))
                    {
                    if(grid_solved[row][col]==number)
                    {
                        printf("Valid move, nice!!!\n");
                            grid_unsolved[row][col]=number;
                        valid_input=true;
                    } else
                    {
                        printf("BUG!\n");
                }
                } 
                }
                
                if (!valid_input)
                    printf("Error: Your input was invalid. Try again. \n");
                
                valid_input=false;
                }
                printGrid(grid_unsolved);
                break;
            case 3:
                printf("You chose to let the AI solve it.\n");
                printGrid(grid_unsolved);
                while(!isSolved(grid_unsolved)){
                    aiBot(grid_unsolved);
                }
                break;

            default: //without hints
                printf("You are playing without hints.\n");
                while(!isSolved(grid_unsolved))
                {
                printf("\n");
                printGrid(grid_unsolved);
                printf("row (0 to 8): ");
                scanf("%d",&row);
                printf("col (0 to 8): ");
                scanf("%d",&col);
                printf("num (1 to 9): ");
                scanf("%d",&number);
                if (row>=0 && row<=8 && col>=0 && col<=8) 
                if (number>0 && number<10) 
                    if (grid_unsolved[row][col]==0)
                {
                    if (isSafe(grid_unsolved,row,col,number))
                {
                if(grid_solved[row][col]==number)
                {
                    printf("Valid move, nice!!!\n");
                        grid_unsolved[row][col]=number;
                    valid_input=true;
                } else
                {
                    printf("BUG!\n");
                }
                } 
                }
                
                if (!valid_input)
                    printf("Error: Your input was invalid. Try again. \n");
                
                valid_input=false;
                }
                printGrid(grid_unsolved);
                break;
        }
        printf("\n");
        printGrid(grid_unsolved);
        printGrid(grid_solved);
        if(isSolved(grid_unsolved)){
            printf("\nYOU WON, CONGRATULATIONS!!! \n");
            end = clock();
            double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
            printf("Time taken: %.2f seconds\n", time_spent);
        }else{
            printf("Grid is not solved\n");
        }
        
    }else{
        printf("Board is unsolvable.");
    }

    printGrid(grid_unsolved);

    for(int i=0; i<9; i++)
       printf("\a");
    
    return 0;
}

