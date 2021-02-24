#include <memory>
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>

#include "environment.h"
#include "GUI.h"

int main()
{
	auto window = std::make_shared<sf::RenderWindow>(
		sf::VideoMode(800, 700),
		"Yuatja",
		sf::Style::Titlebar | sf::Style::Close);

	GUIsim menu;
	Environment* level;

	//initialise in order:	OOB, empty, prdPry, pryFod, pred, prey, food, energy, moveThreshold, scope, foodGain, conflict, intelligence
	AIPreferences PreyPref = {15, 5,	20,		100,	200,  50,	100,  1000,	  50,			 7,		20,		  -150};		//structures for defining how AI behave
	AIPreferences PredPref = {15, 5,	150,	300,	0,   350,	5,    3300,	  50,			 11,	0,		   200};		//means only one calcMove function needed
	unsigned long iterationCount = 0;

	while(!menu.simQuit)
	{
		while(!menu.gameStarted)
		{
			window->clear();
			menu.drawMenu(*window);

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				auto position = sf::Mouse::getPosition();
				menu.mouseEvent(position.x, position.y, *window);
			}
				
			window->display();
		}

		PreyPref.smartAI = menu.getSettings().intelligence; //set AI Toggle
		level = new Environment(menu.getSettings().gridSize, menu.getSettings().numOfFood, menu.getSettings().numOfObstacles, menu.getSettings().numOfPrey, menu.getSettings().numOfPred, PreyPref, PredPref, *window);
		iterationCount = 0;

		sf::Text text;
		text.setPosition(10.0f, 10.0f);
		text.setFillColor(sf::Color::White);

		while(!menu.isComplete(level->getEntities(), iterationCount))
		{
			window->clear(sf::Color::Black);

			level->iterateSimulation(iterationCount++);
			level->drawSimulation(*window);
			menu.legend(*window);

			auto sleepTime = menu.statusBar(*window);
			std::this_thread::sleep_for(std::chrono::seconds(sleepTime));

			text.setString("Iteration: " + std::to_string(iterationCount) + " / " + std::to_string(menu.getSettings().simTimeOut * 100));
			window->draw(text);

			window->display();
		}

		while(!menu.simEnd)
		{
			window->clear();
			menu.endGameScreen(*window, level->getEntities(), iterationCount);
			window->display();
		}
		delete level;
	}
	return 0;
}