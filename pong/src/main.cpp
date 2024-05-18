#include <iostream>
#include "GameEngine.h"
// pong game
int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Pong", sf::Style::Default); // creating the window that will be the target for all 2D drawing 

	GameEngine gameEngine(window); // initialise the GameEngie class passing in the window object create above by refernce meaning we are passing in the memory location of the window not the value of the window itself 
	gameEngine.run(); /// call the run method of the GameEngine class that contains the main game loop 
}