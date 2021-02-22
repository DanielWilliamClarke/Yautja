#pragma once
#include <ctime>

namespace NLS
{
	time_t secondsCurrent(); //returns system time in microseconds
	int pwr2(int noToSqr);
	std::string timeString(time_t current);
}