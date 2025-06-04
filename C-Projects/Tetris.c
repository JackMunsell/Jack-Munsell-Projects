#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<time.h>

const int WIDTH = 12;
const int HEIGHT = 21;


// Functions needed:
// 1. Rotate

// Other things needed:
// 2. Need to hold store the current highest piece so the rotate function works

void CheckRows(int grid[HEIGHT][WIDTH], int *score) {
    for(int row = HEIGHT - 2; row > 0; row--) { 
        bool IsFullRow = true;

        for (int col = 1; col < WIDTH - 1; col++) {
            if (grid[row][col] == 0) {
                IsFullRow = false;
                break;
            }
        }

        if(IsFullRow) {
            for(int r = row; r > 0; r--) {
                for(int col = 1; col < WIDTH - 1; col++) {
                    grid[r][col] = grid[r - 1][col];                }
            }

            for(int col = 1; col <  WIDTH -1; col++) {
                grid[0][col] = 0;
            }
                
            row++;
            *score += 100;
        }
    }
}

bool CanPlaceShape(int grid[HEIGHT][WIDTH]) {
    for(int row = 0; row < 4; row++) {
        for(int col = 4; col < 9; col++) {
            if(grid[row][col] == 1) {
                return false;
            }
        }
    }
    return true;
}

