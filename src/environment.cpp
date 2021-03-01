// Filename: environment.cpp
// Author: Alex Rudd
// ID: N0321342
// Version: 1.0
// Date: 30/11/2011
// Description: function library for the Environment class
#include "environment.h"
#include "artificial intelligence.h"

#include "util/IGraphics.h"

Environment::Environment(std::shared_ptr<IGraphics> gfx, AIPreferences& PreyPreferences, AIPreferences& PredPreferences, int grid, int plant, int obs, int prey, int pred)
	: gfx(gfx), gridSize_(grid), plants_(plant), obstacles_(obs)
{
	int xTemp, yTemp;
	srand(time(NULL));

	//resize environment grid
	grid_.resize(NLS::pwr2(gridSize_));

	//construct objects
	for (int idx = 0; idx < obstacles_; idx++)
	{
		do
		{
			xTemp = rand() % gridSize_;
			yTemp = rand() % gridSize_;
		} while (grid_[(gridSize_ * (xTemp)) + yTemp].getObstacle() != NULL);

		grid_[(gridSize_ * (xTemp)) + yTemp].setObstacle(new Obstacle);
	}

	//construct food
	foodXY_.resize(plants_);
	for (int idx = 0; idx < plants_; ++idx)
	{
		do
		{
			xTemp = rand() % gridSize_;
			yTemp = rand() % gridSize_;
		} while (grid_[(gridSize_ * (xTemp)) + yTemp].getFood() != NULL || (grid_[(gridSize_ * (xTemp)) + yTemp].getObstacle() != NULL));

		grid_[(gridSize_ * (xTemp)) + yTemp].setFood(new Food);
		grid_[(gridSize_ * (xTemp)) + yTemp].getFood()->setTIP(rand() % 30);

		foodXY_[idx].first = xTemp;
		foodXY_[idx].second = yTemp;
	}

	populateGrid(prey, PreyPreferences, pred, PredPreferences);

	logFile_.open(std::string("NLS - ").append(NLS::timeString(time(NULL))).append(".txt").c_str());
	logFile_ << "Simulation Started - " << NLS::timeString(time(NULL)) << std::endl;
	logFile_ << std::endl;
	logFile_ << "SETTINGS" << std::endl;
	logFile_ << "Grid Size: " << grid << std::endl
		<< "Amount of food: " << plant << std::endl
		<< "Number of Obstacles: " << obs << std::endl
		<< "Number of Prey: " << prey << std::endl
		<< "Amount of Predators: " << pred << std::endl
		<< "Smart AI: " << PreyPreferences.smartAI << std::endl << std::endl;

}

