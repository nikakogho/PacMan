#include "AudioManager.h"

Debug AudioManager::debug("Audio");
sf::SoundBuffer* AudioManager::coinCollectingSoundBuffer; 
sf::SoundBuffer* AudioManager::fruitEatenSoundBuffer;
sf::SoundBuffer* AudioManager::ghostEatenSoundBuffer;
sf::SoundBuffer* AudioManager::deathSoundBuffer;
sf::Music* AudioManager::openingMusic;

sf::Sound* AudioManager::coinCollectingSound;
sf::Sound* AudioManager::fruitEatenSound;
sf::Sound* AudioManager::ghostEatenSound;
sf::Sound* AudioManager::deathSound;

void AudioManager::init()
{
	debug.logLine("Starting init");

	debug.logLine("Started loading audio files");

	// initialize buffers
	coinCollectingSoundBuffer = new sf::SoundBuffer();
	fruitEatenSoundBuffer = new sf::SoundBuffer();
	ghostEatenSoundBuffer = new sf::SoundBuffer();
	deathSoundBuffer = new sf::SoundBuffer();

	openingMusic = new sf::Music();

	// load audio files
	coinCollectingSoundBuffer->loadFromFile("Resources/Sounds/pacman_chomp.wav");
	fruitEatenSoundBuffer->loadFromFile("Resources/Sounds/pacman_eatfruit.wav");
	ghostEatenSoundBuffer->loadFromFile("Resources/Sounds/pacman_eatghost.wav");
	deathSoundBuffer->loadFromFile("Resources/Sounds/pacman_death.wav");

	openingMusic->openFromFile("Resources/Sounds/pacman_beginning.wav");

	debug.logLine("Done loading audio files");

	// may do more

	debug.logLine("Done with init");
}

void AudioManager::play(int audioID)
{
	switch(audioID) // check what kind of sound we want to play
	{
	case OPENING_MUSIC_ID:
		openingMusic->play(); // play the music
		break;
	case COIN_COLLECTING_SOUND_ID:
		delete coinCollectingSound; // delete old sound object
		coinCollectingSound = new sf::Sound(*coinCollectingSoundBuffer); // create new sound object
		coinCollectingSound->play(); // play the sound
		break;
	case FRUIT_EATEN_SOUND_ID:
		delete fruitEatenSound; // delete old sound object
		fruitEatenSound = new sf::Sound(*fruitEatenSoundBuffer); // create new sound object
		fruitEatenSound->play(); // play the sound
		break;
	case GHOST_EATEN_SOUND_ID:
		delete ghostEatenSound; // delete old sound object
		ghostEatenSound = new sf::Sound(*ghostEatenSoundBuffer); // create new sound object
		ghostEatenSound->play(); // play the sound
		break;
	case DEATH_SOUND_ID:
		delete deathSound;
		deathSound = new sf::Sound(*deathSoundBuffer); // create new sound object
		deathSound->play(); // play the sound
		break;
	default:
		debug.logLine("There is no audio associated with ID " + to_string(audioID));
	}
}
