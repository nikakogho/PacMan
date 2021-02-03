#pragma once

#include "SFML/Graphics.hpp"
#include "Debug.h"
#include "GameManager.h"

class Core // for core functionality like setting up window and running game loop
{
	static sf::RenderWindow* window; // actual window
	static Debug debug; // debug for console
	static GameManager gameManager; // the game manager

public:
	const static int SCREEN_WIDTH; // screen width
	const static int SCREEN_HEIGHT; // screen height

	static void init(); // initialize the program
	static bool isRunning(); // is it still running?
	static void handleEvents(); // handle keypress and window close
	static void draw(); // draw to window
};