Environment::~Environment()
{
	logFile_ << std::endl << "Simulation Over." << std::endl;
	logFile_ << "Prey Remaining: " << preyXY_.size() << std::endl;
	logFile_ << "Predators Remaining: " << predXY_.size() << std::endl;
	logFile_ << "Total Iterations: " << currentIteration_ << std::endl;
	logFile_.close();

	for (unsigned int idx = 0; idx < grid_.size(); ++idx)
	{
		grid_[idx].clearSegment();
	}
}
void Environment::drawSimulation(sf::RenderWindow& window)
{
	auto windowSize = window.getView().getSize();

	double segmentPixelsW = (windowSize.x - WIDTHBUFFER) / (gridSize_); // calculate the width of the segment in pixels
	double segmentPixelsH = (windowSize.y - HEIGHTBUFFER) / (gridSize_);// calculate the height of segement in pixel

	auto textSize = 15;
	auto barBuffer = 20;
	auto screenPosition = sf::Vector2f(0.0f, 0.0f);
	auto screenSize = sf::Vector2f(windowSize.x - (WIDTHBUFFER - 2), windowSize.y - (HEIGHTBUFFER - 2));

	this->gfx->DrawRectangle(screenPosition, screenSize, sf::Color::Black);

	auto healthBarX = windowSize.x - 250;
	auto ySideBar = 5.0f;
	auto entityTextPosition = sf::Vector2f(healthBarX, ySideBar);

	//HEALTH BARS
	this->gfx->DrawText(entityTextPosition, sf::Color::Blue, textSize, "Prey: " + std::to_string(getEntities().first));

	for (unsigned int idx = 0; idx < preyXY_.size(); ++idx)
	{
		ySideBar += barBuffer;

		auto idPosition = sf::Vector2f(healthBarX, ySideBar);
		auto healthPosition = sf::Vector2f(healthBarX + 20, ySideBar);
		auto healthSize = sf::Vector2f(float(grid_[(gridSize_ * preyXY_[idx].first) + preyXY_[idx].second].getPrey()->getHealth()), textSize);

		this->gfx->DrawText(
			idPosition,
			sf::Color::Blue,
			textSize,
			std::to_string(grid_[(gridSize_ * preyXY_[idx].first) + preyXY_[idx].second].getPrey()->getIdenifier()));

		this->gfx->DrawRectangle(
			healthPosition,
			healthSize,
			grid_[(gridSize_ * preyXY_[idx].first) + preyXY_[idx].second].getPrey()->getHealth() < 20 ? sf::Color::Red : sf::Color(34, 139, 34));

		this->gfx->DrawText(
			healthPosition,
			sf::Color::White,
			textSize,
			grid_[(gridSize_ * preyXY_[idx].first) + preyXY_[idx].second].getMessage());

	}

	ySideBar += barBuffer * 2;
	entityTextPosition = sf::Vector2f(healthBarX, ySideBar);

	this->gfx->DrawText(
		entityTextPosition,
		sf::Color::Blue,
		textSize,
		"Predators: " + std::to_string(getEntities().second));

	for (unsigned int idx = 0; idx < predXY_.size(); ++idx)
	{
		ySideBar += barBuffer;

		auto idPosition = sf::Vector2f(healthBarX, ySideBar);
		auto healthPosition = sf::Vector2f(healthBarX + 20, ySideBar);
		auto healthSize = sf::Vector2f(float(grid_[(gridSize_ * predXY_[idx].first) + predXY_[idx].second].getPredator()->getHealth()), textSize);

		this->gfx->DrawText(
			idPosition,
			sf::Color::White,
			textSize,
			std::to_string(grid_[(gridSize_ * predXY_[idx].first) + predXY_[idx].second].getPredator()->getIdenifier()));

		this->gfx->DrawRectangle(
			healthPosition,
			healthSize,
			grid_[(gridSize_ * predXY_[idx].first) + predXY_[idx].second].getPredator()->getHealth() < 20 ? sf::Color::Red : sf::Color(34, 139, 34));
	}

	//ENVIRONMENT
	auto scale = 3.0f;

	std::string texture;
	bool shouldDrawId;
	unsigned int id;
	sf::Color color;

	for (int idx = 0; idx < gridSize_; idx++)		//cycle through locations
	{
		for (int jdx = 0; jdx < gridSize_; jdx++)	//in the Grid
		{
			shouldDrawId = false;
			color = sf::Color::Transparent;
			id = 0;

			if (grid_[(gridSize_ * idx) + jdx].getPredator() != NULL) //if !NULL draw Predator
			{
				texture = "pred";
				shouldDrawId = true;
				color = sf::Color::Red;
				id = grid_[(gridSize_ * idx) + jdx].getPredator()->getIdenifier();

			}
			else if (grid_[(gridSize_ * idx) + jdx].getPrey() != NULL)	//if !NULL draw Prey
			{
				texture = "prey";
				shouldDrawId = true;
				color = sf::Color::Blue;
				id = grid_[(gridSize_ * idx) + jdx].getPrey()->getIdenifier();
			}
			else if (grid_[(gridSize_ * idx) + jdx].getObstacle() != NULL) //if !NULL draw obstacle
			{
				texture = "rock";
			}
			else if (grid_[(gridSize_ * idx) + jdx].getFood() != NULL)	//if !NULL draw Food
			{

				texture = "berry";
			}
			else
			{
				texture = "grass";
			}

			auto spritePosition = sf::Vector2f(idx * int(segmentPixelsW), jdx * int(segmentPixelsH));
			auto idPosition = sf::Vector2f((idx - 1) * int(segmentPixelsW) + 8, jdx * int(segmentPixelsH));

			this->gfx->DrawSprite(spritePosition, texture, scale);

			if (shouldDrawId) {
				this->gfx->DrawText(idPosition, color, 10, std::to_string(id));
			}
		}
	}
}

