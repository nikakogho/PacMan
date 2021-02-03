#include "Maze.h"
#include "AudioManager.h"
#include <set>

int Maze::is_inside_borders_x(int x) const
{
	return x >= 0 && x < SIZE_X;
}

int Maze::is_inside_borders_y(int y) const
{
	return y >= 0 && y < SIZE_Y;
}

int Maze::coord_to_index(int x, int y) const // turn coordinates into index used by costs
{
	return y * SIZE_X + x;
}

std::pair<int, int> Maze::index_to_coords(int index) const // turn index into (x, y) coordinates
{
	int x = index % SIZE_X;
	int y = index / SIZE_X;

	return { x, y };
}

void Maze::print_costs(int from) const // print the costs to the console
{
	auto coords = index_to_coords(from);
	int x = coords.first;
	int y = coords.second;

	debug.logLine("Costs for going from point (" + to_string(x) + " " + to_string(y) + ") are:");

	std::string fullText = "\n";

	for(int mapY = SIZE_Y - 1; mapY >= 0; mapY--)
	{
		for (int mapX = 0; mapX < SIZE_X; mapX++)
		{
			int index = coord_to_index(mapX, mapY);

			int cost = costs[from][index];

			string cost_text = " " + to_string(cost);

			if (cost < 10) cost_text += "  ";
			else if (cost < 100) cost_text += " ";
			else if (cost == BIG) cost_text = " -1 ";

			fullText += cost_text;
		}
		fullText += "\n";
	}

	fullText += "\n";
	debug.logLine(fullText);
}

void Maze::calculate_distances_from(int from) // implements Dijkstra algorithm to find shortest paths
{
	if (calculated_for[from]) return; // we've already calculated costs from this point so don't do anything

	auto coords = index_to_coords(from); // get the coordinates of this point

	std::set<std::pair<int, std::pair<int, int>>> stored_costs; // Dijkstra: store all improvable (cost, coordinate) pairs to go through
	stored_costs.insert({ 0, coords }); // start with cost of 0 from current coordinate

	do
	{

		auto iter_first = stored_costs.begin(); // get the first element in this set (the cheapest path)

		int cost_here = iter_first->first; // get the cost of this point

		auto coords_here = iter_first->second; // get the coordinates of this point
		int x = coords_here.first;
		int y = coords_here.second;

		int index_here = coord_to_index(x, y); // get the index of this point

		//debug.logLine("Selected point (" + to_string(x) + " " + to_string(y) + ") with cost of " + to_string(costs[from][index_here]));

		stored_costs.erase(iter_first); // erase this point from the set

		if (cost_here >= costs[from][index_here]) continue; // we don't care if this is not a cheaper solution
		if (tileBlocksCreature(x, y)) continue; // we don't care if this tile is not pass-through

		//debug.logLine("Updated cost for point (" + to_string(x) + " " + to_string(y) + ") to " + to_string(cost_here));

		costs[from][index_here] = cost_here; // set this point's cost

		std::vector<std::pair<int, int>> sides = { { x + 1, y }, { x - 1, y }, { x, y + 1 }, { x, y - 1 } }; // get all sides

		for(auto side : sides) // for each side
		{
			int side_x = side.first; // get x
			int side_y = side.second; // get y

			if (side_x < 0 || side_y < 0 || side_x >= SIZE_X || side_y >= SIZE_Y) continue; // out of bounds
			if (tileBlocksCreature(side_x, side_y)) continue; // neighbors don't allow this tile

			int side_index = coord_to_index(side_x, side_y); // get index

			int new_cost = cost_here + 1; // get cost of going to this point from last point (each new step costs 1 so it's just cost_here + 1)

			if (new_cost >= costs[from][side_index]) continue; // not a cheaper solution, so we ignore it

			stored_costs.insert({ new_cost, side }); // it is a cheaper solution, so we add it to set
		}
	} while (!stored_costs.empty()); // keep doing it until there's nothing left in the set

	calculated_for[from] = true; // we have calculated each cost from this point

	//print_costs(from); // print the results
}

