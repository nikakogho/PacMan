#include "Animation.h"

Animation::Animation(sf::IntRect * rect, int frames)
:anim_bounds(rect), frames(frames), resetValue(rect->left)
{

}

void Animation::changeFrame()
{
	if(timer.getElapsedTime().asSeconds() >= FRAME_DURATION) // enough time passed, time to move to next frame
	{
		if (anim_bounds->left < resetValue + 15 * (frames - 1)) // still more frames to go
			anim_bounds->left += 15; // move to next frame
		else anim_bounds->left = resetValue; // reset to first frame
		timer.restart(); // restart counting
	}
}

sf::IntRect Animation::getBounds() const
{
	return *anim_bounds; // get current frame
}
