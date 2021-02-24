// Filename: prey.cpp
// Author: Alex Rudd
// ID: N0321342
// Version: 1.0
// Date: 30/11/2011
// Description: function library for the Prey class

#include "prey.h"

Prey::Prey(AIPreferences & Preferences, int identifier) : ArtIntell(Preferences, identifier)
{
	intelligent_ = Preferences.smartAI;
}
std::vector<unsigned int> Prey::validateMove(std::vector<unsigned int> & direction)
{
	unsigned int scopeRad = p_.scope/2;
	if( memory_->front().first[(p_.scope * scopeRad) + (scopeRad - 1)] == 0 || 
		memory_->front().first[(p_.scope * scopeRad) + (scopeRad - 1)] == 2 ||
		memory_->front().first[(p_.scope * scopeRad) + (scopeRad - 1)] == 3 ||
		memory_->front().first[(p_.scope * scopeRad) + (scopeRad - 1)] == 4 ||
		memory_->front().first[(p_.scope * scopeRad) + (scopeRad - 1)] == 6 )
			direction[NORTH] = 0;
	if( memory_->front().first[(p_.scope * (scopeRad + 1)) + scopeRad] == 0 || 
		memory_->front().first[(p_.scope * (scopeRad + 1)) + scopeRad] == 2 ||
		memory_->front().first[(p_.scope * (scopeRad + 1)) + scopeRad] == 3 ||
		memory_->front().first[(p_.scope * (scopeRad + 1)) + scopeRad] == 4 ||
		memory_->front().first[(p_.scope * (scopeRad + 1)) + scopeRad] == 6 )
			direction[EAST] = 0;
	if( memory_->front().first[(p_.scope * scopeRad) + (scopeRad + 1)] == 0 || 
		memory_->front().first[(p_.scope * scopeRad) + (scopeRad + 1)] == 2 ||
		memory_->front().first[(p_.scope * scopeRad) + (scopeRad + 1)] == 3 ||
		memory_->front().first[(p_.scope * scopeRad) + (scopeRad + 1)] == 4 ||
		memory_->front().first[(p_.scope * scopeRad) + (scopeRad + 1)] == 6 )
			direction[SOUTH] = 0;
	if( memory_->front().first[(p_.scope * (scopeRad - 1)) + scopeRad] == 0 || 
		memory_->front().first[(p_.scope * (scopeRad - 1)) + scopeRad] == 2 ||
		memory_->front().first[(p_.scope * (scopeRad - 1)) + scopeRad] == 3 ||
		memory_->front().first[(p_.scope * (scopeRad - 1)) + scopeRad] == 4 ||
		memory_->front().first[(p_.scope * (scopeRad - 1)) + scopeRad] == 6 )
			direction[WEST] = 0;

	return direction;
}
std::pair<std::vector<unsigned int>, unsigned int> Prey::AISpecificOperation(std::pair<std::vector<unsigned int> , std::vector<unsigned int> > & dirMsg, std::pair<unsigned int, unsigned int> XYpair, int preyCase)
{
	unsigned int scopeRad = p_.scope/2;
	unsigned int msg = 5;

	switch(preyCase)
	{
		case 3:		//Predator + Prey
			if(XYpair.first == scopeRad && XYpair.second == scopeRad)	//Pred is in attacking
				{
					p_.energy += p_.conflict;			//lose energy 
					dirMsg.first[rand()%4] += 500;			//move in random direction (FLEE)
				}
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

			msg = 4;
			break;
		case 4:		//Prey + Food
			if((XYpair.first == scopeRad) && (XYpair.second == scopeRad))	//Food is here
			{
				dirMsg.first[NO_MOVE] += 150;
				p_.energy += p_.foodGain;

				for (int i = 0; i < 4; i++)
						dirMsg.first[i] /= 5;
			}
			else
			{
				if(XYpair.first > scopeRad)	//Food + Prey are North
					dirMsg.first[EAST] += p_.pryFod;
				else if(XYpair.first < scopeRad) //Food  + Prey are South
					dirMsg.first[WEST] += p_.pryFod;
				if(XYpair.second > scopeRad)	//Food  + Prey are West
					dirMsg.first[SOUTH] += p_.pryFod;
				else if(XYpair.second < scopeRad)	//Food  + Prey are East
					dirMsg.first[NORTH] += p_.pryFod;
			}
			break;
		case 5:		//Just Pred
			if(XYpair.first > scopeRad)	//Pred is North
			{
				dirMsg.first[WEST] += p_.pred;
				dirMsg.first[EAST] /= 5;
			}
			if(XYpair.first < scopeRad) //Pred is South
			{
				dirMsg.first[EAST] += p_.pred;
				dirMsg.first[WEST] /= 5;
			}
			if (XYpair.second > scopeRad)	//Pred is West
			{
				dirMsg.first[NORTH] += p_.pred;
				dirMsg.first[SOUTH] /= 5;
			}
			if(XYpair.second < scopeRad)	//Pred is East
			{
				dirMsg.first[SOUTH] += p_.pred;
				dirMsg.first[NORTH] /= 5;
			}

			if(( memory_->front().first[(p_.scope * scopeRad) + (scopeRad - 1)] == 5) ||
			  ( memory_->front().first[(p_.scope * (scopeRad + 1)) + scopeRad] == 5) ||
			  ( memory_->front().first[(p_.scope * scopeRad) + (scopeRad + 1)] == 5) ||
			  ( memory_->front().first[(p_.scope * (scopeRad - 1)) + scopeRad] == 5))
				p_.energy += p_.conflict/2;
			msg = 4;
			break;
        case 6:
            if(XYpair.first > scopeRad)	//Prey is North
				dirMsg.first[EAST] += p_.prey;
			else if(XYpair.first < scopeRad) //Prey is South
				dirMsg.first[WEST] += p_.prey;
            if(XYpair.second > scopeRad)	//Prey is West
				dirMsg.first[SOUTH] += p_.prey;
            else if(XYpair.second < scopeRad)	//Prey is East
				dirMsg.first[NORTH] += p_.prey;
            break;
		case 7:
			if(XYpair.first > scopeRad)	//Food is North
				dirMsg.first[EAST] += p_.food;
			else if(XYpair.first < scopeRad) //Food is South
				dirMsg.first[WEST] += p_.food;
			if(XYpair.second > scopeRad)	//Food is West
				dirMsg.first[SOUTH] += p_.food;
			else if(XYpair.second < scopeRad)	//Food is East
				dirMsg.first[NORTH] += p_.food;

			if(( memory_->front().first[(p_.scope * scopeRad) + (scopeRad - 1)] == 7) ||
			  ( memory_->front().first[(p_.scope * (scopeRad + 1)) + scopeRad] == 7) ||
			  ( memory_->front().first[(p_.scope * scopeRad) + (scopeRad + 1)] == 7) ||
			  ( memory_->front().first[(p_.scope * (scopeRad - 1)) + scopeRad] == 7))
				p_.energy += p_.foodGain/2;
			break;
	}

	if ((p_.smartAI) && (msg != 4))
	{
		std::list<std::pair<std::vector<unsigned int>, unsigned int> >::iterator it1;

		for (it1 = memory_->begin(); it1 != memory_->end(); ++it1)
		{
			for(unsigned int idx = 0; idx < p_.scope; idx++) //X value
			{
				for(unsigned int jdx = 0; jdx < p_.scope; jdx++) //Y value
				{
					switch(it1->first[(p_.scope * idx) + jdx])
					{
						case 5:		//Remembers a pred
						if(idx < scopeRad) //West
							msg = WEST;
						else if(idx > scopeRad) //East
							msg = EAST;
						if(jdx < scopeRad)	//North
							msg = NORTH;
						else if(jdx > scopeRad)	//South
							msg = SOUTH;
						break;
					}
				}
			}
		}

		for (unsigned int i = 0; i < dirMsg.second.size(); ++i)
		{
			switch(dirMsg.second[i])
			{
				case 0:
					dirMsg.first[SOUTH] += 500;
					break;
				case 2:
					dirMsg.first[NORTH] += 500;
					break;
				case 1:
					dirMsg.first[WEST] += 500;
					break;
				case 3:
					dirMsg.first[EAST] += 500;
					break;
				default:
					break;
			}

		}
	}
	return std::pair<std::vector<unsigned int>, unsigned int>(dirMsg.first, msg);
}