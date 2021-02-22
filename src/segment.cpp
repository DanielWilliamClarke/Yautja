// Filename: Segment.cpp
// Author: Alex Rudd
// ID: N0321342
// Version: 1.0
// Date: 30/11/2011
// Description: function library for the Segment class
#pragma once
#include "stdafx.h"
#include "segment.h"

Segment::Segment(void)
{
	srand(unsigned int(time(NULL)));
	pry_ = NULL;
	prd_ = NULL;
	obs_ = NULL;
	fod_ = NULL;
	message_ = "hello :3";
	setOOB(false);
}
void Segment::clearSegment()
{
	delete pry_;
	delete prd_;
	delete obs_;
	delete fod_;
}
void Segment::setPrey(Prey *newPrey)
{
	pry_ = newPrey;
}
void Segment::setPredator(Predator *newPred)
{
	prd_ = newPred;
}
void Segment::setObstacle(Obstacle *newObstacle)
{
	obs_ = newObstacle;
}
void Segment::setFood(Food *newFood)
{
	fod_ = newFood;
}
void Segment::setOOB(bool newOOB)
{
	outOfBounds_ = newOOB;
}
void Segment::setSubScope(std::vector<Segment> newScope)
{
	subScope_.assign(newScope.begin(), newScope.end());
}
std::pair<unsigned int, std::string> Segment::convertScopeToInt(ArtIntell *currentAI) //Converts a 2D vector of segments into a 2D vector of flag integers
{
	std::vector<unsigned int> intSubScope, messages;

	intSubScope.resize(NLS::pwr2(currentAI->getScope()));

	for(unsigned int idx = 0; idx < currentAI->getScope(); idx++)
		for(unsigned int jdx = 0; jdx < currentAI->getScope(); jdx++)
		{
			if( subScope_[(currentAI->getScope() * idx) + jdx].getOOB() == true)	//OOB is set to false
				intSubScope[(currentAI->getScope() * idx) + jdx] = 0;					//Can't Move there

			else if
				((subScope_[(currentAI->getScope() * idx) + jdx].getPredator() == NULL) &&
				(subScope_[(currentAI->getScope() * idx) + jdx].getPrey() == NULL) &&		//Segment is empty
				(subScope_[(currentAI->getScope() * idx) + jdx].getObstacle() == NULL) &&
				(subScope_[(currentAI->getScope() * idx) + jdx].getFood() == NULL))
					intSubScope[(currentAI->getScope() * idx) + jdx] = 1;						//Can Move there

			else if(subScope_[(currentAI->getScope() * idx) + jdx].getObstacle() != NULL)	//There is an obstacle
				intSubScope[(currentAI->getScope() * idx) + jdx] = 2;							//can't in to obstacle

			else if
				((subScope_[(currentAI->getScope() * idx) + jdx].getPredator() != NULL) &&
				(subScope_[(currentAI->getScope() * idx) + jdx].getPrey() != NULL))
			{
					intSubScope[(currentAI->getScope() * idx) + jdx] = 3;						//Predator and Prey
					messages.push_back(convertMessageToInt(subScope_[(currentAI->getScope() * idx) + jdx].getMessage()));
			}

			else if
				((subScope_[(currentAI->getScope() * idx) + jdx].getFood() != NULL) &&
				(subScope_[(currentAI->getScope() * idx) + jdx].getPrey() != NULL))
			{
					intSubScope[(currentAI->getScope() * idx) + jdx] = 4;						//Prey and Food
					messages.push_back(convertMessageToInt(subScope_[(currentAI->getScope() * idx) + jdx].getMessage()));
			}

			else if(subScope_[(currentAI->getScope() * idx) + jdx].getPredator() != NULL)
				intSubScope[(currentAI->getScope() * idx) + jdx] = 5;							//Just a Predator

			else if(subScope_[(currentAI->getScope() * idx) + jdx].getPrey() != NULL)
			{
				intSubScope[(currentAI->getScope() * idx) + jdx] = 6;		//Just a Prey
				messages.push_back(convertMessageToInt(subScope_[(currentAI->getScope() * idx) + jdx].getMessage()));
			}

			else if(subScope_[(currentAI->getScope() * idx) + jdx].getFood() != NULL)
				intSubScope[(currentAI->getScope() * idx) + jdx] = 7;							//Just Food
		}


		std::pair<unsigned int, unsigned int> dirMsg = currentAI->moveDeterminent(intSubScope, messages);
		std::pair<unsigned int, std::string> dirStr;
		dirStr.first = dirMsg.first;
		dirStr.second = convertIntToMessage(dirMsg.second);

		return dirStr;
}
Prey* Segment::getPrey(void)
{
	return pry_;
}
Predator* Segment::getPredator(void)
{
	return prd_;
}
Obstacle* Segment::getObstacle(void)
{
	return obs_;
}
Food* Segment::getFood(void)
{
	return fod_;
}
bool Segment::getOOB(void)
{
	return outOfBounds_;
}

void Segment::setMessage(std::string newMessage)
{
	message_ = newMessage;
}
std::string Segment::getMessage(void)
{
	return message_;
}

void Segment::clearSubScope(void)
{
	subScope_.clear();
}
int Segment::convertMessageToInt(std::string message)
{
	if (message == "predator north :0")
	{
		return NORTH;
	}
	if (message == "predator south :0")
	{
		return SOUTH;
	}
	if (message == "predator east :0")
	{
		return EAST;
	}
	if (message == "predator west :0")
	{
		return WEST;
	}
	if (message == "help! D:")
	{
		return 4;
	}
	if ((message == "hello :3") || (message == "friend! :D") || (message == "hi :)"))
	{
		return 5;
	}
	if ((message == "hungry :<") || (message == "need food :S") || (message == "food plz :("))
	{
		return 6;
	}
	if (message == "food north ^^")
	{
		return 10;
	}
	if (message == "food south ^^")
	{
		return 11;
	}
	if (message == "food east ^^")
	{
		return 12;
	}
	if (message == "food west ^^")
	{
		return 13;
	}

	return 5;
}

std::string Segment::convertIntToMessage(int message)
{
	if (message == NORTH)
	{
		return "pred north";
	}
	if (message == SOUTH)
	{
		return "pred south";
	}
	if (message == EAST)
	{
		return "pred east";
	}
	if (message == WEST)
	{
		return "pred west";
	}
	if (message == 4)
	{
		return "help! D:";
	}
	if (message == 5)
	{
		return "hello :3";
	}
	if (message == 6)
	{
		return "hungry :<";
	}
	if (message == 10)
	{
		return "food north ^^";
	}
	if (message == 11)
	{
		return "food south ^^";
	}
	if (message == 12)
	{
		return "food east ^^";
	}
	if (message == 13)
	{
		return "food west ^^";
	}

	return "";
}
