#include <memory>
#include <chrono>
#include <thread>
#include <SFML/Graphics.hpp>

#include "environment.h"
#include "GUI.h"
#include "util/TextureAtlas.h"
#include "util/SFMLGraphics.h"

int main()
{
	auto window = std::make_shared<sf::RenderWindow>(
		sf::VideoMode(1200, 1000),
		"Yuatja",
		sf::Style::Titlebar | sf::Style::Close);


	auto atlas = std::make_shared<TextureAtlas>();
	atlas
		->AddTexture("background", "assets/GUI/Background.png")
		->AddTexture("begin", "assets/GUI/BeginButton.png")
		->AddTexture("options", "assets/GUI/OptionsButton.png")
		->AddTexture("credits", "assets/GUI/CreditsButton.png")
		->AddTexture("mainTitle", "assets/GUI/MainTitle.png")
		->AddTexture("save", "assets/GUI/SaveButton.png")
		->AddTexture("menu", "assets/GUI/ResetButton.png")
		->AddTexture("exit", "assets/GUI/ExitButton.png")
		->AddTexture("table", "assets/GUI/Table.png")
		->AddTexture("back", "assets/GUI/BackButton.png")
		->AddTexture("prey", "assets/Sprites/prey.png")
		->AddTexture("pred", "assets/Sprites/pred.png")
		->AddTexture("berry", "assets/Sprites/berry.png")
		->AddTexture("rock", "assets/Sprites/rock.png");

	auto gfx = std::make_shared<SFMLGraphics>(*window, atlas);

	GUIsim menu(gfx);
	Environment* level = nullptr;

	//initialise in order:	OOB, empty, prdPry, pryFod, pred, prey, food, energy, moveThreshold, scope, foodGain, conflict, intelligence
	AIPreferences PreyPref = { 15, 5,	20,		100,	200,  50,	100,  1000,	  50,			 7,		20,		  -150 };		//structures for defining how AI behave
	AIPreferences PredPref = { 15, 5,	150,	300,	0,   350,	5,    3300,	  50,			 11,	0,		   200 };		//means only one calcMove function needed
	unsigned long iterationCount = 0;

	sf::Text text;
	text.setPosition(10.0f, 10.0f);
	text.setFillColor(sf::Color::White);

	while (window->isOpen() && !menu.simQuit)
	{
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window->close();
			}
		}

		window->clear();
		if (!menu.gameStarted) {
			iterationCount = 0;
			menu.drawMenu(*window);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				auto position = sf::Mouse::getPosition(*window);
				menu.mouseEvent(position.x, position.y, *window);
			}

			if (menu.simEnd || !level) {
				PreyPref.smartAI = menu.getSettings().intelligence; //set AI Toggle
				level = new Environment(menu.getSettings().gridSize, menu.getSettings().numOfFood, menu.getSettings().numOfObstacles, menu.getSettings().numOfPrey, menu.getSettings().numOfPred, PreyPref, PredPref, *window);
				menu.simEnd = false;
			}
		}

		if (menu.gameStarted) {
			if (level)
			{
				if (!menu.isComplete(level->getEntities(), iterationCount))
				{
					level->iterateSimulation(iterationCount++);
					level->drawSimulation(*window);
					menu.legend(*window);

					auto sleepTime = menu.statusBar(*window);
					std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));

					text.setString("Iteration: " + std::to_string(iterationCount) + " / " + std::to_string(menu.getSettings().simTimeOut * 100));
					window->draw(text);
				}
				else if (!menu.simEnd)
				{
					menu.endGameScreen(*window, level->getEntities(), iterationCount);
				}
			}
		}
		window->display();
	}
	return 0;
}