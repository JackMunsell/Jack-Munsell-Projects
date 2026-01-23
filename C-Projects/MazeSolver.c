/*
 * Maze Solver Program
 * ------------------------------------------------------------
 * DESCRIPTION:
 *  This program loads a maze represented as a grid and finds the
 *  shortest path between a start and end position using both
 *  Dijkstra's Algorithm and the A* pathfinding algorithm.
 *
 * HOW TO COMPILE:
 *      gcc MazeSolver.c -o maze_solver
 *
 * HOW TO RUN:
 *      ./maze_solver
 *
 * PROGRAM FLOW:
 *  - Loads or builds a maze grid
 *  - Runs Dijkstra’s Algorithm
 *  - Runs A* Algorithm
 *  - Compares path length / cost
 *  - Prints the final path and results
 *
 * NOTES:
 *  - Obstacles are treated as blocked cells.
 *  - The algorithms reconstruct the path using parent pointers.
 *  - Execution time is measured to compare performance.
 *
 * Author: Jack Munsell
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <time.h>

// Both must be odd numbers
int ROWS, COLS;

// Up, Down, Left, Right
int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};


struct Node {
    int row, col;        //Position in the grid
    int g_cost;          //Cost from the start node
    int h_cost;          //Heuristic cost to the goal (used in A*)
    int f_cost;          //Total cost (g_cost + h_cost, only for A*)
    bool visited;        //Whether this node has been processed
    struct Node* parent; //Pointer to the parent node for path reconstruction
};

//Function to calculate Manhattan distance
//This function returns the Manhattan distance between two points (row1, col1) and (row2, col2).
//It is used as a heuristic in the A* algorithm to estimate the remaining distance to the goal.
int heuristic(int row1, int col1, int row2, int col2) {
    return abs(row1 - row2) + abs(col1 - col2);
}

static int randOdd(int low, int high) {
    // Returns a random odd number in [low, high], assumes low/high are valid range bounds.
    int x = low + (rand() % (high - low + 1));
    if (x % 2 == 0) x++;
    if (x > high) x -= 2;
    return x;
}

static void bfsFarthestCell(int **gridMap, int startR, int startC, int* outR, int* outC) {
    int dist[ROWS][COLS];
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) dist[r][c] = -1;
    }

    int qR[ROWS * COLS];
    int qC[ROWS * COLS];
    int head = 0, tail = 0;

    qR[tail] = startR;
    qC[tail] = startC;
    tail++;
    dist[startR][startC] = 0;

    int farR = startR, farC = startC;

    while (head < tail) {
        int r = qR[head];
        int c = qC[head];
        head++;

        if (dist[r][c] > dist[farR][farC]) {
            farR = r;
            farC = c;
        }

        for (int i = 0; i < 4; i++) {
            int nr = r + directions[i][0];
            int nc = c + directions[i][1];

            if (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLS &&
                gridMap[nr][nc] != 1 && dist[nr][nc] == -1) {
                dist[nr][nc] = dist[r][c] + 1;
                qR[tail] = nr;
                qC[tail] = nc;
                tail++;
            }
        }
    }

    *outR = farR;
    *outC = farC;
}

void generateMaze(int **gridMap, int rows, int cols) {
    (void)rows; (void)cols;

    // 1) Fill with walls
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            gridMap[r][c] = 1;
        }
    }

    // 2) Pick an odd start cell away from borders
    int startR = randOdd(1, ROWS - 2);
    int startC = randOdd(1, COLS - 2);

    // Carve start
    gridMap[startR][startC] = 0;

    // 3) Iterative DFS stack (stores cell coordinates)
    int stackR[ROWS * COLS];
    int stackC[ROWS * COLS];
    int top = 0;

    stackR[top] = startR;
    stackC[top] = startC;
    top++;

    while (top > 0) {
        int r = stackR[top - 1];
        int c = stackC[top - 1];

        // Collect unvisited neighbors 2 steps away
        int candR[4], candC[4];
        int candCount = 0;

        for (int i = 0; i < 4; i++) {
            int nr = r + 2 * directions[i][0];
            int nc = c + 2 * directions[i][1];

            if (nr >= 1 && nr <= ROWS - 2 && nc >= 1 && nc <= COLS - 2 &&
                gridMap[nr][nc] == 1) {
                candR[candCount] = nr;
                candC[candCount] = nc;
                candCount++;
            }
        }

        if (candCount > 0) {
            // Choose random candidate
            int k = rand() % candCount;
            int nr = candR[k];
            int nc = candC[k];

            // Carve wall between (r,c) and (nr,nc)
            int wallR = (r + nr) / 2;
            int wallC = (c + nc) / 2;

            gridMap[wallR][wallC] = 0;
            gridMap[nr][nc] = 0;

            // Push neighbor
            stackR[top] = nr;
            stackC[top] = nc;
            top++;
        } else {
            // Backtrack
            top--;
        }
    }

    // 4) Mark start
    gridMap[startR][startC] = 2;

    // 5) Pick end as farthest reachable open cell
    int endR, endC;
    bfsFarthestCell(gridMap, startR, startC, &endR, &endC);

    // Ensure we don't overwrite start
    if (gridMap[endR][endC] == 2) {
        // If somehow same (tiny maze), just find any open cell
        for (int r = 1; r < ROWS - 1; r++) {
            for (int c = 1; c < COLS - 1; c++) {
                if (gridMap[r][c] == 0) { endR = r; endC = c; goto done; }
            }
        }
    }
done:
    gridMap[endR][endC] = 3;
}

//Function to initialize the grid
//This function initializes each node in the grid with default values:
//-Position (row, col)
//-Costs set to infinity (INT_MAX)
//-Heuristic cost set to 0
//-Visited flag set to false
//-Parent pointer set to NULL
void initializeGrid(struct Node grid[ROWS][COLS]) {
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            grid[i][j].row = i;
            grid[i][j].col = j;
            grid[i][j].g_cost = INT_MAX;
            grid[i][j].h_cost = 0;
            grid[i][j].f_cost = INT_MAX;
            grid[i][j].visited = false;
            grid[i][j].parent = NULL;
        }
    }
}

//Function to get the node with the lowest cost
//Returns and removes the node with the lowest cost from the open list.
//1. If useHeuristic is true, it considers f_cost (used in A*).
//2. If useHeuristic is false, it considers g_cost (used in Dijkstra's).
struct Node* getLowestCostNode(struct Node* openList[], int* openListSize, bool useHeuristic) {
    int minIndex = 0;
    for (int i = 1; i < *openListSize; i++) {
        int cost = useHeuristic ? openList[i]->f_cost : openList[i]->g_cost;
        int minCost = useHeuristic ? openList[minIndex]->f_cost : openList[minIndex]->g_cost;
        if (cost < minCost) {
            minIndex = i;
        }
    }
    struct Node* minNode = openList[minIndex];
    for (int i = minIndex; i < *openListSize - 1; i++) {
        openList[i] = openList[i + 1];
    }
    (*openListSize)--;
    return minNode;
}

//This function ensures the position (row, col):
//1. Is within the grid boundaries.
//2. Is not a wall (gridMap[row][col] != 1).
bool isValid(int row, int col, int **gridMap) {
    return row >= 0 && row < ROWS && col >= 0 && col < COLS && gridMap[row][col] != 1;
}

//# for walls
//S for the start point
//E for the endpoint
//P for the path
//. for unexplored areas
void printGrid(int **gridMap, struct Node* goal) {
    char displayGrid[ROWS][COLS];

    // Initialize the grid with the map
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (gridMap[i][j] == 1) {
                displayGrid[i][j] = '#'; // Wall
            } else if (gridMap[i][j] == 2) {
                displayGrid[i][j] = 'S'; // Start
            } else if (gridMap[i][j] == 3) {
                displayGrid[i][j] = 'E'; // End
            } else {
                displayGrid[i][j] = '.'; // Path
            }
        }
    }

    //Mark the path with P
    struct Node* current = goal;
    while (current != NULL) {
        if (gridMap[current->row][current->col] == 0) {
            displayGrid[current->row][current->col] = '+';
        }
        current = current->parent;
    }

    //Print the grid
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%c ", displayGrid[i][j]);
        }
        printf("\n");
    }
}

//Finds the shortest path from the start to the goal using only actual distances (g_cost).
void dijkstra(struct Node grid[ROWS][COLS], int startRow, int startCol, int goalRow, int goalCol, int **gridMap) {
    struct Node* openList[ROWS * COLS]; //List of the nodes that were discovered, not yet processed
    int openListSize = 0;

    struct Node* startNode = &grid[startRow][startCol];
    startNode->g_cost = 0;
    openList[openListSize++] = startNode; 

    while (openListSize > 0) { //If openListSize becomes empty (checked every path) before reaching the goal it means there is no valid path
        struct Node* current = getLowestCostNode(openList, &openListSize, false);
        current->visited = true;

        if (current->row == goalRow && current->col == goalCol) { //Checks if the current node is the end node
            printf("Dijkstra's Path:\n");
            printGrid(gridMap, current);
            printf("Shortest distance (Dijkstra's): %d\n", current->g_cost);
            return;
        }

        for (int i = 0; i < 4; i++) { //Iterates between all 4 neighbor nodes
            int newRow = current->row + directions[i][0];
            int newCol = current->col + directions[i][1];

            if (isValid(newRow, newCol, gridMap)) { //Checks if the move is valid/ if the the neighbor node is within the grid boundaries
                struct Node* neighbor = &grid[newRow][newCol];
                if (neighbor->visited) continue;

                int tentativeGCost = current->g_cost + 1; //Adds 1 to the g_cost of the current node since the neighbor has a cost of 1

                if (tentativeGCost < neighbor->g_cost) { //Updates the neighbors cost and parent if a shorter path to the neighbor is found
                    neighbor->g_cost = tentativeGCost;
                    neighbor->parent = current;

                    bool inOpenList = false;
                    for (int j = 0; j < openListSize; j++) { //Checks if the neighbor node is in the openlist, if not it adds it
                        if (openList[j] == neighbor) { 
                            inOpenList = true;
                            break;
                        }
                    }
                    if (!inOpenList) {
                        openList[openListSize++] = neighbor;
                    }
                }
            }
        }
    }

    printf("Dijkstra's: No path found.\n");
}

//Finds the shortest path from the start to the goal using both actual distances (g_cost) and estimated distances (h_cost).
void aStar(struct Node grid[ROWS][COLS], int startRow, int startCol, int goalRow, int goalCol, int **gridMap) {
    struct Node* openList[ROWS * COLS]; //List of the nodes that were discovered, not yet processed
    int openListSize = 0;

    struct Node* startNode = &grid[startRow][startCol];
    startNode->g_cost = 0;
    startNode->h_cost = heuristic(startRow, startCol, goalRow, goalCol);
    startNode->f_cost = startNode->g_cost + startNode->h_cost;
    openList[openListSize++] = startNode;

    while (openListSize > 0) { //If openListSize becomes empty (checked every path) before reaching the goal it means there is no valid path
        struct Node* current = getLowestCostNode(openList, &openListSize, true);
        current->visited = true;

        if (current->row == goalRow && current->col == goalCol) {//Checks if the current node is the end node
            printf("A* Path:\n");
            printGrid(gridMap, current);
            printf("Shortest distance (A*): %d\n", current->g_cost);
            return;
        }

        for (int i = 0; i < 4; i++) { //Iterates between all 4 neighbors
            int newRow = current->row + directions[i][0];
            int newCol = current->col + directions[i][1];

            if (isValid(newRow, newCol, gridMap)) { //Checks if the move is valid/ if the the neighbor node is within the grid boundaries
                struct Node* neighbor = &grid[newRow][newCol];
                if (neighbor->visited) continue;

                int tentativeGCost = current->g_cost + 1; //Adds 1 to the g_cost of the current node since the neighbor has a cost of 1

                if (tentativeGCost < neighbor->g_cost) { //Balances the actual cost and estimated cost to the end
                    neighbor->g_cost = tentativeGCost;
                    neighbor->h_cost = heuristic(newRow, newCol, goalRow, goalCol);
                    neighbor->f_cost = neighbor->g_cost + neighbor->h_cost;
                    neighbor->parent = current;

                    bool inOpenList = false;
                    for (int j = 0; j < openListSize; j++) { //Checks if the neighbor node is in the openlist, if not it adds it
                        if (openList[j] == neighbor) {
                            inOpenList = true;
                            break;
                        }
                    }
                    if (!inOpenList) {
                        openList[openListSize++] = neighbor;
                    }
                }
            }
        }
    }

    printf("A*: No path found.\n");
}

int main() {
    printf("Maze Solver Program\n");
    printf("What size maze would you like to generate? (odd numbers only < 339x339, e.g., 21 21): ");
    scanf("%d %d", &ROWS, &COLS);
    if(ROWS % 2 == 0) ROWS++;
    if(COLS % 2 == 0) COLS++;

    int **gridMap = malloc(ROWS * sizeof(int *));
    for (int i = 0; i < ROWS; i++) {
        gridMap[i] = malloc(COLS * sizeof(int));
    }

    srand((unsigned)time(NULL));
    generateMaze(gridMap, ROWS, COLS);

    struct Node grid1[ROWS][COLS], grid2[ROWS][COLS];
    initializeGrid(grid1);
    initializeGrid(grid2);

    //Finding the start and end points
    int startRow, startCol, goalRow, goalCol;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (gridMap[i][j] == 2) {
                startRow = i;
                startCol = j;
            }
            if (gridMap[i][j] == 3) {
                goalRow = i;
                goalCol = j;
            }
        }
    }

    printf("Starting Grid:\n");
    printGrid(gridMap, NULL);

    clock_t start = clock();

    printf("\nRunning Dijkstra's Algorithm\n");
    dijkstra(grid1, startRow, startCol, goalRow, goalCol, gridMap);

    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Dijkstra's Algorithm Time: %f seconds\n", cpu_time_used);


    start = clock();
    printf("\nRunning A* Algorithm\n");
    aStar(grid2, startRow, startCol, goalRow, goalCol, gridMap);

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("A* Algorithm Time: %f seconds\n", cpu_time_used);
    
    return 0;
}
