#include "Game/Game.h"
#include <iostream>
#include <exception>

int main()
{
    try 
    {
        Game game;
        game.run();
        return 0;
    } 
    catch (const std::exception& e) 
    {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
