#pragma once
#include <SFML/Graphics.hpp>
#include "Grid.h"

class Game 
{
private:
	sf::RenderWindow window;
	Grid grid;

	// View settings
	int cellsize; // Amount of pixels for each cell
	int windowWidth;
	int windowHeight;

	// Timing
	sf::Clock clock;
	float updateInterval;  // seconds between generations
	float timeSinceLastUpdate;

	// Game state
	bool isRunning;
	bool isPaused;
public:
	Game(int gridRows, int gridCols, int cellSize);
	void run();
	void handleInput();
	void update();
	void render();
	void togglePause();
	void clearGrid();
	void randomizeGrid();
};