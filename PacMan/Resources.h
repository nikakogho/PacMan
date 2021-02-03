#pragma once

#include "SFML/Graphics.hpp"
#include "Debug.h"
#include "Animation.h"
#include "Directions.h"

#include <map>

class Resources // all textures and sounds
{
	static sf::Texture* creatures; // texture that hold all creatures (pacman, ghosts, and their scared and dead forms)
	static sf::Texture* maze; // texture that holds the entire maze
	static Debug debug; // Debug object to write things to console

	static std::map<int, sf::Sprite*> sprites; // stores all sprites associated to values (for pacman and ghosts looking at each side)
	static std::map<int, Animation> animations; // stores all animations

	static void loadSprite(int value, int rect1, int rect2, int frames); // load a sprite into memory

	static const int LOADED_SPRITE_WIDTH = 15; // width of each loaded sprite
	static const int LOADED_SPRITE_HEIGHT = 15; // height of each loaded sprite

	#pragma region IDs of each side (Down Left Right) of each creature

	static const int PAC_MAN_DOWN;
	static const int PAC_MAN_LEFT;
	static const int PAC_MAN_RIGHT;

	static const int BLINKY_DOWN;
	static const int BLINKY_LEFT;
	static const int BLINKY_RIGHT;

	static const int PINKY_DOWN;
	static const int PINKY_LEFT;
	static const int PINKY_RIGHT;

	static const int INKY_DOWN;
	static const int INKY_LEFT;
	static const int INKY_RIGHT;

	static const int CLYDE_DOWN;
	static const int CLYDE_LEFT;
	static const int CLYDE_RIGHT;

	#pragma endregion

public:
	static void load(); // load everything
	static sf::Sprite* get(int value, bool animated, Direction facing); // get a specific sprite (and switch to next frame if needed)

	static sf::Sprite* mazePieces[]; // stores each piece of the maze

	static const int PAC_MAN; // player

	static const int BLINKY; // ghost 1
	static const int PINKY; // ghost 2
	static const int INKY; // ghost 3
	static const int CLYDE; // ghost 4

	static const int SCARED_GHOST; // ghost when scared
	static const int DEAD_PAC_MAN; // player when dead
};