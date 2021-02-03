#include "Resources.h"

std::map<int, sf::Sprite*> Resources::sprites;
std::map<int, Animation> Resources::animations;

sf::Texture* Resources::creatures;
sf::Texture* Resources::maze;
Debug Resources::debug("Resources");

sf::Sprite* Resources::mazePieces[32];

#pragma region Creature IDs

const int Resources::PAC_MAN       = 0;
const int Resources::PAC_MAN_DOWN  = 1;
const int Resources::PAC_MAN_LEFT  = 2;
const int Resources::PAC_MAN_RIGHT = 3;

const int Resources::BLINKY        = 4;
const int Resources::BLINKY_DOWN   = 5;
const int Resources::BLINKY_LEFT   = 6;
const int Resources::BLINKY_RIGHT  = 7;

const int Resources::PINKY = 8;
const int Resources::PINKY_DOWN = 9;
const int Resources::PINKY_LEFT = 10;
const int Resources::PINKY_RIGHT = 11;
								   
const int Resources::INKY = 12;
const int Resources::INKY_DOWN = 13;
const int Resources::INKY_LEFT = 14;
const int Resources::INKY_RIGHT = 15;
								   
const int Resources::CLYDE = 16;
const int Resources::CLYDE_DOWN = 17;
const int Resources::CLYDE_LEFT = 18;
const int Resources::CLYDE_RIGHT = 19;

const int Resources::SCARED_GHOST = 20;
const int Resources::DEAD_PAC_MAN = 21;

#pragma endregion

void Resources::load()
{
	maze = new sf::Texture(); // create texture for maze
	maze->loadFromFile("Resources/Textures/maze.png"); // load the maze

	for(int i = 0; i < 8; i++) for(int j = 0; j < 4; j++) // go through each piece of the maze
	{
		int index = i * 4 + j; // get the index of the current piece

		mazePieces[index] = new sf::Sprite(*maze, sf::IntRect(i * 8, j * 8, 8, 8)); // take the chunk of the maze associated with (i, j)
		mazePieces[index]->setScale(2, 2); // set the chunk's size to (2, 2)
	}

	creatures = new sf::Texture(); // create texture for creatures
	creatures->loadFromFile("Resources/Textures/creatures.png"); // load creatures

	#pragma region Load Creature Sprites

	loadSprite(Resources::PAC_MAN,      0, 0, 3);
	loadSprite(Resources::PAC_MAN_DOWN, 45, 0, 3);
	loadSprite(Resources::PAC_MAN_LEFT, 90, 0, 3);
	loadSprite(Resources::PAC_MAN_RIGHT, 135, 0, 3);

	loadSprite(Resources::BLINKY, 0, 15, 2);
	loadSprite(Resources::BLINKY_DOWN, 30, 15, 2);
	loadSprite(Resources::BLINKY_LEFT, 60, 15, 2);
	loadSprite(Resources::BLINKY_RIGHT, 90, 15, 2);

	loadSprite(Resources::PINKY, 0, 30, 2);
	loadSprite(Resources::PINKY_DOWN, 30, 30, 2);
	loadSprite(Resources::PINKY_LEFT, 60, 30, 2);
	loadSprite(Resources::PINKY_RIGHT, 90, 30, 2);

	loadSprite(Resources::INKY, 0, 45, 2);
	loadSprite(Resources::INKY_DOWN, 30, 45, 2);
	loadSprite(Resources::INKY_LEFT, 60, 45, 2);
	loadSprite(Resources::INKY_RIGHT, 90, 45, 2);

	loadSprite(Resources::CLYDE, 0, 60, 2);
	loadSprite(Resources::CLYDE_DOWN, 30, 60, 2);
	loadSprite(Resources::CLYDE_LEFT, 60, 60, 2);
	loadSprite(Resources::CLYDE_RIGHT, 90, 60, 2);

	loadSprite(Resources::SCARED_GHOST, 120, 15, 2);
	loadSprite(Resources::DEAD_PAC_MAN, 0, 75, 12);

	#pragma endregion

	debug.logLine("Finished loading sprites..."); // done
}

sf::Sprite* Resources::get(int value, bool animated, Direction facing)
{
	if(value != SCARED_GHOST) // scared ghosts don't look different based on direction, so we only care about direction if it's not a scared ghost
	{
		switch(facing) // consider each direction, up is assumed by default
		{
		case Direction::DOWN:
			value++; // ID of DOWN is always 1 after ID of up
			break;
		case Direction::LEFT:
			value += 2; // ID of LEFT is always 2 after ID of up
			break;
		case Direction::RIGHT:
			value += 3; // ID of RIGHT is always 3 after ID of up
			break;
		}
	}

	if(animated)
	{
		animations.at(value).changeFrame(); // go to next frame if time has come
		sprites.at(value)->setTextureRect(animations.at(value).getBounds()); // frame may have changed, so make sure to update sprite accordingly
	}

	return sprites.at(value); // return the sprite
}

void Resources::loadSprite(int value, int rect1, int rect2, int frames)
{
	sf::IntRect* rect = new sf::IntRect(rect1, rect2, LOADED_SPRITE_WIDTH, LOADED_SPRITE_HEIGHT); // calculate rect bounds
	sf::Sprite* sprite = new sf::Sprite(*creatures, *rect); // take the specified chunk from creatures

	sprite->setScale(2, 2);
	sprite->setOrigin(7.5f, 7.5f);
	sprites.insert({ value, sprite }); // add sprite to sprites
	animations.insert(std::pair<int, Animation>( value, Animation(rect, frames) )); // add this animation to animations
}