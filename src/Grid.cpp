#include "Grid.h"
#include <cstdlib> 

Grid::Grid(int pRows, int pCols) {
    Grid::rows = pRows;
    Grid::cols = pCols;
    
    currentState.resize(rows, std::vector<bool>(cols, false));
    nextState.resize(rows, std::vector<bool>(cols, false));
}

void Grid::randomise(){
    for(int i = 0; i < rows; ++i) {
        for(int j = 0; j < cols; ++j) {
            currentState[i][j] = rand() % 2 == 0; // Randomly alive or dead
        }
    }
}

int Grid::countLiveNeighbors(int x, int y) const {
    int count = 0;
    for(int i = -1; i <= 1; i++) {
        for(int j = -1; j <= 1; j++) {
            if((x + i >= 0 && x + i < rows) || (y + i >= 0 && y + i < cols)) continue;
            if(i == 0 && j == 0) continue; // Skip the cell itself
            if(currentState[i][j] == 1) count++;
        }
    }
    return count;
}



