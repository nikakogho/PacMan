#pragma once

#include "Creature.h"

#include "Directions.h"
#include <queue>

class PacMan : public Creature
{
	std::queue<Direction> directions; // store directions (no more than 2)
	int dotsEaten; // count the number of dots we ate
	bool alive; // still alive?

	const static int START_X = 13; // starting x coordinate on grid
	const static int START_Y = 26; // starting y coordinate on grid

	const float MOVE_SPEED = 0.2f; // what speed does PacMan move at? (pixels per millisecond)

public:
	PacMan(); // constructor

	void addDirectionToQueue(Direction dir); // add new direction
	void move(); // move
	std::queue<Direction> getDirections() const; // get all directions
	void stop(); // stop moving

	int getDotsEaten() const; // return dotsEaten
	void eatDot(); // eat a dot

	bool isAlive() const; // getter for alive
	void setAlive(bool really); // setter for alive
};