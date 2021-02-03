#pragma once

#include "Debug.h"
#include "Maze.h"
#include "Creature.h"
#include "PacMan.h"
#include "Ghost.h"

#include "TextDisplay.h"

class GameManager
{
	Debug debug; // Debug object for writing things on console
	Maze* maze; // the maze
	TextDisplay textDisplay; // text display

	PacMan* pacman; // the player

	Ghost *blinky, *pinky, *inky, *clyde; // ghosts
	std::vector<Ghost*> ghosts; // stores all ghosts for simplyfying actions that happen to all of them the same way

	sf::Clock timer;

public:
	const static int TILE_SIZE = 16; // size of tile in pixels
	const static int TILE_HALF = TILE_SIZE / 2; // size of half of the tile

	const static int GHOST_START_X = 13; // starting tile x of ghosts
	const static int GHOST_START_Y = 14; // starting tile y of ghosts

	static float elapsed; // elapsed time since last call to loop

private:
	const static int PINKY_UNLOCK_SCORE = 5; // score required to unleash Pinky
	const static int INKY_UNLOCK_SCORE = 50; // score required to unleash Inky
	const static int CLYDE_UNLOCK_SCORE = 100; // score required to unleash Clyde

	const static int WINNING_SCORE = 240; // score required to win the game

	const static int MAX_WAIT_TIME = 8000; // once waitTime reaches this, time to reset the game

	float waitTime; // waiting timer before we start a new game once the current game is over

	bool pacmanCanMove() const; // can PacMan move?
	void handleGhostMovement(Ghost* ghost); // make the ghosts move in whatever way they are supposed to
	bool ghostCanMove(Ghost* ghost) const; // can this ghost move?
	void teleportThroughTunnels(Creature* creature); // if the creature is in a tunnel, teleport it to the other end
	void handleGhostCollision(Ghost* ghost); // check if ghost is in same spot as player and do something about it

	sf::Sprite getGhostSprite(Ghost* ghost) const; // give the ghost a different sprite (for moving animations or for being scared)

public:
	GameManager(); // constructor

	void init(); // initialize the game
	void loop(); // game loop where all the logic takes place
	void draw(sf::RenderWindow& window) const; // draw things to window
	void keyPressed(int code); // key was pressed, do something about it
	void keyReleased(int code); // key was released, do something about it
};