Maze::Maze()
	:debug("Maze"), tiles { { 30, 30, 30,  0, 13, 13, 13, 13, 13, 13, 13, 13,  1, 30, 30, 30,  8, 30,  9, 30, 30, 30,  0, 13, 13, 13, 13, 17, 28, 13, 13, 13, 13,  1, 30, 30 },
{ 30, 30, 30,  8, 26, 26, 27, 26, 26, 26, 26, 26,  9, 30, 30, 30,  8, 30,  9, 30, 30, 30,  8, 26, 26, 26, 27, 11, 10, 26, 26, 26, 26,  9, 30, 30 },
{ 30, 30, 30,  8, 26,  2, 14,  3, 26,  2,  3, 26,  9, 30, 30, 30,  8, 30,  9, 30, 30, 30,  8, 26,  2,  3, 26,  6,  7, 26,  2,  3, 26,  9, 30, 30 },
{ 30, 30, 30,  8, 26, 11, 30, 10, 26, 11, 10, 26,  9, 30, 30, 30,  8, 30,  9, 30, 30, 30,  8, 26, 11, 10, 26, 26, 26, 26, 11, 10, 26,  9, 30, 30 },
{ 30, 30, 30,  8, 26, 11, 30, 10, 26, 11, 10, 26,  9, 30, 30, 30,  8, 30,  9, 30, 30, 30,  8, 26, 11,  6, 14, 14,  3, 26, 11, 10, 26,  9, 30, 30 },
{ 30, 30, 30,  8, 26,  6, 15,  7, 26,  6,  7, 26,  6, 13, 13, 13,  7, 30,  6, 13, 13, 13,  7, 26,  6, 15, 15, 15,  7, 26, 11, 10, 26,  9, 30, 30 },
{ 30, 30, 30,  8, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 11, 10, 26,  9, 30, 30 },
{ 30, 30, 30,  8, 26,  2, 14,  3, 26,  2, 14, 14, 14, 14, 14, 14,  3, 30,  2, 14, 14, 14,  3, 26,  2,  3, 26,  2, 14, 14,  7, 10, 26,  9, 30, 30 },
{ 30, 30, 30,  8, 26, 11, 30, 10, 26,  6, 15, 15,  3,  2, 15, 15,  7, 30,  6, 15, 15, 15,  7, 26, 11, 10, 26,  6, 15, 15,  3, 10, 26,  9, 30, 30 },
{ 30, 30, 30,  8, 26, 11, 30, 10, 26, 26, 26, 26, 11, 10, 30, 30, 30, 30, 30, 30, 30, 30, 30, 26, 11, 10, 26, 26, 26, 26, 11, 10, 26,  9, 30, 30 },
{ 30, 30, 30,  8, 26, 11, 30, 10, 26,  2,  3, 26, 11, 10, 30, 18, 12, 12, 12, 19, 30,  2,  3, 26, 11, 10, 26,  2,  3, 26, 11, 10, 26,  9, 30, 30 },
{ 30, 30, 30,  8, 26,  6, 15,  7, 26, 11, 10, 26,  6,  7, 30,  9, 30, 30, 30,  8, 30, 11, 10, 26,  6,  7, 26, 11, 10, 26,  6,  7, 26,  9, 30, 30 },
{ 30, 30, 30,  8, 26, 26, 26, 26, 26, 11, 10, 26, 30, 30, 30,  9, 30, 30, 30,  8, 30, 11, 10, 26, 26, 26, 26, 11, 10, 26, 26, 26, 26,  9, 30, 30 },
{ 30, 30, 30, 24, 14, 14, 14,  3, 26, 11,  6, 14, 14,  3, 30, 31, 30, 30, 30,  8, 30, 11,  6, 14, 14,  3, 30, 11,  6, 14, 14,  3, 26,  9, 30, 30 },
{ 30, 30, 30, 16, 15, 15, 15,  7, 26, 11,  2, 15, 15,  7, 30, 31, 30, 30, 30,  8, 30, 11,  2, 15, 15,  7, 30, 11,  2, 15, 15,  7, 26,  9, 30, 30 },
{ 30, 30, 30,  8, 26, 26, 26, 26, 26, 11, 10, 26, 30, 30, 30,  9, 30, 30, 30,  8, 30, 11, 10, 26, 26, 26, 26, 11, 10, 26, 26, 26, 26,  9, 30, 30 },
{ 30, 30, 30,  8, 26,  2, 14,  3, 26, 11, 10, 26,  2,  3, 30,  9, 30, 30, 30,  8, 30, 11, 10, 26,  2,  3, 26, 11, 10, 26,  2,  3, 26,  9, 30, 30 },
{ 30, 30, 30,  8, 26, 11, 30, 10, 26,  6,  7, 26, 11, 10, 30, 22, 13, 13, 13, 23, 30,  6,  7, 26, 11, 10, 26,  6,  7, 26, 11, 10, 26,  9, 30, 30 },
{ 30, 30, 30,  8, 26, 11, 30, 10, 26, 26, 26, 26, 11, 10, 30, 30, 30, 30, 30, 30, 30, 30, 30, 26, 11, 10, 26, 26, 26, 26, 11, 10, 26,  9, 30, 30 },
{ 30, 30, 30,  8, 26, 11, 30, 10, 26,  2, 14, 14,  7,  6, 14, 14,  3, 30,  2, 14, 14, 14,  3, 26, 11, 10, 26,  2, 14, 14,  7, 10, 26,  9, 30, 30 },
{ 30, 30, 30,  8, 26,  6, 15,  7, 26,  6, 15, 15, 15, 15, 15, 15,  7, 30,  6, 15, 15, 15,  7, 26,  6,  7, 26,  6, 15, 15,  3, 10, 26,  9, 30, 30 },
{ 30, 30, 30,  8, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 11, 10, 26,  9, 30, 30 },
{ 30, 30, 30,  8, 26,  2, 14,  3, 26,  2,  3, 26,  2, 12, 12, 12,  3, 30,  2, 12, 12, 12,  3, 26,  2, 14, 14, 14,  3, 26, 11, 10, 26,  9, 30, 30 },
{ 30, 30, 30,  8, 26, 11, 30, 10, 26, 11, 10, 26,  9, 30, 30, 30,  8, 30,  9, 30, 30, 30,  8, 26, 11,  2, 15, 15,  7, 26, 11, 10, 26,  9, 30, 30 },
{ 30, 30, 30,  8, 26, 11, 30, 10, 26, 11, 10, 26,  9, 30, 30, 30,  8, 30,  9, 30, 30, 30,  8, 26, 11, 10, 26, 26, 26, 26, 11, 10, 26,  9, 30, 30 },
{ 30, 30, 30,  8, 26,  6, 15,  7, 26,  6,  7, 26,  9, 30, 30, 30,  8, 30,  9, 30, 30, 30,  8, 26,  6,  7, 26,  2,  3, 26,  6,  7, 26,  9, 30, 30 },
{ 30, 30, 30,  8, 26, 26, 27, 26, 26, 26, 26, 26,  9, 30, 30, 30,  8, 30,  9, 30, 30, 30,  8, 26, 26, 26, 27, 11, 10, 26, 26, 26, 26,  9, 30, 30 },
								{ 30, 30, 30,  4, 12, 12, 12, 12, 12, 12, 12, 12,  5, 30, 30, 30,  8, 30,  9, 30, 30, 30,  4, 12, 12, 12, 12, 25, 20, 12, 12, 12, 12,  5, 30, 30 }}
{
	intersections = {
	{ 4, { 1, 6, 12, 15, 21, 26, 4 } },
	{ 8, { 1, 6, 9, 12, 15, 18, 21, 26 } },
	{ 11, { 1, 6, 9, 12, 15, 18, 21, 26 } },
	{ 14, { 9, 12, 13, 15, 18 } },
	{ 15, { 14 } },
	{ 17, { 6, 9, 12, 15, 18, 21, 26 } },
	{ 20, { 9, 18 } },
	{ 23, { 1, 3, 6, 9, 12, 15, 18, 21, 26 } },
	{ 26, { 1, 3, 6, 9, 12, 15, 18, 21, 24, 26 } },
	{ 29, { 1, 3, 6, 9, 12, 15, 18, 21, 24, 26 } },
	{ 32, { 1, 12, 15, 26 } }
	};

	for (int i = 0; i < MAP_FULL_SIZE; i++) // setup each path as not calculated by default
	{
		calculated_for[i] = false; // no paths are calculated by default
		for (int j = 0; j < MAP_FULL_SIZE; j++) costs[i][j] = BIG; // by default, assume all paths are unreachable
	}

	debug.logLine("Started calculating distances");
	for (int i = 0; i < MAP_FULL_SIZE; i++) calculate_distances_from(i); // calculate all paths at start
	debug.logLine("Done calculating distances...");
}

