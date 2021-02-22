#pragma once

#include "prey.h"
#include "predator.h"
#include "obstacle.h"
#include "food.h"

class Segment
{
private:
	Prey * pry_;
	Predator * prd_;
	Obstacle * obs_;
	Food * fod_;

	std::vector<Segment> subScope_;
	std::string message_;

	bool outOfBounds_;
public:

	Segment(void);
	void clearSegment();
	void setPrey(Prey *newPrey);
	void setPredator(Predator *newPred);
	void setObstacle(Obstacle *newObstacle);
	void setFood(Food *newFood);
	void setOOB(bool newOOB);
	void setSubScope(std::vector<Segment> newScope);
	void clearSubScope(void);
	int convertMessageToInt(std::string message);
	std::string convertIntToMessage(int message);

	std::pair<unsigned int, std::string> convertScopeToInt(ArtIntell *currentAI);
	Prey* getPrey(void);
	Predator* getPredator(void);
	Obstacle* getObstacle(void);
	Food* getFood(void);
	bool getOOB(void);
	std::string getMessage(void);
	void setMessage(std::string newMessage);
};

