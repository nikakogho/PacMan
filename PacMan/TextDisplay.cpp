#include "TextDisplay.h"
#include "Core.h"

const std::string TextDisplay::instructions("Use WSAD or arrow keys to turn. Eat dots to get scores. Eat fruits (big circles) to temporarily scare and eat ghosts. Avoid ghosts unless they're scared of you!");
const std::string TextDisplay::scorePrefix("Score: ");
const std::string TextDisplay::leaderboardPrefix("Leaderboard: ");
const std::string TextDisplay::noLeaderboardText("No leaderboard so far");

TextDisplay::TextDisplay()
	:debug("Text Display")
{
	leaderboard = noLeaderboardText; //TODO
	score = 0;

	font.loadFromFile("Resources/Fonts/CrackMan.TTF");

	texts = { &instructionsText, &scoreText, &leaderboardText };

	for(auto text : texts) // setup the visual of each text
	{
		text->setFont(font);
		text->setCharacterSize(24);
		text->setFillColor(sf::Color::White);
	}

	// position each text
	instructionsText.setPosition(Core::SCREEN_WIDTH / 2 - 150, Core::SCREEN_HEIGHT - 50);
	scoreText.setPosition(10, 10);
	leaderboardText.setPosition(10, 50);

	// setup texts
	instructionsText.setString(instructions);
	scoreText.setString(scorePrefix + "0");
	leaderboardText.setString(leaderboard);
}

void TextDisplay::update_leaderboard(std::vector<std::pair<std::string, int>> _leaderboard)
{
	leaderboard = leaderboardPrefix;
	for(int i = 1; i <= _leaderboard.size(); i++)
	{
		auto data = _leaderboard[i - 1];
		leaderboard += "\n" + to_string(i) + ") " + data.first + ": " + to_string(data.second);
	}

	leaderboardText.setString(leaderboard);
}

void TextDisplay::update_score(int _score)
{
	score = _score;

	scoreText.setString(scorePrefix + to_string(score));
}

void TextDisplay::draw(sf::RenderWindow & window) const
{
	for (auto text : texts) window.draw(*text); // draw each text on screen
}
