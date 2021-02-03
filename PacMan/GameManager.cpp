#include "GameManager.h"

#include "SFML/Graphics.hpp"

#include "Resources.h"
#include "AudioManager.h"

#include "PacMan.h"
#include "Ghost.h"
#include "Directions.h"

#include "TextDisplay.h"

#include <cmath>
#include <random>
#include <cstdlib>

using namespace sf;

float GameManager::elapsed = 0;

GameManager::GameManager()
	:debug("Playing State") // create the debug object
{
}

void GameManager::init()
{
	maze = new Maze(); // create maze

	pacman = new PacMan(); // create pacman

	// create ghosts
	blinky = new Ghost("Blinky", 13, 14, 3, 4, Resources::BLINKY);
	pinky = new Ghost("Pinky", 13, 17, 23, 4, Resources::PINKY);
	inky = new Ghost("Inky", 11, 17, 26, 32, Resources::INKY);
	clyde = new Ghost("Clyde", 15, 17, 1, 32, Resources::CLYDE);

	ghosts = { blinky, pinky, inky, clyde }; // put ghosts in array

	blinky->teleport(GHOST_START_X, GHOST_START_Y); // spawn blinky in game

	AudioManager::play(AudioManager::OPENING_MUSIC_ID); // play the opening music

	srand(time(NULL)); // seed the random number generator

	waitTime = 0; // reset the gameover timer

	debug.logLine("Init done...");
}

void GameManager::loop() // game logic
{
	elapsed = (float)timer.getElapsedTime().asMicroseconds() / 1e3; // get elapsed time (in milliseconds)
	timer.restart(); // reset the timer

	if (pacmanCanMove() && pacman->isAlive()) pacman->move(); // if pacman can move, move it
	else pacman->stop(); // if pacman can't move, stop it

	if (maze->isIntersection(pacman->getGridX(), pacman->getGridY())) // if pacman met an intersection point
		pacman->stop(); // stop it

	maze->removeDot(pacman, ghosts); // check if pacman just ate a dot, and if he did, remove the dot from screen

	if(!pacman->getDirections().empty()) // pacman is moving, so we may need to change ghost directions
	{
		if (!blinky->isScattering()) // blinky chases directly
			blinky->setDestination(pacman->getGridX(), pacman->getGridY());

		if(!pinky->isScattering()) // pinky tries to ambush the player
		{
			int destX = pacman->getGridX(); // point to chase at
			int destY = pacman->getGridY();

			bool wantToSet = true; // if pacman is not going in a normal direction, ignore it

			switch(pacman->getDirections().front()) // considers direction to chase 4 tiles in front of where the player is heading
			{
			case Direction::UP:
				destY -= 4;
				break;
			case Direction::DOWN:
				destY += 4;
				break;
			case Direction::LEFT:
				destX -= 4;
				break;
			case Direction::RIGHT:
				destX += 4;
				break;
			default:
				wantToSet = false; // pacman is not going in a normal direction, ignore it
				break;
			}

			if (wantToSet) // pacman is going in a normal direction, chase it!
				pinky->setDestination(destX, destY);
		}

		if (!inky->isScattering()) // inky "cooperates" with blinky
		{
			int destX = (pacman->getGridX() + blinky->getGridX()) / 2; // make it so blinky and inky trap pacman in the middle
			int destY = (pacman->getGridY() + blinky->getGridY()) / 2;

			inky->setDestination(destX, destY);
		}

		if(!clyde->isScattering()) // chase if close, otherwise go back to patrol point
		{
			int distanceFromPacman = sqrt(pow((clyde->getGridX() - (pacman->getGridX())), 2) + pow((clyde->getGridY() - (pacman->getGridY())), 2));

			if (distanceFromPacman < 9) // we are close so let's chase pacman
				clyde->setDestination(pacman->getGridX(), pacman->getGridY());
			else if (clyde->reachedDestination()) 
			{
				if (clyde->getDestX() == 1 && clyde->getDestY() == 32)
					clyde->setDestination(26, 17);
				else clyde->setDestination(1, 32); // otherwise, go to corner
			}
		}
	}

	for (Ghost* ghost : ghosts) handleGhostMovement(ghost); // handle movement of each ghost

	//check if new ghosts were just unlocked
	if (pacman->getDotsEaten() == PINKY_UNLOCK_SCORE)
		pinky->teleport(GHOST_START_X, GHOST_START_Y);
	if (pacman->getDotsEaten() == INKY_UNLOCK_SCORE)
		inky->teleport(GHOST_START_X, GHOST_START_Y);
	if (pacman->getDotsEaten() == CLYDE_UNLOCK_SCORE)
		clyde->teleport(GHOST_START_X, GHOST_START_Y);

	// if somebody walked into a tunnel, teleport them to the other end
	teleportThroughTunnels(pacman);
	for (Ghost* ghost : ghosts) teleportThroughTunnels(ghost);

	// check if any ghost hit the pacman
	for (Ghost* ghost : ghosts) handleGhostCollision(ghost);

	if(pacman->getDotsEaten() >= WINNING_SCORE) // we won
	{
		for (Ghost* ghost : ghosts) ghost->teleport(-2, -2); // teleport ghosts out of map
		waitTime += elapsed; // add another frame to waiting time (we wait so that we don't restart game right away)
	}

	if (!pacman->isAlive()) waitTime += elapsed; // player died so game is over

	if(waitTime >= MAX_WAIT_TIME) // done waiting
	{
		if (pacman->isAlive()) init(); // reset the game
		else
		{ // reset the game but for dead pacman
			for (Ghost* ghost : ghosts)
				if (ghost->isOutOfCage()) ghost->teleport(GHOST_START_X, GHOST_START_Y, true);
			pacman->teleport(13, 26);
			pacman->setAlive(true);
		}
		waitTime = 0; // reset the wait timer
	}
}

