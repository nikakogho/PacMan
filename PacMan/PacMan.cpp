#include "PacMan.h"
#include "GameManager.h"

#include <iostream>

PacMan::PacMan()
	:Creature(START_X, START_Y) // spawn the player at tiles[START_X][START_Y]
{
	dotsEaten = 0;
	alive = true;
}

void PacMan::addDirectionToQueue(Direction dir)
{
	if (!directions.empty()) // we are not moving
	{
		if((int)dir == -(int)directions.front()) // if we want to move in opposite direction, clear out the direqction queue
		{
			std::queue<Direction> clear; // new direction queue
			directions = clear; // assign this new direction queue
		}
	}

	if (directions.size() < 2) directions.push(dir); // if there is room, add new direction (so that we turn as soon as we reach an intersection)
}

void PacMan::move()
{
	if (directions.empty()) return; // if we do not have a direction, do nothing

	float speed = MOVE_SPEED * GameManager::elapsed;

	switch(directions.front()) // check our current direction and move accordingly
	{
	case Direction::UP:
		Creature::move(0, -speed);
		break;
	case Direction::DOWN:
		Creature::move(0, speed);
		break;
	case Direction::RIGHT:
		Creature::move(speed, 0);
		break;
	case Direction::LEFT:
		Creature::move(-speed, 0);
		break;
	}
}

std::queue<Direction> PacMan::getDirections() const
{
	return directions;
}

void PacMan::stop()
{
	if(directions.size() > 1)
	{
		//check if we are standing on an existing tile on the grid
		if ((int)(screenX + TILE_HALF) % TILE_SIZE == 0 && (int)(screenY + TILE_HALF) % TILE_SIZE == 0)
		{
			switch (directions.front()) // check current direction
			{
			case Direction::UP:
			case Direction::DOWN:
			case Direction::RIGHT:
			case Direction::LEFT:
				directions.pop(); // if it is a legit moving direction, then pop it so we stop
				break;
			}
		}
	}
}

int PacMan::getDotsEaten() const
{
	return dotsEaten;
}

void PacMan::eatDot()
{
	dotsEaten++;
}

bool PacMan::isAlive() const
{
	return alive;
}

void PacMan::setAlive(bool really)
{
	alive = really;
}
