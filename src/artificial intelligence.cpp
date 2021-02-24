// Filename: artificial intelligence.cpp
// Author: Alex Rudd
// ID: N0321342
// Version: 1.0
// Date: 30/11/2011
// Description: function library for the ArtIntell class which is inherited by Predator and Prey

#include "artificial intelligence.h"


ArtIntell::ArtIntell(AIPreferences & Preferences, int identifier)
{
	p_ = Preferences;
	maxEnergy_ = p_.energy*2;
	direction_.resize(5);
	identifier_ = identifier;
	memory_ = new std::list<std::pair <std::vector<unsigned int>, unsigned int> >;
}
unsigned int ArtIntell::getScope()
{
	return p_.scope;
}
void ArtIntell::setMemory(std::vector<unsigned int> newMemory)
{
    memory_->push_front(std::pair <std::vector<unsigned int>, unsigned int>(newMemory, 0));
}
unsigned int ArtIntell::largestElement(std::vector<unsigned int> vect)
{
	int sum  = 0;
	unsigned int tempProb = 0;

	for (unsigned int i = 0; i < vect.size(); i++)
		sum += vect[i];
	
	tempProb = rand()% sum;

	for(unsigned int idx = 0, sum = 0; idx < vect.size();idx++)
	{
		sum += vect[idx];

		if(tempProb < sum)
			return idx;
	}

	return NO_MOVE;
}

std::pair<unsigned int, unsigned int> ArtIntell::moveDeterminent(std::vector<unsigned int> newMemory, std::vector<unsigned int> messages)
{
	setMemory(newMemory);
	return calculateMovement(messages);
}

std::pair<unsigned int, unsigned int> ArtIntell::calculateMovement(std::vector<unsigned int> messages)
{

	if(p_.energy < 0)   //this AI has no energy and is deleted
	{
        return std::pair<unsigned int, unsigned int>(DEATH, (++memory_->begin())->second);		//return code 0 - environment can NULL this AI segment
	}

	unsigned int msg = 0;
	unsigned int scopeRad = p_.scope/2;

	//initialise direction
	for(unsigned int idx = 0;idx < direction_.size(); idx++)
		direction_[idx] = 0;

	for(unsigned int idx = 0; idx < p_.scope; idx++) //X value
	{
		for(unsigned int jdx = 0; jdx < p_.scope; jdx++) //Y value
		{
			switch(memory_->front().first[(p_.scope * idx) + jdx])
			{
			case 0:		//OOB
				if(idx < scopeRad)	//OOB West
					direction_[EAST] += p_.OOB;
				else if(idx > scopeRad) //OOB East
					direction_[WEST] += p_.OOB;
				if(jdx < scopeRad)	//OOB North
					direction_[SOUTH] += p_.OOB;
				else if(jdx > scopeRad)	//OOB South
					direction_[NORTH] += p_.OOB;
				break;
			case 1:		//Empty Segment
				if(idx < scopeRad)	//Empty Space West
					direction_[WEST] += p_.empty;
				else if(idx > scopeRad) //Empty Space East
					direction_[EAST] += p_.empty;
				if(jdx < scopeRad)	//Empty Space North
					direction_[NORTH] += p_.empty;
				else if(jdx > scopeRad)	//Empty Space South
					direction_[SOUTH] += p_.empty;
				break;
			case 2:		//Obstacle
				//NOTHING
				break;
			case 3:		//Predator + Prey
			case 4:		//Prey + Food
			case 5:		//Just Pred
            case 6:		//Just Prey
			case 7:		//Just Food
				{
					std::pair<std::vector<unsigned int>, unsigned int> temp;
                    temp = AISpecificOperation(std::pair<std::vector<unsigned int>, std::vector<unsigned int> >(direction_, messages), std::pair<unsigned int, unsigned int>(idx, jdx), (memory_->front().first[(p_.scope * idx) + jdx])); 
					direction_ = temp.first;
					msg = temp.second;
					break;
				}
			default:
				break;
			}
		}
	}

	//check if anydirections are out of bounds or already contain Prey/Obstacles
	validateMove(direction_);

	(p_.energy > maxEnergy_)? p_.energy = maxEnergy_ : p_.energy -= 4;	//energy used in move
	
	if((p_.energy < maxEnergy_/5) && (msg != 3) && (msg != 4))
		msg = 6;

	switch(previousMove_)
	{
	case NORTH:
		direction_[NORTH] *= 2;
		direction_[SOUTH] = 0;
		break;
	case EAST:
		direction_[EAST] *= 2;
		direction_[WEST] = 0;
		break;
	case SOUTH:
		direction_[SOUTH] *= 2;
		direction_[NORTH] = 0;
		break;
	case WEST:
		direction_[WEST] *= 2;
		direction_[EAST] = 0;
		break;
	case NO_MOVE:
		direction_[NO_MOVE] *= 3;
		direction_[NORTH] /= 3;
		direction_[SOUTH] /= 3;
		direction_[EAST] /= 3;
		direction_[WEST] /= 3;
	}

	if(memory_->size() > 10)
		memory_->erase(--memory_->end());

	if ((direction_[NORTH] == 0) && (direction_[EAST] == 0) && (direction_[SOUTH] == 0) && (direction_[WEST] == 0))
	{
		previousMove_ = NO_MOVE;
        memory_->begin()->second = msg;
		return std::pair<unsigned int, unsigned int>(NO_MOVE, msg);
	}
	else
	{
        memory_->begin()->second = msg;
		previousMove_ = largestElement(direction_);
		return std::pair<unsigned int, unsigned int>(previousMove_, msg);
	}
}
double ArtIntell::getHealth()
{
	return p_.energy*(100.0/maxEnergy_);
}
int ArtIntell::getIdenifier()
{
	return identifier_;
}