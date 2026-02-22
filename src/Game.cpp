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
    
    }
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