#pragma once

#include "Ghost.h"
#include "PacMan.h"
#include "Debug.h"

#include <map>

class Maze
{
public:
	static const int SIZE_X = 28; // width of the maze (in tiles)
	static const int SIZE_Y = 36; // height of the maze (in tiles)

private:
	static const int MAP_FULL_SIZE = SIZE_X * SIZE_Y;

	static const int BIG = 1e9; // big number used as default cost for cost of each path

	int is_inside_borders_x(int x) const; // is this x inside borders?
	int is_inside_borders_y(int y) const; // is this y inside borders?

	int coord_to_index(int x, int y) const; // turn (x, y) coordinates into index used by the costs matrix
	std::pair<int, int> index_to_coords(int index) const; // turn index into (x, y) coordinates

	int costs[MAP_FULL_SIZE][MAP_FULL_SIZE]; // cost to going from point A to point B
	bool calculated_for[MAP_FULL_SIZE]; // have we already calculated all costs going from point A?

	void print_costs(int from) const; // print the costs

	void calculate_distances_from(int path); // calculate distances coming from this path

	Debug debug; // debugger for maze stuff

public:
	std::map<int, std::vector<int>> intersections; // all intersection points

private:
	int tiles[SIZE_X][SIZE_Y]; // all tiles

public:
	Maze(); // constructor

	int getTileCode(int x, int y) const; // gets the value of tiles[x][y]

	int getPathCost(int x1, int y1, int x2, int y2); // get the cost of path from (x1, y1) to (x2, y2)

	bool tileBlocksCreature(int x, int y) const; // does tiles[x][y] block creatures or is it pass-through?
	bool isIntersection(int x, int y) const; // is (x, y) one of intersection points?

	void removeDot(PacMan* pacman, std::vector<Ghost*> ghosts); // this tile previously held a dot (the coin), turn it to normal
};