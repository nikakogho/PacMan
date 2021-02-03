#pragma once

class Creature
{
protected:
	float screenX; // x coordinate on screen
	float screenY; // y coordinate on screen
	int gridX; // x coordinate on grid
	int gridY; // y coordinate on grid

	const static int TILE_SIZE = 16; // size of tile in pixels
	const static int TILE_HALF = TILE_SIZE / 2; // size of half of the tile

public:
	Creature(int gridX, int gridY); // constructor

	float getScreenPosX() const; // get screenX
	float getScreenPosY() const; // get screenY
	int getGridX() const; // get gridX
	int getGridY() const; // get gridY

	void move(float x, float y); // move by (x, y) pixels on screen
	void teleport(int x, int y, bool necessarily = false); // teleport to (x, y) on grid
};