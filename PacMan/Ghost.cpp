#include "Ghost.h"
#include "GameManager.h"

Ghost::Ghost(std::string name, int gridX, int gridY, int destinationX, int destinationY, int resourceID) // constructor
:Creature(gridX, gridY), name(name), destinationGridX(destinationX), destinationGridY(destinationY), spriteID(resourceID)
{
	scattering = true;
	outOfCage = false;
	decision = true;
	scared = false;
}

std::string Ghost::getName() const // gets the same
{
	return name;
}

void Ghost::setDestination(int x, int y) // sets a new destination
{
	destinationGridX = x;
	destinationGridY = y;
}

int Ghost::getDestX() const // get destination's x
{
	return destinationGridX;
}

int Ghost::getDestY() const // get destination's y
{
	return destinationGridY;
}

void Ghost::setDirection(Direction _dir) // setter for dir
{
	dir = _dir;
}

Direction Ghost::getDirection() const // getter for dir
{
	return dir;
}

int Ghost::getResourceID() const // getter for resourceID (we use this to load the sprites for this ghost)
{
	return spriteID;
}

void Ghost::move()
{
	float speed = MOVE_SPEED * GameManager::elapsed;

	switch(dir) // decide where to move based on direction
	{
	case Direction::UP:
		Creature::move(0, -speed); // since top left corner is (0, 0), what we see as UP technically moves us down
		break;
	case Direction::DOWN:
		Creature::move(0, speed); // since top left corner is (0, 0), what we see as DOWN technically moves us up
		break;
	case Direction::RIGHT:
		Creature::move(speed, 0);
		break;
	case Direction::LEFT:
		Creature::move(-speed, 0);
		break;
	}
}

bool Ghost::isScattering() const // is the ghost scattering right now?
{
	return scattering;
}

void Ghost::setScattering(bool really) // start (or stop) scattering!
{
	scattering = really;
}

bool Ghost::isOutOfCage() const // is the ghost out of cage? (ghost won't move if he's in cage)
{
	return outOfCage;
}

bool Ghost::shouldTakeDecision() const // is it time to make a decision about what to do?
{
	return decision;
}

void Ghost::setTakeDecision(bool really) // tell the ghost that it's time to make (or not time to make) a decision
{
	decision = really;
}

void Ghost::teleport(int x, int y, bool necessarily) // teleport the ghost somewhere on the map (or outside the map if x < 0 or y < 0)
{
	if (!necessarily && x >= 0 && y >= 0 && outOfCage) return; // ignore it if this is a normal request and we are already out of cage

	Creature::teleport(x, y);
	//if (x == PlayingState::GHOST_START_X && y == PlayingState::GHOST_START_Y) setScattering(true);
	outOfCage = true;
}

void Ghost::setScared(bool really)
{
	if (really) scared = 2000; // setup countdown
	else scared = 0; // no longer scared
}

bool Ghost::hasDestination() const
{
	return destinationGridX >= 0 && destinationGridY >= 0;
}

bool Ghost::reachedDestination() const // has the ghost reached the destination point?
{
	if (!hasDestination()) return true; // no destination, so true by default
	else return gridX == destinationGridX && gridY == destinationGridY; // check if we stand on destination point
}

void Ghost::clearDestinationIfReached()
{
	if (!hasDestination()) return; // no destination so ignore it

	if (reachedDestination()) destinationGridX = destinationGridY = -1; // clear destination
}

bool Ghost::isScared() // is the ghost still scared? (also subtract 1 from the scared countdown)
{
	if (scared) scared--;
	return scared > 0;
}