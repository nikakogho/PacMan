#pragma once

#include "SFML/Graphics.hpp"

class Animation 
{
	int frames; // amount of frames this animation (sprite sheet) has
	sf::IntRect* anim_bounds; // the entire sprite sheet
	sf::Clock timer; // timer to keep track of which frame to display
	int resetValue; // first frame in anim_bounds

	const float FRAME_DURATION = 0.1f; // duration of each frame

public:
	Animation(sf::IntRect* rect, int frames); // constructor
	void changeFrame(); // going to next frame
	sf::IntRect getBounds() const; // get current frame
};