bool CanMoveDown(int grid[HEIGHT][WIDTH]) {
    for(int row = HEIGHT - 1; row >= 0; row--) {
        for(int col = 0; col < WIDTH; col++) {
            if(grid[row][col] == 6) {
                if(row == HEIGHT -1 || grid[row + 1][col] == 1 || grid[row + 1][col] == 3) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool CanMoveRight(int grid[HEIGHT][WIDTH]) {
    for(int row = 0; row < HEIGHT;row++) {
        for(int col = WIDTH - 1; col >= 1; col--) {
            if(grid[row][col] == 6) {
                if(col == WIDTH - 1 || grid[row][col + 1] == 1 || grid[row][col + 1] == 2) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool CanMoveLeft(int grid[HEIGHT][WIDTH]) {
    for(int row = 0; row < HEIGHT;row++) {
        for(int col = 0; col < WIDTH; col++) {
            if(grid[row][col] == 6) {
                if(col == 0 || grid[row][col - 1] == 1 || grid[row][col - 1] == 2) {
                    return false;
                }
            }
        }
    }
    return true;
}

void SquareShape(int grid[HEIGHT][WIDTH]) {
    grid[0][5] = 6;
    grid[0][6] = 6;
    grid[1][5] = 6;
    grid[1][6] = 6;
}
void RectangleShape(int grid[HEIGHT][WIDTH]) {
    grid[0][4] = 6;
    grid[0][5] = 6;
    grid[0][6] = 6;
    grid[0][7] = 6;
}
void SShape(int grid[HEIGHT][WIDTH]) {
    grid[0][5] = 6;
    grid[0][6] = 6;
    grid[1][4] = 6;
    grid[1][5] = 6;
}
void ZShape(int grid[HEIGHT][WIDTH]) {
    grid[0][5] = 6;
    grid[0][6] = 6;
    grid[1][6] = 6;
    grid[1][7] = 6;
}
void LShape(int grid[HEIGHT][WIDTH]) {
    grid[0][5] = 6;
    grid[0][6] = 6;
    grid[1][6] = 6;
    grid[2][6] = 6;
}
void JShape(int grid[HEIGHT][WIDTH]) {
    grid[0][5] = 6;
    grid[0][6] = 6;
    grid[1][5] = 6;
    grid[2][5] = 6;
}
void TShape(int grid[HEIGHT][WIDTH]) {
    grid[0][6] = 6;
    grid[1][5] = 6;
    grid[1][6] = 6;
    grid[2][6] = 6;
}


void spawnpiece(int grid[HEIGHT][WIDTH], int num) {
    bool addshape = false;

    for(int row = 0; row < HEIGHT; row++) { 
        for(int col = 0; col < WIDTH; col++) {
            if(grid[row][col] == 6) {
                return;
            }
        }
    }
    
    switch(num) {
        case 0:
            SquareShape(grid);
            break;
        case 1:
            RectangleShape(grid);
            break;
        case 2:
            SShape(grid);
            break;
        case 3:
            ZShape(grid);
            break;
        case 4:
            LShape(grid);
            break;
        case 5:
            JShape(grid);
            break;
        case 6:
            TShape(grid);
            break;
    }
    
}

// Print function
// 0 is play area
// 1 is block
// 2 is vertical border wall
// 3 is horizontal border wall
// 4 is the bottom left corner
// 5 is the bottom right corner
void printBoard(int grid[HEIGHT][WIDTH]) {
    for(int row = 0 ; row < HEIGHT; row++) {
        for(int col = 0 ; col < WIDTH ; col++) {
            if(grid[row][col] == 0) {
                printf(" . ");
            }
            if(grid[row][col] == 1) {
                printf(" ■ ");
            }
            if(grid[row][col] == 2) {
                printf("|");
            }
            if(grid[row][col] == 3) {
                printf("---");
            }
            if(grid[row][col] == 4) {
                printf("└");
            }
            if(grid[row][col] == 5) {
                printf("┘");
            }
            if(grid[row][col] == 6) {
                printf(" # ");
            }
        }
        printf("\n");
    }
}




int main() {
    system("clear");
   
    srand(time(NULL));

    int score = 0;
    int piecenum;
    

    int grid[HEIGHT][WIDTH] = {
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
        {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2},
        {4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 5}
    };

    piecenum = rand() % 7;
    spawnpiece(grid, piecenum);

    int playing = 1;
    while(playing != 0) {
        system("clear");
        

        printBoard(grid);
        printf("---------Actions---------\n");
        printf("- s to move down\n");
        printf("- d to move right\n");
        printf("- a to move left\n");
        printf("- r to rotate the piece\n");
        printf("- e to end the game\n");
        printf("-------------------------\n");
        printf("Your score is %d\n", score);
        

        char playermove = 'd';
        printf("Enter Your Choice: ");
        scanf(" %c", &playermove);

        switch(playermove){
            case 's':
                if(CanMoveDown(grid)) {
                    for(int row = HEIGHT - 1; row >= 0; row--) {
                        for(int col = 0; col < WIDTH; col++) {
                            if(grid[row][col] == 6){
                                grid[row + 1][col] = 6;
                                grid[row][col] = 0;
                            }
                        }
                    }
                } else {
                    for(int row = 0; row < HEIGHT; row++) {
                        for(int col = 0; col < WIDTH; col++) {
                            if(grid[row][col] == 6) {
                                grid[row][col] = 1;
                            }
                        }
                    }
                }

                CheckRows(grid, &score);

                if (!CanPlaceShape(grid)) {
                    printf("Game Over! No space to place a new shape.\n");
                    printf("Final score: %d\n", score);
                    playing = 0;
                } else {
                    piecenum = rand() % 7;
                    spawnpiece(grid, piecenum);
                }
                break;
            case 'd':
                if(CanMoveRight(grid)) {
                    for(int row = 0; row <HEIGHT; row++) {
                        for(int col = WIDTH - 1; col >= 1; col--) {
                            if(grid[row][col] == 6) {
                                grid[row][col + 1] = 6;
                                grid[row][col] = 0;
                            }
                        }
                    }
                }
                break;
            case 'a':
                if(CanMoveLeft(grid)) {
                    for(int row = 0; row <HEIGHT; row++) {
                        for(int col = 0; col < WIDTH; col++) {
                            if(grid[row][col] == 6) {
                                grid[row][col - 1] = 6;
                                grid[row][col] = 0;
                            }
                        }
                    }
                }
                break;
            case 'e':
                printf("Ending Game!\n");
                printf("Your final score is %d\n", score);
                playing = 0;
                break;
            case 'r':

                break;
            default:

                break;
        }
    }
}