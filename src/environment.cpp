// Filename: environment.cpp
// Author: Alex Rudd
// ID: N0321342
// Version: 1.0
// Date: 30/11/2011
// Description: function library for the Environment class
#include "environment.h"
#include "artificial intelligence.h"

Environment::Environment(int grid, int plant, int obs, int prey, int pred, AIPreferences& PreyPreferences, AIPreferences& PredPreferences, sf::RenderTarget& window)
{
	int xTemp, yTemp;
	srand(unsigned int(time(NULL)));

	gridSize_ = grid;
	plants_ = plant;
	obstacles_ = obs;


	//resize environment grid
	grid_.resize(NLS::pwr2(gridSize_));

	//construct objects
	for(int idx = 0; idx < obstacles_; idx++)
	{
		do
		{
			xTemp = rand()%gridSize_;
			yTemp = rand()%gridSize_;
		}
		while(grid_[(gridSize_ * (xTemp)) + yTemp].getObstacle() != NULL);
		
		grid_[(gridSize_ * (xTemp)) + yTemp].setObstacle(new Obstacle);
	}

	//construct food
	foodXY_.resize(plants_);
	for(int idx = 0; idx < plants_; ++idx)
	{
		do
		{
			xTemp = rand()%gridSize_;
			yTemp = rand()%gridSize_;
		}
		while(grid_[(gridSize_ * (xTemp)) + yTemp].getFood() != NULL || (grid_[(gridSize_ * (xTemp)) + yTemp].getObstacle() != NULL));

		grid_[(gridSize_ * (xTemp)) + yTemp].setFood(new Food);
		grid_[(gridSize_ * (xTemp)) + yTemp].getFood()->setTIP(rand()%30);

		foodXY_[idx].first = xTemp;
		foodXY_[idx].second = yTemp;
	}

	populateGrid(prey, PreyPreferences, pred, PredPreferences);


	_Grass->loadFromFile("assets/Sprites/8bit grass2.png");
	_Berry->loadFromFile("assets/Sprites/8bit berry.png");
	_Rock->loadFromFile("assets/Sprites/8bit rock.png");
	_Prey->loadFromFile("assets/Sprites/8bit prey.png");
	_Pred->loadFromFile("assets/Sprites/8bit pred.png");

	//_Grass->resize(imageWidth, imageHeight);
	//_Berry->resize(imageWidth, imageHeight);
	//_Rock->resize(imageWidth, imageHeight);
	//_Prey->resize(imageWidth, imageHeight);
	//_Pred->resize(imageWidth, imageHeight);

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
void Environment::drawSimulation(sf::RenderTarget& window)
{
	double segmentPixelsW = (window.getView().getSize().x - WIDTHBUFFER)/(gridSize_); // calculate the width of the segment in pixels
	double segmentPixelsH = (window.getView().getSize().y - HEIGHTBUFFER)/(gridSize_);// calculate the height of segement in pixel
														//now allowing for a 50 pixel stats bar at the bottom
	sf::RectangleShape rectangle;
	rectangle.setFillColor(sf::Color::Black);
	rectangle.setPosition(0, 0);
	rectangle.setSize(sf::Vector2f(
		window.getView().getSize().x - (WIDTHBUFFER - 2),
		window.getView().getSize().y - (HEIGHTBUFFER - 2)));
	window.draw(rectangle);

	//HEALTH BARS
	float ySideBar = 5.0f;
	sf::Text preyTotalText;
	preyTotalText.setPosition(window.getView().getSize().x - 90, ySideBar);
	preyTotalText.setFillColor(sf::Color::Blue);
	preyTotalText.setString("Prey: " + std::to_string(getEntities().first));
	window.draw(preyTotalText);
	ySideBar+=15.0f;

	for(unsigned int idx = 0; idx < preyXY_.size(); ++idx)
	{
		sf::Text nameText;
		nameText.setPosition(window.getView().getSize().x - 95, ySideBar);
		nameText.setFillColor(sf::Color::White);
		nameText.setString(std::to_string(grid_[(gridSize_ * preyXY_[idx].first) + preyXY_[idx].second].getPrey()->getIdenifier()));
		window.draw(nameText);

		sf::RectangleShape healthBar;
		if (grid_[(gridSize_ * preyXY_[idx].first) + preyXY_[idx].second].getPrey()->getHealth() < 20) 
		{
			healthBar.setFillColor(sf::Color::Red);
		}
		else
		{
			healthBar.setFillColor(sf::Color::Green);
		}
		auto healthBarX = window.getView().getSize().x - 80;
		healthBar.setPosition(healthBarX, ySideBar);
		healthBar.setSize(sf::Vector2f(
			float(healthBarX + ((75.0 / 100.0) * grid_[(gridSize_ * preyXY_[idx].first) + preyXY_[idx].second].getPrey()->getHealth())),
			ySideBar + 10));
		window.draw(healthBar);

		sf::RectangleShape healthBarBox;
		healthBarBox.setPosition(healthBarX, ySideBar);
		healthBarBox.setSize(sf::Vector2f(window.getView().getSize().x - 5, ySideBar + 10));
		healthBarBox.setOutlineColor(sf::Color::Black);
		window.draw(healthBarBox);

		sf::Text messageText;
		messageText.setPosition(window.getView().getSize().x - 75, ySideBar);
		messageText.setFillColor(sf::Color::White);
		messageText.setString(grid_[(gridSize_ * preyXY_[idx].first) + preyXY_[idx].second].getMessage());
		window.draw(messageText);

		ySideBar+=15;
	}

	ySideBar = 5.0f;
	sf::Text predatorTotalText;
	predatorTotalText.setPosition(window.getView().getSize().x - 190, ySideBar);
	predatorTotalText.setFillColor(sf::Color::Blue);
	predatorTotalText.setString("Predators: " + std::to_string(getEntities().second));
	window.draw(predatorTotalText);
	ySideBar+=15;

	for(unsigned int idx = 0; idx < predXY_.size(); ++idx)
	{
		sf::Text nameText;
		nameText.setPosition(window.getView().getSize().x - 195, ySideBar);
		nameText.setFillColor(sf::Color::White);
		nameText.setString(std::to_string(grid_[(gridSize_ * predXY_[idx].first) + predXY_[idx].second].getPredator()->getIdenifier()));
		window.draw(nameText);

		sf::RectangleShape healthBar;
		if (grid_[(gridSize_ * predXY_[idx].first) + predXY_[idx].second].getPredator()->getHealth() < 20)
		{
			healthBar.setFillColor(sf::Color::Red);
		}
		else
		{
			healthBar.setFillColor(sf::Color::Green);
		}
		auto healthBarX = window.getView().getSize().x - 180;
		healthBar.setPosition(healthBarX, ySideBar);
		healthBar.setSize(sf::Vector2f(
			float(healthBarX + ((75.0 / 100.0) * grid_[(gridSize_ * predXY_[idx].first) + predXY_[idx].second].getPredator()->getHealth())),
			ySideBar + 10));
		window.draw(healthBar);


		sf::RectangleShape healthBarBox;
		healthBarBox.setPosition(healthBarX, ySideBar);
		healthBarBox.setSize(sf::Vector2f(window.getView().getSize().x - 105, ySideBar + 10));
		healthBarBox.setOutlineColor(sf::Color::Black);
		window.draw(healthBarBox);

		ySideBar+=15;
	}

	//ENVIRONMENT
	unsigned int imageWidth = (window.getView().getSize().x - WIDTHBUFFER) / (gridSize_); // calculate the width of the segment in pixels
	unsigned int imageHeight = (window.getView().getSize().y - HEIGHTBUFFER) / (gridSize_);// calculate the height of segement in pixel
	for(int idx = 0; idx < gridSize_; idx++)		//cycle through locations
		for(int jdx = 0; jdx < gridSize_; jdx++)	//in the Grid
		{

			if(grid_[(gridSize_ * idx) + jdx].getObstacle() != NULL) //if !NULL draw obstacle
			{
				sf::Sprite sprite;
				sprite.setTexture(*_Rock);
				sprite.setColor(sf::Color(255, 255, 255, 200));
				sprite.setPosition(idx * int(segmentPixelsW), jdx * int(segmentPixelsH));
				window.draw(sprite);
			}
			else
			{
				if(grid_[(gridSize_ * idx) + jdx].getPredator() != NULL) //if !NULL draw Predator
				{
					sf::Sprite sprite;
					sprite.setTexture(*_Pred);
					sprite.setColor(sf::Color(255, 255, 255, 200));
					sprite.setPosition(idx * int(segmentPixelsW), jdx * int(segmentPixelsH));
					window.draw(sprite);

					sf::Text nameText;
					nameText.setPosition((idx - 1)* int(segmentPixelsW) + 8, jdx* int(segmentPixelsH));
					nameText.setFillColor(sf::Color::White);
					nameText.setString(std::to_string(grid_[(gridSize_ * idx) + jdx].getPredator()->getIdenifier()));
					window.draw(nameText);
				}
				else if(grid_[(gridSize_ * idx) + jdx].getPrey() != NULL)	//if !NULL draw Prey
				{
					sf::Sprite sprite;
					sprite.setTexture(*_Prey);
					sprite.setColor(sf::Color(255, 255, 255, 200));
					sprite.setPosition(idx* int(segmentPixelsW), jdx* int(segmentPixelsH));
					window.draw(sprite);

					sf::Text nameText;
					nameText.setPosition((idx - 1) * int(segmentPixelsW) + 8, jdx * int(segmentPixelsH));
					nameText.setFillColor(sf::Color::White);
					nameText.setString(std::to_string(grid_[(gridSize_ * idx) + jdx].getPrey()->getIdenifier()));
					window.draw(nameText);
				}
				else if(grid_[(gridSize_ * idx) + jdx].getFood() != NULL)	//if !NULL draw Food
				{
					sf::Sprite sprite;
					sprite.setTexture(*_Berry);
					sprite.setColor(sf::Color(255, 255, 255, 200));
					sprite.setPosition(idx* int(segmentPixelsW), jdx* int(segmentPixelsH));
					window.draw(sprite);
				}
				else
				{
					sf::Sprite sprite;
					sprite.setTexture(*_Grass);
					sprite.setColor(sf::Color(255, 255, 255, 200));
					sprite.setPosition(idx* int(segmentPixelsW), jdx* int(segmentPixelsH));
					window.draw(sprite);
				}
			}
		}
}

void Environment::populateGrid(int numOfPrey, AIPreferences& PreyPreferences, int numOfPred, AIPreferences& PredPreferences)
{
	int gridY,
		gridX;
	std::pair<int, int> tempXY;

	for(int idx = 0; idx < numOfPrey; idx++)
	{
		do{
			gridY = rand()%gridSize_;
			gridX = rand()%(gridSize_-gridY);
		}while((grid_[(gridSize_ * gridX) + gridY].getPrey() != NULL) || (grid_[(gridSize_ * gridX) + gridY].getObstacle() != NULL));

		Prey * newPrey = new Prey(PreyPreferences, idx+1);
		grid_[(gridSize_ * gridX) + gridY].setPrey(newPrey);
		
		tempXY.first = gridX;
		tempXY.second = gridY;

		preyXY_.push_back(tempXY);
	}
	for(int idx = 0; idx < numOfPred; idx++)
	{
		do{
			gridY = rand()%gridSize_;
			gridX = (gridY==0) ? gridSize_-1 : (rand()%(gridY))+(gridSize_-gridY);
		}while((grid_[(gridSize_ * gridX) + gridY].getPredator() != NULL) || (grid_[(gridSize_ * gridX) + gridY].getObstacle() != NULL));

		Predator *newPred = new Predator(PredPreferences, idx+1);
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
	int plantIdx = gameCount%plants_;
	int tempX = foodXY_[plantIdx].first;
	int tempY = foodXY_[plantIdx].second;

	if(grid_[(gridSize_ * tempX) + tempY].getFood()->getTIP() < (gameCount - 30) && (0 == rand()%5))
	{
		do
		{
			tempX = rand()%gridSize_;
			tempY = rand()%gridSize_;
		}
		while((grid_[(gridSize_ * (tempX)) + tempY].getFood() != NULL) || (grid_[(gridSize_ * (tempX)) + tempY].getObstacle() != NULL));

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

	for(unsigned int idx = 0; idx < preyXY_.size(); idx++)	//in the Grid
	{
		aiX = preyXY_[idx].first;
		aiY = preyXY_[idx].second;

		if(grid_[(gridSize_ * aiX) + aiY].getPrey() != NULL)					//if there is a Prey, calculate its next move
		{
			grid_[(gridSize_ * aiX) + aiY].setSubScope(_AIScopeSubArray(aiX, aiY, grid_[(gridSize_ * aiX) + aiY].getPrey()->getScope()));
			dirMsg = grid_[(gridSize_ * aiX) + aiY].convertScopeToInt(grid_[(gridSize_ * aiX) + aiY].getPrey());
			grid_[(gridSize_ * aiX) + aiY].clearSubScope();

			switch(dirMsg.first)
			{
			case DEATH:
				logFile_ << "Prey " << grid_[(gridSize_ * aiX) + aiY].getPrey()->getIdenifier() << " died at iteration " << currentIteration_+1 << std::endl;
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

	for(unsigned int idx = 0; idx < predXY_.size(); idx++)	//in the Grid
	{
		aiX = predXY_[idx].first;
		aiY = predXY_[idx].second;

		if(grid_[(gridSize_ * aiX) + aiY].getPredator() != NULL)
		{
			grid_[(gridSize_ * aiX) + aiY].setSubScope(_AIScopeSubArray(aiX, aiY, grid_[(gridSize_ * aiX) + aiY].getPredator()->getScope()));
			dirMsg = grid_[(gridSize_ * aiX) + aiY].convertScopeToInt(grid_[(gridSize_ * aiX) + aiY].getPredator());
			grid_[(gridSize_ * aiX) + aiY].clearSubScope();

			switch(dirMsg.first)
			{
			case DEATH:
				logFile_ << "Predator " << grid_[(gridSize_ * aiX) + aiY].getPredator()->getIdenifier() << " died at iteration " << currentIteration_+1 << std::endl << std::endl;
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
	int scopeRad = AIScope/2;

	subScope.resize(NLS::pwr2(AIScope));

	for(int idx = 0; idx < AIScope; idx++)
		for(int jdx = 0; jdx < AIScope; jdx++)
		{
			if((idx+(xPos-scopeRad) >= 0) && (idx+(xPos-scopeRad) < gridSize_) && (jdx+(yPos-scopeRad) >= 0) && (jdx+(yPos-scopeRad) < gridSize_))
			{
				subScope[(AIScope * idx) + jdx] = grid_[(gridSize_ * (idx+(xPos-scopeRad))) + (jdx+(yPos-scopeRad))];
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
