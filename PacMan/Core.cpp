#include "Core.h"

#include "Resources.h"
#include "AudioManager.h"

using namespace sf;

const int Core::SCREEN_WIDTH = 500;
const int Core::SCREEN_HEIGHT = 600;

RenderWindow* Core::window;
Debug Core::debug("Core");
GameManager Core::gameManager;

void Core::init()
{
	debug.logLine("Init...");
	Resources::load(); // load all sprites (png files for maze and creatures)
	AudioManager::init(); // load all audio files
	window = new sf::RenderWindow(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "PacMan"); // create window
	debug.logLine("Window has been created...");
	gameManager.init();
	debug.logLine("Game is ready!");
}

bool Core::isRunning()
{
	return window->isOpen(); // if window is still open, game is still running
}

void Core::handleEvents()
{
	Event event;
	while(window->pollEvent(event))
	{
		switch(event.type)
		{
		case Event::Closed:
			debug.logLine("Closing the window...");
			window->close();
			break;

		case Event::KeyPressed:
			gameManager.keyPressed(event.key.code); // call keyPressed on stateManager
			break;

		case Event::KeyReleased:
			gameManager.keyReleased(event.key.code); // call keyReleased on stateManager
			break;
		}
	}
	gameManager.loop(); // call the loop() of stateManager
}

void Core::draw() 
{
	window->clear(Color::Black); // clear window
	gameManager.draw(*window); // draw everything
	window->display(); // display everything
}