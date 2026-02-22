#include "Game.h"

Game::Game(int pGridRows, int pGridCols, int pCellSize) 
	: grid(pGridRows, pGridCols),
    cellsize(pCellSize),
    windowWidth(pGridCols* pCellSize),
    windowHeight(pGridRows* pCellSize),
    updateInterval(0.1f),
    timeSinceLastUpdate(0.0f),
    isRunning(true),
    isPaused(true)
{
    window.create(sf::VideoMode({ (unsigned int)windowWidth, (unsigned int)windowHeight }), "Game of Life");
}

void Game::run() {
    while (window.isOpen())  
    {
        handleInput();

        float deltaTime = clock.restart().asSeconds();

         if (!isPaused) {
            timeSinceLastUpdate += deltaTime;
            while (timeSinceLastUpdate >= updateInterval) {
                grid.update();
                timeSinceLastUpdate -= updateInterval;
            }
        }

        render();
    }
}

void Game::handleInput(){
     while (auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        
        // Key presses
        if (auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
            
            if (keyEvent->scancode == sf::Keyboard::Scancode::Space) {
                // Toggle pause
                togglePause();
            }
            else if (keyEvent->scancode == sf::Keyboard::Scancode::C) {
                // Clear grid
                clearGrid();
            }
            else if (keyEvent->scancode == sf::Keyboard::Scancode::R) {
                // Randomize grid
                randomiseGrid();
            }
        }

        // Mouse Presses
        if (auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseEvent->button == sf::Mouse::Button::Left) {
                // Get mouse position in window
                sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                
                // Convert to grid coordinates
                int gridX = pixelPos.y / cellsize;  // Y is row
                int gridY = pixelPos.x / cellsize;  // X is column
                
                // Check bounds and toggle
                if (gridX >= 0 && gridX < grid.getRows() && 
                    gridY >= 0 && gridY < grid.getCols()) {
                    grid.toggleCell(gridX, gridY);
                }
            }
        }

        // Mouse movement (for dragging)
        if (auto* mouseEvent = event->getIf<sf::Event::MouseMoved>()) {
            // Check if left button is currently pressed
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                // Get current mouse position
                sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                
                // Convert to grid coordinates
                int gridRow = pixelPos.y / cellsize;
                int gridCol = pixelPos.x / cellsize;
                
                // Check bounds and toggle cell
                if (gridRow >= 0 && gridRow < grid.getRows() && 
                    gridCol >= 0 && gridCol < grid.getCols()) {
                    grid.toggleCell(gridRow, gridCol);
                }
            }
        }
    
    }
}

void Game::render() {
    window.clear(sf::Color::Black);  
    
    // Draw each live cell
    for (int row = 0; row < grid.getRows(); ++row) {
        for (int col = 0; col < grid.getCols(); ++col) {
            if (grid.isAlive(row, col)) {
                // Create a rectangle for this cell
                sf::RectangleShape cell(sf::Vector2f(float(cellsize - 1), float(cellsize - 1)));
                cell.setPosition({float(col * cellsize), float(row * cellsize)});
                cell.setFillColor(sf::Color::White);
                window.draw(cell);
            }
        }
    }
    
    window.display();
}

void Game::togglePause() {
    isPaused = !isPaused;
}

void Game::clearGrid() {
    grid.clear();
}

void Game::randomiseGrid() {
    grid.randomise();
}