void GameManager::draw(RenderWindow& window) const // draws everything
{
	Sprite pacmanSprite, blinkySprite, pinkySprite, inkySprite, clydeSprite; // gets all the sprites

	if (!pacman->isAlive()) // pacman is dead
		pacmanSprite = *Resources::get(Resources::DEAD_PAC_MAN, true, Direction::NONE); // draw dead pacman
	else if (pacman->getDirections().empty()) // pacman is alive but not moving
		pacmanSprite = *Resources::get(Resources::PAC_MAN, false, Direction::NONE); // draw standing pacman
	else // pacman is alive and moving
		pacmanSprite = *Resources::get(Resources::PAC_MAN, true, pacman->getDirections().front()); // draw moving pacman

	// get ghost sprites
	blinkySprite = getGhostSprite(blinky);
	pinkySprite = getGhostSprite(pinky);
	inkySprite = getGhostSprite(inky);
	clydeSprite = getGhostSprite(clyde);

	// put all sprites in correct positions
	pacmanSprite.setPosition(pacman->getScreenPosX(), pacman->getScreenPosY());
	blinkySprite.setPosition(blinky->getScreenPosX(), blinky->getScreenPosY());
	pinkySprite.setPosition(pinky->getScreenPosX(), pinky->getScreenPosY());
	inkySprite.setPosition(inky->getScreenPosX(), inky->getScreenPosY());
	clydeSprite.setPosition(clyde->getScreenPosX(), clyde->getScreenPosY());

	// go through each point of the maze
	for (int x = 0; x < Maze::SIZE_X; x++)
	{
		for (int y = 0; y < Maze::SIZE_Y; y++)
		{
			Sprite *piece = Resources::mazePieces[maze->getTileCode(x, y)]; // get the maze piece
			piece->setPosition(x * TILE_SIZE, y * TILE_SIZE); // put maze tile here
			window.draw(*piece); // draw the tile
		}
	}

	// draw pacman and ghosts
	window.draw(pacmanSprite);
	window.draw(blinkySprite);
	window.draw(pinkySprite);
	window.draw(inkySprite);
	window.draw(clydeSprite);

	textDisplay.draw(window); // draw texts
}

void GameManager::keyPressed(int code) // handle keypress events
{
	switch(code)
	{
	case Keyboard::Up:
	case Keyboard::W:
		pacman->addDirectionToQueue(Direction::UP);
		break;
	case Keyboard::Down:
	case Keyboard::S:
		pacman->addDirectionToQueue(Direction::DOWN);
		break;
	case Keyboard::Left:
	case Keyboard::A:
		pacman->addDirectionToQueue(Direction::LEFT);
		break;
	case Keyboard::Right:
	case Keyboard::D:
		pacman->addDirectionToQueue(Direction::RIGHT);
		break;
	}
}

void GameManager::keyReleased(int code)
{
	// nothing for now
}

