#include "Creature.h"

#include <cmath>

Creature::Creature(int gridX, int gridY)
:gridX(gridX), gridY(gridY)
{
	screenX = gridX * TILE_SIZE + TILE_HALF + TILE_HALF;
	screenY = gridY * TILE_SIZE + TILE_HALF;
}

float Creature::getScreenPosX() const
{
	return screenX;
}

float Creature::getScreenPosY() const
{
	return screenY;
}

int Creature::getGridX() const
{
	return gridX;
}

int Creature::getGridY() const
{
	return gridY;
}

void Creature::move(float x, float y)
{
	screenX += x; // move by x pixels
	screenY += y; // move by y pixels

	// check if we entered a new tile on grid
	if((int)(screenX + TILE_HALF) % TILE_SIZE == 0 && (int)(screenY + TILE_HALF) % TILE_SIZE == 0)
	{
		// move to the new tile on grid
		gridX = (int)round((screenX - TILE_HALF) / TILE_SIZE);
		gridY = (int)round((screenY - TILE_HALF) / TILE_SIZE);
	}
}

void Creature::teleport(int x, int y, bool necessarily) // teleport to (x, y) on grid
{
	gridX = x;
	gridY = y;
	screenX = x * TILE_SIZE + TILE_HALF;
	screenY = y * TILE_SIZE + TILE_HALF;
}