int Maze::getTileCode(int x, int y) const
{
	return tiles[x][y];
}

int Maze::getPathCost(int x1, int y1, int x2, int y2)
{
	if (!is_inside_borders_x(x1) || !is_inside_borders_y(y1) || !is_inside_borders_x(x2) || !is_inside_borders_y(y2)) return BIG; // out of borders so ignore it

	int from = coord_to_index(x1, y1); // get the index associated with point (x1, y1)
	int to = coord_to_index(x2, y2); // get the index associated with point (x2, y2)

	if (!calculated_for[from]) calculate_distances_from(from); // if we haven't calculated costs from this point, calculate them now

	return costs[from][to]; // get the cost of going from (x1, y1) to (x2, y2)
}

bool Maze::tileBlocksCreature(int x, int y) const
{
	return tiles[x][y] != 30 && tiles[x][y] != 26 && tiles[x][y] != 27; // only (26, 27, 30) types of tiles are pass-through
}

bool Maze::isIntersection(int x, int y) const
{
	auto it = intersections.find(y); // get all intersections for given y

	if (it == intersections.end()) return false; // if there are no intersections at this y, then this is not an intersection

	for (int checkX : it->second) if (x == checkX) return true; // if one of such intersections match, return true

	return false; // no intersection found
}

void Maze::removeDot(PacMan* pacman, std::vector<Ghost*> ghosts)
{
	switch (tiles[pacman->getGridX()][pacman->getGridY()]) // consider what tile we are standing on
	{
	case 26: // a dot
		AudioManager::play(AudioManager::COIN_COLLECTING_SOUND_ID); // play the sound
		tiles[pacman->getGridX()][pacman->getGridY()] = 30; // turn dot into normal empty space
		pacman->eatDot(); // eat the dot
		break;

	case 27: // the power booster thingy
		AudioManager::play(AudioManager::FRUIT_EATEN_SOUND_ID); // play the sound
		tiles[pacman->getGridX()][pacman->getGridY()] = 30; // turn it into normal empty tile
		for (Ghost* ghost : ghosts) // for each ghost
			ghost->setScared(true); // set ghost to scared
	}

}