bool GameManager::pacmanCanMove() const // can pacman move?
{
	if(!pacman->getDirections().empty()) // has a direction
	{
		int x = pacman->getGridX(); // x of where we are supposed to go
		int y = pacman->getGridY(); // y of where we are supposed to go

		switch(pacman->getDirections().front()) // check where pacman is headed right now to adjust the move location
		{
		case Direction::UP:
			y--;
			break;
		case Direction::DOWN:
			y++;
			break;
		case Direction::LEFT:
			x--;
			break;
		case Direction::RIGHT:
			x++;
			break;
		default:
			return true;
		}

		return !maze->tileBlocksCreature(x, y); // check if this new location is pass-through
	}

	return true; // no direction selected, so we can move
}

void GameManager::handleGhostMovement(Ghost* ghost)
{
	if(ghost->isScattering()) // ghost is in scattering mode
	{
		if (ghost->reachedDestination()) // ghost reached its destination
			ghost->setScattering(false); // stop scattering
	}

	ghost->clearDestinationIfReached(); // if the destination was reached, clear it

	if(maze->isIntersection(ghost->getGridX(), ghost->getGridY())) // if it's an intersection point, time to turn
	{
		if(ghost->shouldTakeDecision()) // should we turn?
		{
			// ghost destination coordinates
			int destX = ghost->getDestX();
			int destY = ghost->getDestY();

			// ghost coordinates
			int ghostX = ghost->getGridX();
			int ghostY = ghost->getGridY();

			// get the cost for each option
			int costRight = maze->getPathCost(destX, destY, ghostX + 1, ghostY);
			int costLeft = maze->getPathCost(destX, destY, ghostX - 1, ghostY);
			int costUp = maze->getPathCost(destX, destY, ghostX, ghostY - 1);
			int costDown = maze->getPathCost(destX, destY, ghostX, ghostY + 1);

			// sort the costs of possible directions
			int arr[] = { costRight, costLeft, costUp, costDown };
			sort(arr, arr + 4);

			// choose direction based on costs
			Direction dir = Direction::RIGHT;
			if (arr[0] == costLeft) dir = Direction::LEFT;
			else if (arr[0] == costUp) dir = Direction::UP;
			else if (arr[0] == costDown) dir = Direction::DOWN;

			ghost->setDirection(dir);
			ghost->setTakeDecision(false);
		}
	}
	else ghost->setTakeDecision(true); // we should turn

	if (ghostCanMove(ghost) && ghost->isOutOfCage()) // if ghost can move and is out of cage
		ghost->move(); // then move
	else ghost->setTakeDecision(true); // otherwise, time to make a decision
}

bool GameManager::ghostCanMove(Ghost* ghost) const // can the ghost move?
{
	// determine new position after moving
	int newX = ghost->getGridX();
	int newY = ghost->getGridY();

	switch(ghost->getDirection())
	{
	case Direction::UP:
		newY--;
		break;
	case Direction::DOWN:
		newY++;
		break;
	case Direction::LEFT:
		newX--;
		break;
	case Direction::RIGHT:
		newX++;
		break;
	default: return false;
	}

	return !maze->tileBlocksCreature(newX, newY); // is this new position pass-through?
}

void GameManager::teleportThroughTunnels(Creature* creature) // if a creature stands in a tunnel, teleport it to the other end
{
	if (creature->getGridX() == 0 && creature->getGridY() == 17)
		creature->teleport(26, 17);
	else if (creature->getGridX() == 27 && creature->getGridY() == 17)
		creature->teleport(1, 17);
}

void GameManager::handleGhostCollision(Ghost* ghost)
{
	if (pacman->getGridX() == ghost->getGridX() && pacman->getGridY() == ghost->getGridY()) // pacman collided with ghost
	{
		if (ghost->isScared()) // ghost is scared so it gets eaten
		{
			debug.logLine("Scared " + ghost->getName() + " hit pacman and got eaten!");
			AudioManager::play(AudioManager::GHOST_EATEN_SOUND_ID);
			ghost->teleport(GHOST_START_X, GHOST_START_Y, true);
			ghost->setScared(false);
		}
		else // ghost is not scared, so player gets eaten
		{
			debug.logLine("Pacman got eaten by " + ghost->getName() + "!");
			AudioManager::play(AudioManager::DEATH_SOUND_ID);
			pacman->setAlive(false);
			blinky->teleport(-2, -2);
			pinky->teleport(-2, -2);
			inky->teleport(-2, -2);
			clyde->teleport(-2, -2);
		}
	}
}

Sprite GameManager::getGhostSprite(Ghost * ghost) const // gets appropriate sprite for this ghost
{
	if (ghost->isScared()) return *Resources::get(Resources::SCARED_GHOST, ghost->isOutOfCage(), ghost->getDirection());
	else return *Resources::get(ghost->getResourceID(), ghost->isOutOfCage(), ghost->getDirection());
}
