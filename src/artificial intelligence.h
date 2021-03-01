#pragma once
#include <list>
#include <vector>

#include "lib.h"

const int DEATH = 100;
const int NORTH = 0;
const int EAST = 1;
const int SOUTH = 2;
const int WEST = 3;
const int NO_MOVE = 4;

struct AIPreferences
{
	int OOB,
		empty,
		prdPry,
		pryFod,
		pred,
		prey,
		food,
		energy,
		moveThreshold;
	unsigned int scope;
	int foodGain,
		conflict;
	bool smartAI;
};

class ArtIntell
{
protected:
	int previousMove_;
	int maxEnergy_;
	int identifier_;
	bool intelligent_;
	AIPreferences p_;

	std::vector <unsigned int> direction_;
    std::list<std::pair <std::vector<unsigned int >, unsigned int > > * memory_;

public:

	virtual std::vector<unsigned int> validateMove(std::vector <unsigned int> & direction)=0;
	virtual std::pair<std::vector<unsigned int>, unsigned int> AISpecificOperation(std::pair<std::vector<unsigned int>, std::vector<unsigned int> > & dirMsg, std::pair<unsigned int, unsigned int> XYpair, int AICase)=0;

	ArtIntell(AIPreferences & Preferences, int identifier);
	unsigned int getScope();
	void setMemory(std::vector<unsigned int> newMemory);
	unsigned int largestElement(std::vector<unsigned int>);
	std::pair<unsigned int, unsigned int> moveDeterminent(std::vector<unsigned int> newMemory, std::vector<unsigned int> messages);
	std::pair<unsigned int, unsigned int> calculateMovement(std::vector<unsigned int> messages);
	double getHealth();
	int getIdenifier();
};
