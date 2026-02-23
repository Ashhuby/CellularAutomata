#include "Game.h"
#include <string>
#include <sstream>   
#include <algorithm> 

Game::Game(int pGridRows, int pGridCols, int pCellSize) 
	: grid(pGridRows, pGridCols),
    cellsize(pCellSize),
    windowWidth(pGridCols* pCellSize),
    windowHeight(pGridRows* pCellSize),
    updateInterval(0.1f),
    timeSinceLastUpdate(0.0f),
    speedMultiplier(1.0f), 
    isRunning(true),
    isPaused(true), 
    viewScale(1.0f),  
    lastMousePos({0, 0}),
    uiBarHeight(40),
    currentPalette(0)
{
    window.create(sf::VideoMode({ (unsigned int)windowWidth, (unsigned int)windowHeight }), "Game of Life");
    
    view.setViewport(sf::FloatRect(
        {0.f, static_cast<float>(uiBarHeight) / static_cast<float>(windowHeight)},
        {1.f, 1.f - static_cast<float>(uiBarHeight) / static_cast<float>(windowHeight)}
    ));
    view.setSize({static_cast<float>(windowWidth), static_cast<float>(windowHeight - uiBarHeight)});
    view.setCenter({static_cast<float>(windowWidth) / 2.f, static_cast<float>(windowHeight - uiBarHeight) / 2.f});

    if (!font.openFromFile("BarFont.ttf")) {
        // Font loading failed, but continue
    }
   
    speedText = std::make_unique<sf::Text>(font);
    speedText->setCharacterSize(24);
    speedText->setFillColor(sf::Color::White);
    speedText->setPosition({10, 20});
    speedText->setString("Speed: 1.0x");

    zoomText = std::make_unique<sf::Text>(font);
    zoomText->setCharacterSize(24);
    zoomText->setFillColor(sf::Color::White);
    zoomText->setPosition({210, 20});
    zoomText->setString("Zoom: 100%");

    populationText = std::make_unique<sf::Text>(font);
    populationText->setCharacterSize(24);
    populationText->setFillColor(sf::Color::White);
    populationText->setPosition({420, 20});
    populationText->setString("Cells: 0");

    pauseText = std::make_unique<sf::Text>(font);
    pauseText->setCharacterSize(24);
    pauseText->setFillColor(sf::Color::Yellow);
    pauseText->setPosition({630, 20});
    pauseText->setString("PAUSED");

    palette = {
        sf::Color::White,
        sf::Color::Green,
        sf::Color::Cyan,
        sf::Color::Magenta,
        sf::Color(255, 200, 100)
    };
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

void Game::handleInput() {
    while (auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }

        if (auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
            if (keyEvent->scancode == sf::Keyboard::Scancode::Space) {
                togglePause();
            }
            else if (keyEvent->scancode == sf::Keyboard::Scancode::C) {
                clearGrid();
            }
            else if (keyEvent->scancode == sf::Keyboard::Scancode::R) {
                randomiseGrid();
            }
            else if (keyEvent->scancode == sf::Keyboard::Scancode::Equal) {
                speedMultiplier = std::min(speedMultiplier + 0.1f, 10.0f);
                updateInterval = 0.1f / speedMultiplier;
            }
            else if (keyEvent->scancode == sf::Keyboard::Scancode::Hyphen) {
                speedMultiplier = std::max(speedMultiplier - 0.1f, 0.1f);
                updateInterval = 0.1f / speedMultiplier;
            }
            else if (keyEvent->scancode == sf::Keyboard::Scancode::P) {
                currentPalette = (currentPalette + 1) % palette.size();
            }
        }

        auto pixelToGrid = [&](sf::Vector2i pixelPos, int& row, int& col) -> bool {
            if (pixelPos.y < uiBarHeight) return false;
            // No adjustment needed - the viewport handles the offset
            sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos, view);
            row = static_cast<int>(worldPos.y) / cellsize;
            col = static_cast<int>(worldPos.x) / cellsize;
            return row >= 0 && row < grid.getRows() && col >= 0 && col < grid.getCols();
        };

        if (auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseEvent->button == sf::Mouse::Button::Left) {
                sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                int row, col;
                if (pixelToGrid(pixelPos, row, col)) {
                    grid.toggleCell(row, col);
                }
            }
        }

        if (auto* mouseEvent = event->getIf<sf::Event::MouseMoved>()) {
            sf::Vector2i pixelPos = sf::Mouse::getPosition(window);

            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Middle)) {
                sf::Vector2f delta(static_cast<float>(pixelPos.x - lastMousePos.x),
                                   static_cast<float>(pixelPos.y - lastMousePos.y));
                view.move(sf::Vector2f(-delta.x, -delta.y));
                window.setView(view);
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                int row, col;
                if (pixelToGrid(pixelPos, row, col)) {
                    grid.toggleCell(row, col);
                }
            }

            lastMousePos = pixelPos;
        }

        if (auto* wheelEvent = event->getIf<sf::Event::MouseWheelScrolled>()) {
            if (wheelEvent->wheel == sf::Mouse::Wheel::Vertical) {
                sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                sf::Vector2i adjustedPixel = { pixelPos.x, pixelPos.y - uiBarHeight };

                // Map to world BEFORE zoom, explicitly against game view
                sf::Vector2f beforeZoom = window.mapPixelToCoords(adjustedPixel, view);

                float zoomFactor = (wheelEvent->delta > 0) ? 0.9f : 1.1f;
                viewScale *= zoomFactor;
                viewScale = std::clamp(viewScale, 0.1f, 10.0f);

                // Zoom incrementally, do NOT reset to default view
                view.zoom(zoomFactor);

                sf::Vector2f afterZoom = window.mapPixelToCoords(adjustedPixel, view);
                view.move(beforeZoom - afterZoom);
                window.setView(view);
            }
        }
    }
}

void Game::render() {
    window.clear(sf::Color::Black);  
    
    // Set the view for the grid
    window.setView(view);
    
    // Draw each live cell - now using the transformed view!
    for (int row = 0; row < grid.getRows(); ++row) {
        for (int col = 0; col < grid.getCols(); ++col) {
            if (grid.isAlive(row, col)) {
                sf::RectangleShape cell(sf::Vector2f(static_cast<float>(cellsize - 1), 
                                                      static_cast<float>(cellsize - 1)));
                cell.setPosition({static_cast<float>(col * cellsize), 
                                  static_cast<float>(row * cellsize)});
                cell.setFillColor(palette[currentPalette]);
                window.draw(cell);
            }
        }
    }
    
    // Switch to UI view (fixed on screen)
    sf::View uiView(sf::FloatRect({0, 0}, {static_cast<float>(windowWidth), 
                                            static_cast<float>(windowHeight)}));
    window.setView(uiView);
    
    // Draw UI bar
    sf::RectangleShape uiBar(sf::Vector2f(static_cast<float>(windowWidth), 
                                           static_cast<float>(uiBarHeight)));
    uiBar.setFillColor(sf::Color(50, 50, 50));
    window.draw(uiBar);
    
    // Update and draw text
    std::stringstream speedStream;
    speedStream << std::fixed << std::setprecision(1) << speedMultiplier;
    speedText->setString("Speed: " + speedStream.str() + "x");
    zoomText->setString("Zoom: " + std::to_string(static_cast<int>(viewScale * 100)) + "%");
    populationText->setString("Cells: " + std::to_string(grid.countLiveCells()));
    pauseText->setString(isPaused ? "PAUSED" : "RUNNING");
    
    window.draw(*speedText);
    window.draw(*zoomText);
    window.draw(*populationText);
    window.draw(*pauseText);
    
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