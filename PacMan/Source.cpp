#include "Core.h"

int main()
{
	Core::init(); // initialize the game window

	while(Core::isRunning()) // iterate while game is running
	{
		Core::handleEvents(); // handle keypress events and game loop
		Core::draw(); // draw to screen
	}

	system("pause"); // don't exit right away
}