#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#include "segment.h"

#define WIDTHBUFFER 200
#define HEIGHTBUFFER 100

class Environment
{
public:
	//allow for changes in options

	Environment(int grid, int plant, int obs, int prey, int pred, AIPreferences& PreyPreferences, AIPreferences& PredPreferences, sf::RenderWindow& window);
	~Environment();
	void populateGrid(int NUMOFPREY, AIPreferences& PreyPreferences, int NUMOFPRED, AIPreferences& PredPreferences);
	void drawSimulation(sf::RenderWindow& window);
	void iterateSimulation(int  gameCount);

	std::pair<unsigned int, unsigned int> getEntities();

private:
	std::ofstream logFile_;
	unsigned long int currentIteration_;
	std::vector<Segment> grid_;
	std::vector<std::pair<int, int> > preyXY_;
	std::vector<std::pair<int, int> > predXY_;
	std::vector<std::pair<int, int> > foodXY_;

	void _manageFood(int gameCount);
	void _pushSubScope(void);

	std::vector<Segment> _AIScopeSubArray(int xPos, int yPos, int AIScope);

	std::shared_ptr<sf::Texture> _Grass;
	std::shared_ptr<sf::Texture> _Berry;
	std::shared_ptr<sf::Texture> _Rock;
	std::shared_ptr<sf::Texture> _Prey;
	std::shared_ptr<sf::Texture> _Pred;

	int gridSize_;
	int obstacles_;
	int plants_;
};