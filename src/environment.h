#pragma once

#include "segment.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#define WIDTHBUFFER 200
#define HEIGHTBUFFER 100

class Environment
{
public:
	//allow for changes in options

	Environment(int grid, int plant, int obs, int prey, int pred, AIPreferences& PreyPreferences, AIPreferences& PredPreferences, GWindow & Gwin);
	~Environment();
	void populateGrid(int NUMOFPREY, AIPreferences& PreyPreferences, int NUMOFPRED, AIPreferences& PredPreferences);
	void drawSimulation(GWindow& Gwin);
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

	GImage *_Grass;
	GImage *_Berry;
	GImage *_Rock;
	GImage *_Prey;
	GImage *_Pred;

	int gridSize_;
	int obstacles_;
	int plants_;
};