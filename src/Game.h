#pragma once
#include <SFML/Graphics.hpp>
#include "Grid.h"
#include <vector> 
#include <memory>

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
	float speedMultiplier;
	const float MIN_SPEED = 0.1f;
	const float MAX_SPEED = 10.0f;
	const float SPEED_STEP = 0.1f;

	// Zoom/pan
	sf::View view;
	float viewScale;  // 1.0 = normal, <1 = zoomed in, >1 = zoomed out
	sf::Vector2i lastMousePos;

	// Game state
	bool isRunning;
	bool isPaused;

	// UI Elements
	 sf::Font font;
    std::unique_ptr<sf::Text> speedText;     
    std::unique_ptr<sf::Text> zoomText;      
    std::unique_ptr<sf::Text> populationText;
    std::unique_ptr<sf::Text> pauseText;     
    int uiBarHeight;  // Height of info bar at top
    
    // Color palette
    std::vector<sf::Color> palette;
    int currentPalette;

public:
	Game(int gridRows, int gridCols, int cellSize);
	void run();
	void handleInput();
	void render();
	void togglePause();
	void clearGrid();
	void randomiseGrid();
};