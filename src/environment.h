#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <memory>

#include "segment.h"

#define WIDTHBUFFER 300
#define HEIGHTBUFFER 100

class IGraphics;

class Environment
{
public:
	Environment(std::shared_ptr<IGraphics> gfx, AIPreferences& PreyPreferences, AIPreferences& PredPreferences, int grid, int plant, int obs, int prey, int pred);
	virtual ~Environment();

	void populateGrid(int NUMOFPREY, AIPreferences& PreyPreferences, int NUMOFPRED, AIPreferences& PredPreferences);
	void drawSimulation(sf::RenderWindow& window);
	void iterateSimulation(int  gameCount);
	std::pair<unsigned int, unsigned int> getEntities();

private:

	void _manageFood(int gameCount);
	void _pushSubScope(void);
	std::vector<Segment> _AIScopeSubArray(int xPos, int yPos, int AIScope);

private:

	std::shared_ptr<IGraphics> gfx;
	std::ofstream logFile_;
	unsigned long int currentIteration_;
	std::vector<Segment> grid_;
	std::vector<std::pair<int, int> > preyXY_;
	std::vector<std::pair<int, int> > predXY_;
	std::vector<std::pair<int, int> > foodXY_;
	int gridSize_;
	int obstacles_;
	int plants_;
};