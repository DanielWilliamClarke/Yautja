// Filename:
// Author:
// ID:
// Version:
// Date: 
// Description:
#include "lib.h"

time_t NLS::secondsCurrent()
{
	return time(NULL);
}

int NLS::pwr2(int noToSqr)
{
	return (noToSqr * noToSqr);
}
std::string NLS::timeString(time_t current)
{
	char buff[20];
	strftime(buff, 20, "%Y-%m-%d %H.%M.%S", localtime(&current));
	return std::string(buff);
}