void Environment::populateGrid(int numOfPrey, AIPreferences& PreyPreferences, int numOfPred, AIPreferences& PredPreferences)
{
	int gridY,
		gridX;
	std::pair<int, int> tempXY;

	for (int idx = 0; idx < numOfPrey; idx++)
	{
		do {
			gridY = rand() % gridSize_;
			gridX = rand() % (gridSize_ - gridY);
		} while ((grid_[(gridSize_ * gridX) + gridY].getPrey() != NULL) || (grid_[(gridSize_ * gridX) + gridY].getObstacle() != NULL));

		Prey* newPrey = new Prey(PreyPreferences, idx + 1);
		grid_[(gridSize_ * gridX) + gridY].setPrey(newPrey);

		tempXY.first = gridX;
		tempXY.second = gridY;

		preyXY_.push_back(tempXY);
	}
	for (int idx = 0; idx < numOfPred; idx++)
	{
		do {
			gridY = rand() % gridSize_;
			gridX = (gridY == 0) ? gridSize_ - 1 : (rand() % (gridY)) + (gridSize_ - gridY);
		} while ((grid_[(gridSize_ * gridX) + gridY].getPredator() != NULL) || (grid_[(gridSize_ * gridX) + gridY].getObstacle() != NULL));

		Predator* newPred = new Predator(PredPreferences, idx + 1);
		grid_[(gridSize_ * gridX) + gridY].setPredator(newPred);

		tempXY.first = gridX;
		tempXY.second = gridY;

		predXY_.push_back(tempXY);
	}
}

void Environment::iterateSimulation(int gameCount)
{
	currentIteration_ = gameCount;
	_pushSubScope();
	_manageFood(gameCount);
}

