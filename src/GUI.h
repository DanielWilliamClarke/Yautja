#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <memory>

#include "segment.h"

#define BUTTONWIDTH 250
#define BUTTONHEIGHT 50

enum ScreenTypes {
	mainScreen = 0,
	optionScreen,
	creditScreen,
};

struct simSettings {
	unsigned int gridSize;
	unsigned int numOfObstacles;
	unsigned int numOfPred;
	unsigned int numOfPrey;
	unsigned int numOfFood;
	unsigned int simTimeOut;
	bool intelligence;
};

class IGraphics;

class GUIsim
{
public:
	GUIsim(std::shared_ptr<IGraphics> gfx);
	virtual ~GUIsim() = default;

	bool isComplete(std::pair<unsigned int, unsigned int> entities, int iterationCount);
	void mouseEvent(int mouseX, int mouseY, sf::RenderWindow& window);
	simSettings getSettings();
	void drawMenu(sf::RenderWindow& window);
	unsigned int statusBar(sf::RenderWindow& window);
	void endGameScreen(sf::RenderWindow& window, std::pair<unsigned int, unsigned int> entities, int iterationCount);
	void legend(sf::RenderWindow& window);
	void outPut(std::pair<unsigned int, unsigned int> entities, int iterationCount);

public:

	bool gameStarted;
	bool simEnd;
	bool simQuit;
	
private:

	void mainScreen_(sf::RenderWindow& window);
	void optionScreen_(sf::RenderWindow& window);
	void creditScreen_(sf::RenderWindow& window);
	bool gameEnd();
	void showOption(sf::RenderWindow& window, std::string title, unsigned int value, float height) const;

private:

	std::shared_ptr<IGraphics> gfx;
	ScreenTypes currentScreen_;
	simSettings simulationSettings_;
	unsigned int sleepSliderV_;
	unsigned int defeatIteration_;
	std::vector< std::pair<std::string, std::string> > teamAndRoles_;
	bool isToggled_;
};
	