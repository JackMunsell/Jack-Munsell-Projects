#include "raylib.h"

#include <vector>
#include <time.h>
#include <unistd.h>

#define WIDTH 800
#define HEIGHT 600

const float innerRectWidth = 700.0f;
const float innerRectHeight = 500.0f;
const int FPS = 10000;
const int ARRAY_SIZE = 5;
const int MAX_VALUE = 1000;


struct Bar {
    float value;
    Color color;
    float posX;
};

void initVector(std::vector<Bar>& vec, int size, int maxValue) {
    srand(time(NULL));
    for(int i = 0; i < size; i++) {
        Bar bar;
        bar.value = rand() % maxValue + 1;
        bar.color = WHITE;
        bar.posX = (innerRectWidth / size) * i + (WIDTH / 2 - innerRectWidth / 2);
        vec.push_back(bar);
    }
}

void DrawBarField() {
    DrawRectangle(WIDTH / 2 - innerRectWidth / 2, 
                  HEIGHT / 2 - innerRectHeight / 2, 
                  innerRectWidth, 
                  innerRectHeight, 
                  GRAY);
}

void DrawBars(const std::vector<Bar>& vec) {
    for(int i = 0; i < vec.size(); i++) {
        DrawRectangle(vec[i].posX, 
                      (HEIGHT / 2 + innerRectHeight / 2) - vec[i].value * (innerRectHeight / MAX_VALUE), 
                      (innerRectWidth / vec.size()) - 1, 
                      vec[i].value * (innerRectHeight / MAX_VALUE), 
                      vec[i].color);
    }
}


bool isSorted(const std::vector<Bar>& vec) {
    for(int i = 1 ; i < vec.size(); i++) {
        if(vec[i-1].value > vec[i].value) {
            return false;
        }
    }
    return true;
}

void selectionSort(std::vector<Bar>& vec) {
    for(int i = 0; i < vec.size() - 1; i++) {
        int min_idx = i;
        for(int j = 0; j < vec.size() - 1; j++) {
            if(vec[j].value < vec[min_idx].value) {
                min_idx = j;
            }
        }
        std::swap(vec[i], vec[min_idx]);
    }
}

void merge(std::vector<Bar>& vec, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    std::vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = vec[left + i].value;
    for (int j = 0; j < n2; j++)
        R[j] = vec[mid + 1 + j].value;

    int i = 0, j = 0;
    int k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            vec[k].value = L[i];
            i++;
        }
        else {
            vec[k].value = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        vec[k].value = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        vec[k].value = R[j];
        j++;
        k++;
    }
}

bool bubbleSortStep(std::vector<Bar>& vec) {
    static int i = 0;
    static int j = 0;
    static bool swappedThisPass = false;
    static bool done = false;

    if (done) return true;

    for (auto& b : vec) b.color = WHITE;

    if (i >= (int)vec.size() - 1) {
        done = true;
        return done;
    }

    vec[j].color = GREEN;
    vec[j + 1].color = RED;

    if (vec[j].value > vec[j + 1].value) {
        std::swap(vec[j], vec[j + 1]);
        std::swap(vec[j].posX, vec[j + 1].posX);
        swappedThisPass = true;
    }

    j++;
    if (j >= (int)vec.size() - 1 - i) {
        if (!swappedThisPass) {
            done = true;
            return done;
        }
        swappedThisPass = false;
        j = 0;
        i++;
    }
    return done;
}

bool selectionSortStep(std::vector<Bar>& vec) {
    static int i = 0;
    static int j = 0;
    static int minIndex = 0;
    static bool scanning = false;
    static bool done = false;

    if(done) return done;

    for (auto& b : vec) b.color = WHITE;

    if(i >= (int)vec.size() - 1) {
        done = true;
        return done;
    }

    if(!scanning) {
        minIndex = i;
        j = i + 1;
        scanning = true;
    }

    vec[i].color = BLUE;
    vec[minIndex].color = GREEN;

    if(j < (int)vec.size()) {
        vec[j].color = RED;

        if(vec[j].value < vec[minIndex].value) {
            minIndex = j;
        }
        j++;
    } 
    else {
        if(minIndex != i) {
            std::swap(vec[i], vec[minIndex]);
            std::swap(vec[i].posX, vec[minIndex].posX);
        }

        i++;
        scanning = false;
    }
    return done;
}

bool BogoSortStep(std::vector<Bar> &vec) {
    if(isSorted(vec)) return true;

    for(auto& b : vec) b.color = WHITE;
    int randomIndex1 = rand() % vec.size();
    int randomIndex2 = (rand() * rand()) % vec.size();
    std::swap(vec[randomIndex1], vec[randomIndex2]);
    std::swap(vec[randomIndex1].posX, vec[randomIndex2].posX);
    return false;
}

int main() {
    InitWindow(WIDTH, HEIGHT, "Sorting Visualizer");
    SetTargetFPS(FPS);

    long int iterations = 0;
    bool finished = false;

    std::vector<Bar> values;
    initVector(values, ARRAY_SIZE, MAX_VALUE);
    std::string str, str1;

    double startTime = GetTime();
    double finalTime = 0.0;
    bool timerStopped = false;

    while (!WindowShouldClose()) {
        if(!isSorted(values)) {
            str1 = "Time Elapsed: " + std::to_string(GetTime()) + " seconds";
        }
        if(!timerStopped) {
            // finished = bubbleSortStep(values);
            // finished = selectionSortStep(values);
            finished = BogoSortStep(values);
            // finished = BucketSortStep(values);
            if (finished) {
                finalTime = GetTime() - startTime;
                timerStopped = true;
                str = "Sorting Completed in: " + std::to_string(finalTime) + " seconds";
            } else iterations++;
        }

        BeginDrawing();
            ClearBackground(BLACK);
            DrawBarField();
            DrawBars(values);
            DrawFPS(WIDTH - 100, 20);

            if(isSorted(values)) {
                for(auto& b : values) b.color = GREEN;
                DrawText(str.c_str(), (WIDTH / 2) - (innerRectWidth / 2), 10, 20, WHITE);
            } else 
                DrawText(str1.c_str(), (WIDTH / 2) - (innerRectWidth / 2), 10, 20, WHITE);
            
            std::string iter = "Iterations: " + std::to_string(iterations);
            DrawText(iter.c_str(), (WIDTH / 2) - (innerRectWidth / 2), HEIGHT - 20, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
