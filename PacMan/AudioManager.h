#pragma once

#include "SFML/Audio.hpp"
#include "Debug.h"

class AudioManager 
{
	static Debug debug; // debugger to tell us status of things 
	static sf::SoundBuffer *coinCollectingSoundBuffer, *fruitEatenSoundBuffer, *ghostEatenSoundBuffer, *deathSoundBuffer; // sound buffers
	static sf::Music *openingMusic; // long music so we use sf::Music instead

	static sf::Sound *coinCollectingSound, *fruitEatenSound, *ghostEatenSound, *deathSound; // sounds

public:
	const static int OPENING_MUSIC_ID = 1;
	const static int COIN_COLLECTING_SOUND_ID = 2;
	const static int FRUIT_EATEN_SOUND_ID = 3;
	const static int GHOST_EATEN_SOUND_ID = 4;
	const static int DEATH_SOUND_ID = 5;

	static void init(); // initialize all sounds
	static void play(int audioID); // play the specified audio
};