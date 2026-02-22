#include <SFML/Graphics.hpp>
#include <iostream>
#include "Game.h"

int main()
{
    std::cout << "=== Game of Life starting ===" << std::endl;

    Game game(60,80, 10);
    game.run();

    return 0;
}