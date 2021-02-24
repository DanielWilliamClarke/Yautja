// Filename: predator.cpp
// Author: Alex Rudd
// ID: N0321342
// Version: 1.0
// Date: 30/11/2011
// Description: function library for the Predator class

#include "predator.h"

Predator::Predator(AIPreferences & Preferences, int identifier) : ArtIntell(Preferences, identifier)
{
	intelligent_ = false;
}
std::vector <unsigned int> Predator::validateMove(std::vector <unsigned int> & direction)
{
	unsigned int scopeRad = p_.scope/2;
	if( memory_->front().first[(p_.scope * scopeRad) + (scopeRad - 1)] == 0 || 
		memory_->front().first[(p_.scope * scopeRad) + (scopeRad - 1)] == 2 ||
		memory_->front().first[(p_.scope * scopeRad) + (scopeRad - 1)] == 3 ||
		memory_->front().first[(p_.scope * scopeRad) + (scopeRad - 1)] == 5 )
			direction[NORTH] = 0;
	if( memory_->front().first[(p_.scope * (scopeRad + 1)) + scopeRad] == 0 || 
		memory_->front().first[(p_.scope * (scopeRad + 1)) + scopeRad] == 2 ||
		memory_->front().first[(p_.scope * (scopeRad + 1)) + scopeRad] == 3 ||
		memory_->front().first[(p_.scope * (scopeRad + 1)) + scopeRad] == 5 )
			direction[EAST] = 0;
	if( memory_->front().first[(p_.scope * scopeRad) + (scopeRad + 1)] == 0 || 
		memory_->front().first[(p_.scope * scopeRad) + (scopeRad + 1)] == 2 ||
		memory_->front().first[(p_.scope * scopeRad) + (scopeRad + 1)] == 3 ||
		memory_->front().first[(p_.scope * scopeRad) + (scopeRad + 1)] == 5 )
			direction[SOUTH] = 0;
	if( memory_->front().first[(p_.scope * (scopeRad - 1)) + scopeRad] == 0 || 
		memory_->front().first[(p_.scope * (scopeRad - 1)) + scopeRad] == 2 ||
		memory_->front().first[(p_.scope * (scopeRad - 1)) + scopeRad] == 3 ||
		memory_->front().first[(p_.scope * (scopeRad - 1)) + scopeRad] == 5 )
			direction[WEST] = 0;

	return direction;
}
std::pair<std::vector<unsigned int>, unsigned int> Predator::AISpecificOperation(std::pair<std::vector<unsigned int>, std::vector<unsigned int> > & dirMsg, std::pair<unsigned int, unsigned int> XYpair, int predCase)
{
	unsigned int scopeRad = p_.scope/2;
	switch(predCase)
	{
		case 3:		//Predator + Prey
			if(XYpair.first == scopeRad && XYpair.second == scopeRad)	//Caught Prey!
				p_.energy += p_.conflict;			//conflict. gain energy 
			else
			{
				if(XYpair.first > scopeRad)	//Pred + Prey are North
					dirMsg.first[EAST] += p_.prdPry;
				else if(XYpair.first < scopeRad) //Pred + Prey are South
					dirMsg.first[WEST] += p_.prdPry;
				if(XYpair.second > scopeRad)	//Pred + Prey are West
					dirMsg.first[SOUTH] += p_.prdPry;
				else if(XYpair.second < scopeRad)	//Pred + Prey are East
					dirMsg.first[NORTH] += p_.prdPry;
			}
			break;
		case 4:		//Prey + Food
					//chase the Prey!
			if(XYpair.first > scopeRad)	//Food + Prey are North
				dirMsg.first[EAST] += p_.pryFod;
			else if(XYpair.first < scopeRad) //Food  + Prey are South
				dirMsg.first[WEST] += p_.pryFod;
			if(XYpair.second > scopeRad)	//Food  + Prey are West
				dirMsg.first[SOUTH] += p_.pryFod;
			else if(XYpair.second < scopeRad)	//Food  + Prey are East
				dirMsg.first[NORTH] += p_.pryFod;
			break;
		case 5:		//Just Pred
			if(XYpair.first > scopeRad)	//Pred is North
				dirMsg.first[WEST] += p_.pred;
			if(XYpair.first < scopeRad) //Pred is South
				dirMsg.first[EAST] += p_.pred;
			if (XYpair.second > scopeRad)	//Pred is West
				dirMsg.first[NORTH] += p_.pred;
			if(XYpair.second < scopeRad)	//Pred is East
				dirMsg.first[SOUTH] += p_.pred;
			break;
		case 6:		//Just Prey
			if(( memory_->front().first[(p_.scope * scopeRad) + (scopeRad - 1)] == 6) ||
			  ( memory_->front().first[(p_.scope * (scopeRad + 1)) + scopeRad] == 6) ||
			  ( memory_->front().first[(p_.scope * scopeRad) + (scopeRad + 1)] == 6) ||
			  ( memory_->front().first[(p_.scope * (scopeRad - 1)) + scopeRad] == 6))
				p_.energy += p_.foodGain;

            for(unsigned int i = 0; i < 4; i++)
					direction_[i]/= 3;

			if(XYpair.first > scopeRad)	//Prey is North
				dirMsg.first[EAST] = (dirMsg.first[EAST] * 3) + p_.prey;
			else if(XYpair.first < scopeRad) //Prey is South
				dirMsg.first[WEST] = (dirMsg.first[WEST] * 3) + p_.prey;
            if(XYpair.second > scopeRad)	//Prey is West
				dirMsg.first[SOUTH] = (dirMsg.first[SOUTH] * 3) + p_.prey;
			else if(XYpair.second < scopeRad)	//Prey is East
				dirMsg.first[NORTH] = (dirMsg.first[NORTH] * 3) + p_.prey;
	}
	return std::pair<std::vector<unsigned int>, unsigned int>(dirMsg.first, 0);
}