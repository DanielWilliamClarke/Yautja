// Filename:
// Author: Daniel Clarke
// ID:
// Version:
// Date: 9th March
// Description:
#include "food.h"

int Food::getTIP()
{
	return timeInPlay_;
}
void Food::setTIP(int newTimeInPlay)
{
	timeInPlay_ = newTimeInPlay;
}