void Environment::_manageFood(int gameCount)
{
	int plantIdx = gameCount % plants_;
	int tempX = foodXY_[plantIdx].first;
	int tempY = foodXY_[plantIdx].second;

	if (grid_[(gridSize_ * tempX) + tempY].getFood()->getTIP() < (gameCount - 30) && (0 == rand() % 5))
	{
		do
		{
			tempX = rand() % gridSize_;
			tempY = rand() % gridSize_;
		} while ((grid_[(gridSize_ * (tempX)) + tempY].getFood() != NULL) || (grid_[(gridSize_ * (tempX)) + tempY].getObstacle() != NULL));

		grid_[(gridSize_ * (tempX)) + tempY].setFood(grid_[(gridSize_ * foodXY_[plantIdx].first) + foodXY_[plantIdx].second].getFood());
		grid_[(gridSize_ * foodXY_[plantIdx].first) + foodXY_[plantIdx].second].setFood(NULL);
		grid_[(gridSize_ * tempX) + tempY].getFood()->setTIP(gameCount);

		foodXY_[plantIdx].first = tempX;
		foodXY_[plantIdx].second = tempY;

	}
}
void Environment::_pushSubScope(void)
{
	int aiX, aiY;
	std::pair<unsigned int, std::string> dirMsg;

	for (unsigned int idx = 0; idx < preyXY_.size(); idx++)	//in the Grid
	{
		aiX = preyXY_[idx].first;
		aiY = preyXY_[idx].second;

		if (grid_[(gridSize_ * aiX) + aiY].getPrey() != NULL)					//if there is a Prey, calculate its next move
		{
			grid_[(gridSize_ * aiX) + aiY].setSubScope(_AIScopeSubArray(aiX, aiY, grid_[(gridSize_ * aiX) + aiY].getPrey()->getScope()));
			dirMsg = grid_[(gridSize_ * aiX) + aiY].convertScopeToInt(grid_[(gridSize_ * aiX) + aiY].getPrey());
			grid_[(gridSize_ * aiX) + aiY].clearSubScope();

			switch (dirMsg.first)
			{
			case DEATH:
				logFile_ << "Prey " << grid_[(gridSize_ * aiX) + aiY].getPrey()->getIdenifier() << " died at iteration " << currentIteration_ + 1 << std::endl;
				logFile_ << "last message was: \"" << dirMsg.second << "\"" << std::endl << std::endl;
				delete grid_[(gridSize_ * aiX) + aiY].getPrey();
				grid_[(gridSize_ * aiX) + aiY].setPrey(NULL);
				preyXY_.erase(preyXY_.begin() + idx);
				break;
			case NORTH:
				grid_[(gridSize_ * aiX) + (aiY - 1)].setPrey(grid_[(gridSize_ * aiX) + aiY].getPrey());
				grid_[(gridSize_ * aiX) + aiY].setPrey(NULL);
				grid_[(gridSize_ * aiX) + (aiY - 1)].setMessage(dirMsg.second);
				grid_[(gridSize_ * aiX) + aiY].setMessage("");
				preyXY_[idx].second--;
				break;
			case EAST:
				grid_[(gridSize_ * (aiX + 1)) + aiY].setPrey(grid_[(gridSize_ * aiX) + aiY].getPrey());
				grid_[(gridSize_ * aiX) + aiY].setPrey(NULL);
				grid_[(gridSize_ * (aiX + 1)) + aiY].setMessage(dirMsg.second);
				grid_[(gridSize_ * aiX) + aiY].setMessage("");
				preyXY_[idx].first++;
				break;
			case SOUTH:
				grid_[(gridSize_ * aiX) + (aiY + 1)].setPrey(grid_[(gridSize_ * aiX) + aiY].getPrey());
				grid_[(gridSize_ * aiX) + aiY].setPrey(NULL);
				grid_[(gridSize_ * aiX) + (aiY + 1)].setMessage(dirMsg.second);
				grid_[(gridSize_ * aiX) + aiY].setMessage("");
				preyXY_[idx].second++;
				break;
			case WEST:
				grid_[(gridSize_ * (aiX - 1)) + aiY].setPrey(grid_[(gridSize_ * aiX) + aiY].getPrey());
				grid_[(gridSize_ * aiX) + aiY].setPrey(NULL);
				grid_[(gridSize_ * (aiX - 1)) + aiY].setMessage(dirMsg.second);
				grid_[(gridSize_ * aiX) + aiY].setMessage("");
				preyXY_[idx].first--;
				break;
			default:
				//no move
				break;
			}
		}
	}

	for (unsigned int idx = 0; idx < predXY_.size(); idx++)	//in the Grid
	{
		aiX = predXY_[idx].first;
		aiY = predXY_[idx].second;

		if (grid_[(gridSize_ * aiX) + aiY].getPredator() != NULL)
		{
			grid_[(gridSize_ * aiX) + aiY].setSubScope(_AIScopeSubArray(aiX, aiY, grid_[(gridSize_ * aiX) + aiY].getPredator()->getScope()));
			dirMsg = grid_[(gridSize_ * aiX) + aiY].convertScopeToInt(grid_[(gridSize_ * aiX) + aiY].getPredator());
			grid_[(gridSize_ * aiX) + aiY].clearSubScope();

			switch (dirMsg.first)
			{
			case DEATH:
				logFile_ << "Predator " << grid_[(gridSize_ * aiX) + aiY].getPredator()->getIdenifier() << " died at iteration " << currentIteration_ + 1 << std::endl << std::endl;
				delete grid_[(gridSize_ * aiX) + aiY].getPredator();
				grid_[(gridSize_ * aiX) + aiY].setPredator(NULL);
				predXY_.erase(predXY_.begin() + idx);
				break;
			case NORTH:
				grid_[(gridSize_ * aiX) + (aiY - 1)].setPredator(grid_[(gridSize_ * aiX) + aiY].getPredator());
				grid_[(gridSize_ * aiX) + aiY].setPredator(NULL);
				predXY_[idx].second--;
				break;
			case EAST:
				grid_[(gridSize_ * (aiX + 1)) + aiY].setPredator(grid_[(gridSize_ * aiX) + aiY].getPredator());
				grid_[(gridSize_ * aiX) + aiY].setPredator(NULL);
				predXY_[idx].first++;
				break;
			case SOUTH:
				grid_[(gridSize_ * aiX) + (aiY + 1)].setPredator(grid_[(gridSize_ * aiX) + aiY].getPredator());
				grid_[(gridSize_ * aiX) + aiY].setPredator(NULL);
				predXY_[idx].second++;
				break;
			case WEST:
				grid_[(gridSize_ * (aiX - 1)) + aiY].setPredator(grid_[(gridSize_ * aiX) + aiY].getPredator());
				grid_[(gridSize_ * aiX) + aiY].setPredator(NULL);
				predXY_[idx].first--;
				break;
			default:
				//no move
				break;
			}
		}
	}
}

std::vector<Segment> Environment::_AIScopeSubArray(int xPos, int yPos, int AIScope)
{
	std::vector<Segment> subScope;
	int scopeRad = AIScope / 2;

	subScope.resize(NLS::pwr2(AIScope));

	for (int idx = 0; idx < AIScope; idx++)
		for (int jdx = 0; jdx < AIScope; jdx++)
		{
			if ((idx + (xPos - scopeRad) >= 0) && (idx + (xPos - scopeRad) < gridSize_) && (jdx + (yPos - scopeRad) >= 0) && (jdx + (yPos - scopeRad) < gridSize_))
			{
				subScope[(AIScope * idx) + jdx] = grid_[(gridSize_ * (idx + (xPos - scopeRad))) + (jdx + (yPos - scopeRad))];
			}
			else
			{
				subScope[(AIScope * idx) + jdx].setOOB(true);
			}
		}
	return subScope;
}

std::pair<unsigned int, unsigned int> Environment::getEntities()
{
	return(std::pair<unsigned int, unsigned int>(preyXY_.size(), predXY_.size()));
}
