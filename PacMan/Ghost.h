#pragma once

#include "Creature.h"
#include "Directions.h"
#include <string>

class Ghost : public Creature
{
	int destinationGridX; // grid's x coordinate where ghost should go
	int destinationGridY; // grid's x coordinate where ghost should go
	Direction dir; // moving direction

	bool scattering; // is ghost in scattering phase?
	bool outOfCage; // is ghost out of the cage?

	bool decision; // is it time to make a decision?
	int scared; // how long before we're no longer scared?

	const float MOVE_SPEED = 0.1f;

	int spriteID; // id of the sprite resource associated with this ghost

	std::string name; // name of the ghost (Blinky Pinky Inky Clyde)

public:
	Ghost(std::string name, int gridX, int gridY, int destinationX, int destinationY, int resourceID); // constructor

	std::string getName() const; // get the name of the ghost

	void setDestination(int x, int y); // set the destination on grid
	int getDestX() const; // get destinationGridX
	int getDestY() const; // get destinationGridY

	void setDirection(Direction _dir); // set moving direction
	Direction getDirection() const; // get moving direction

	int getResourceID() const; // get the resourceID

	void move(); // move

	bool isScattering() const; // public fields for scattering
	void setScattering(bool really);

	bool isOutOfCage() const; // are we out of cage?

	bool shouldTakeDecision() const; // public fields for decision
	void setTakeDecision(bool really);

	void teleport(int x, int y, bool necessarily = false); // spawn on these coordinates on grid

	bool isScared(); // is still scared? (if so, subtract 1)
	void setScared(bool really); // reset scared

	bool hasDestination() const;
	bool reachedDestination() const; // is position same as destination?
	void clearDestinationIfReached(); // if we've reached a destination, time to remove it
};