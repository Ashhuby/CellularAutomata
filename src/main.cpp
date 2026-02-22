#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    std::cout << "Game of Life starting..." << std::endl;
    std::cout << "Press any key to continue..." << std::endl;
    std::cin.get(); // Wait for user input
    
    std::cout << "Creating window..." << std::endl;
    
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Game of Life");
    
    std::cout << "Window created. Running main loop..." << std::endl;
    
    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }
        
        window.clear(sf::Color::Black);
        window.display();
    }
    
    std::cout << "Window closed. Exiting..." << std::endl;
    std::cin.get(); // Wait again before closing
    
    return 0;
}