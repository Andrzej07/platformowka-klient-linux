#include "Errors.h"
#include <iostream>
#include <SDL2/SDL.h>


void fatalError(std::string errorString)
{
	std::cout << "Fatal error: ";
	std::cout << errorString << std::endl;
	std::cout << "\n Press any key to quit...\n";
	int tmp;
	std::cin >> tmp;

	SDL_Quit();
	exit(1);
}
