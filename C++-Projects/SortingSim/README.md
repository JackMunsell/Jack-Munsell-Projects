# Sorting Visualizer

  This program visualizes how sorting algorithms work using bars on the screen.  
  Each bar represents a value, and the program shows how the values move as they get sorted.

## How to Run
  cmake -S . -B Build
  cmake --build Build
  ./Build/SortingSim

## Modifiable Code
Line 12 - "const int FPS = 60;"
  - Changes how fast the algorithm runs, 60fps is recommended
Line 13 - "const int ARRAY_SIZE = 5;"
  - Changes the number of elements in the array
Line 14 - "const int MAX_VALUE = 1000;"
  - Changes the maximum value for any element in the array

Line 226-229 -
  // finished = bubbleSortStep(values);
  // finished = selectionSortStep(values);
  // finished = BogoSortStep(values);
  // finished = BucketSortStep(values); Not Yet Implemented
  - Comment in which algorithm you want to use
  - For bogo sort, I don't recommend going more than 10 elements, as it doesn't have a consistent time complexity (it took 10+ minutes at 3000fps for it to solve, aka 500 hours at 60fps)
