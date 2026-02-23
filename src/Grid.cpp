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
            if(i == 0 && j == 0) continue; // Skip the cell itself
            
            int checkX = x + i;
            int checkY = y + j;
            // Check if out of bounds 
            if(checkX < 0 || checkX >= rows || checkY < 0 || checkY >= cols) {
                continue;
            }            
            if(currentState[checkX][checkY]) {
                count++;
            }                            
        }
    }
    return count;
}

void Grid::update(){
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            int count = countLiveNeighbors(i,j);

            if(currentState[i][j]){
                if(count == 2 || count == 3){
                    nextState[i][j] = true;
                } 
                else{
                    nextState[i][j] = false;
                }
            }
            else {   // Cell is dead
                if(count == 3){
                    nextState[i][j] = true;
                }
                else{
                    nextState[i][j] = false;
                }                
            }         
        }
    }
    std::swap(currentState,nextState);
}
 
bool Grid::isAlive(int x, int y) const {
    return currentState[x][y];
}

void Grid::toggleCell(int x, int y) {
    currentState[x][y] = !currentState[x][y];
}

void Grid::clear() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            currentState[i][j] = false;
        }
    }
}

int Grid::countLiveCells() const {
    int count = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (currentState[i][j]) count++;
        }
    }
    return count;
}
