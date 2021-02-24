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


class GUIsim
{
public:
	//std::ofstream logFile_;
	bool isComplete(std::pair<unsigned int, unsigned int> entities, int iterationCount);
	bool gameStarted;
	bool simEnd;
	bool simQuit;
	GUIsim();
	void mouseEvent(int mouseX, int mouseY, sf::RenderWindow& window);
	simSettings getSettings();
	void drawMenu(sf::RenderWindow& window);
	unsigned int statusBar(sf::RenderWindow& window);
	void endGameScreen(sf::RenderWindow& window, std::pair<unsigned int, unsigned int> entities, int iterationCount);
	void legend(sf::RenderWindow& window);
	void outPut(std::pair<unsigned int, unsigned int> entities, int iterationCount);
	
private:
	ScreenTypes currentScreen_;
	simSettings simulationSettings_;
	unsigned int sleepSliderV_;
	unsigned int defeatIteration_;
	std::vector< std::pair<std::string, std::string> > teamAndRoles_;

	void mainScreen_(sf::RenderWindow& window);
	void optionScreen_(sf::RenderWindow& window);
	void creditScreen_(sf::RenderWindow& window);
	
	bool gameEnd();

	std::shared_ptr<sf::Texture> loadTexture(std::string texturePath) const;
	std::shared_ptr<sf::Texture> findTexture(std::string textureName) const;

	std::map<std::string, std::shared_ptr<sf::Texture>> textures;

	bool isToggled_;
};
	