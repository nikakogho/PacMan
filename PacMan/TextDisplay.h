#pragma once

#include "SFML/Graphics.hpp"
#include "Debug.h"

class TextDisplay
{
	Debug debug;

	const static std::string instructions; // instructions text (string)
	const static std::string leaderboardPrefix;
	const static std::string noLeaderboardText;
	const static std::string scorePrefix;

	std::string leaderboard; // leaderboard text (string)
	int score; // text (string) of score we have right now (amount of dots we ate)

	sf::Font font; // font used for text

	// sf::Text objects for displaying texts
	sf::Text instructionsText;
	sf::Text scoreText;
	sf::Text leaderboardText;

	vector<sf::Text*> texts; // all texts, stored for similar operations

public:
	TextDisplay(); // constructor

	void update_leaderboard(std::vector<std::pair<std::string, int>> leaderboard); // update the leaderboard
	void update_score(int score); // update the text to display
	void draw(sf::RenderWindow& window) const; // draw text to screen
};