#pragma once

#include "artificial intelligence.h"

class Predator: public ArtIntell
{
private:

public:
	Predator(AIPreferences & Preferences, int identifier);
	std::vector<unsigned int> validateMove(std::vector<unsigned int> & direction);
	std::pair<std::vector<unsigned int>, unsigned int> AISpecificOperation(std::pair<std::vector<unsigned int>, std::vector<unsigned int> > & dirMsg, std::pair<unsigned int, unsigned int> XYpair, int predCase);
};