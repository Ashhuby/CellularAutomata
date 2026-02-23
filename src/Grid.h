#pragma once
#include <vector>

class Grid
{
private:
    std::vector<std::vector<bool>> currentState;
    std::vector<std::vector<bool>> nextState;
    int rows;
    int cols;
public:
    Grid(int rows, int cols);
    void randomise();                                    // fills grid with random alive/dead cells
    int countLiveNeighbors(int x, int y) const;         // counts alive cells around (x,y)
    void update();                                     // moves to next generation
    bool isAlive(int x, int y) const;                 // for drawing
    void toggleCell(int x, int y);                   // for mouse interaction
    void clear();                                   // sets all cells to dead
    int getRows() const { return rows; }           
    int getCols() const { return cols; }  
    int countLiveCells() const;        
};