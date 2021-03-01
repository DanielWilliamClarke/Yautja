#include "GUI.h"

#include "util/IGraphics.h"

GUIsim::GUIsim(std::shared_ptr<IGraphics> gfx)
	: gfx(gfx)
{
	currentScreen_ = mainScreen;
	simSettings defaultSettings = { 30, 50, 5, 20, 50, 20, false };
	simulationSettings_ = defaultSettings;
	gameStarted = false;
	simEnd = false;
	simQuit = false;
	sleepSliderV_ = 300;

	teamAndRoles_.push_back(std::pair<std::string, std::string>("Aiden Gallagher", "Team Leader, Researcher"));
	teamAndRoles_.push_back(std::pair<std::string, std::string>("Alex Rudd", "Programmer"));
	teamAndRoles_.push_back(std::pair<std::string, std::string>("Daniel Clarke", "Graphics Designer, Programmer"));
	teamAndRoles_.push_back(std::pair<std::string, std::string>("Emma Hall", "Researcher"));
	teamAndRoles_.push_back(std::pair<std::string, std::string>("Lewis Kell", "Deputy Leader"));
	teamAndRoles_.push_back(std::pair<std::string, std::string>("Michael Percivals", "Programmer"));
	teamAndRoles_.push_back(std::pair<std::string, std::string>("Xiaorong Li", "Researcher"));
}

unsigned int GUIsim::statusBar(sf::RenderWindow& window)
{
	auto windowSize = window.getView().getSize();
	auto position = sf::Mouse::getPosition(window);
	auto barPosition = sf::Vector2f(windowSize.x / 2 - 200, windowSize.y - 80);
	auto barSize = sf::Vector2f(400, 20);
	auto barColor = sf::Color(34, 34, 34);
	auto sliderPosition = sf::Vector2f((windowSize.x / 2 - 200) + sleepSliderV_, windowSize.y - 80);
	auto sliderSize = sf::Vector2f(20, 20);
	auto textPosition = sf::Vector2f(windowSize.x / 2 - 200, windowSize.y - 90);

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
		(position.y > windowSize.y - 80) && (position.y < windowSize.y - 60) &&
		(position.x > (windowSize.x / 2) - 200) &&
		(position.x < (windowSize.x / 2) + 200))
	{
		sleepSliderV_ = position.x - ((windowSize.x / 2) - 200);
	}

	this->gfx->DrawRectangle(barPosition, barSize, barColor);
	this->gfx->DrawRectangle(sliderPosition, sliderSize, sf::Color::Green);

	this->gfx->DrawText(textPosition, sf::Color::White, 12, "Speed: " + std::to_string(sleepSliderV_));

	return 400 - sleepSliderV_;
}

void GUIsim::legend(sf::RenderWindow& window)
{
	auto windowSize = window.getView().getSize();
	auto scale = 3.0f;

	auto preyPosition = sf::Vector2f(10, windowSize.y - 30);
	auto preyTextPosition = sf::Vector2f(50, windowSize.y - 30);
	auto predPosition = sf::Vector2f(windowSize.x / 4, windowSize.y - 30);
	auto predTextPosition = sf::Vector2f((windowSize.x / 4) + 50, windowSize.y - 30);
	auto foodPosition = sf::Vector2f((windowSize.x / 2), windowSize.y - 30);
	auto foodTextPosition = sf::Vector2f((windowSize.x / 2) + 50, windowSize.y - 30);
	auto rockPosition = sf::Vector2f((windowSize.x * 0.75), windowSize.y - 30);
	auto rockTextPosition = sf::Vector2f((windowSize.x * 0.75) + 50, windowSize.y - 30);

	this->gfx->DrawSprite(preyPosition, std::string("prey"), scale);
	this->gfx->DrawText(preyTextPosition, sf::Color::White, 10, std::string("Prey"));

	this->gfx->DrawSprite(predPosition, std::string("pred"), scale);
	this->gfx->DrawText(predTextPosition, sf::Color::White, 10, std::string("Predators"));

	this->gfx->DrawSprite(foodPosition, std::string("berry"), scale);
	this->gfx->DrawText(foodTextPosition, sf::Color::White, 10, std::string("Food"));

	this->gfx->DrawSprite(rockPosition, std::string("rock"), scale);
	this->gfx->DrawText(rockTextPosition, sf::Color::White, 10, std::string("Rock"));
}

void GUIsim::mouseEvent(int mouseX, int mouseY, sf::RenderWindow& window)
{
	auto windowSize = window.getView().getSize();

	switch (currentScreen_)
	{
	case mainScreen:
		mainScreen_(window);
		break;
	case optionScreen:
		optionScreen_(window);
		break;
	case creditScreen:
		creditScreen_(window);
		break;
	}

	if (!(mouseX > (windowSize.x / 2 - (BUTTONWIDTH / 2))) && !(mouseX < (windowSize.x / 2 + (BUTTONWIDTH / 2))))
		return;

	switch (currentScreen_)
	{
	case mainScreen:
		if ((mouseY > 200) && (mouseY < 200 + this->gfx->GetTextureHeight("begin")))
			gameStarted = true;
		else if ((mouseY > 260) && (mouseY < 250 + this->gfx->GetTextureHeight("options")))
			currentScreen_ = optionScreen;
		else if ((mouseY > 320) && (mouseY < 320 + this->gfx->GetTextureHeight("credits")))
			currentScreen_ = creditScreen;
		break;
	case optionScreen:
		if ((mouseY > windowSize.y - 100) && (mouseY < windowSize.y - this->gfx->GetTextureHeight("back")))
			currentScreen_ = mainScreen;
		break;
	case creditScreen:
		if ((mouseY > windowSize.y - 100) && (mouseY < windowSize.y - this->gfx->GetTextureHeight("back")))
			currentScreen_ = mainScreen;
		break;
	}
}

void GUIsim::drawMenu(sf::RenderWindow& window)
{
	switch (currentScreen_)
	{
	case mainScreen:
		mainScreen_(window);
		break;
	case optionScreen:
		optionScreen_(window);
		break;
	case creditScreen:
		creditScreen_(window);
		break;
	}
}

void GUIsim::mainScreen_(sf::RenderWindow& window)
{
	auto windowSize = window.getView().getSize();
	auto mainTitlePosition = sf::Vector2f((windowSize.x / 2) - (this->gfx->GetTextureWidth("mainTitle") / 2), 10);
	auto beginPosition = sf::Vector2f((windowSize.x / 2) - (this->gfx->GetTextureWidth("begin") / 2), 200);
	auto optionsPosition = sf::Vector2f((windowSize.x / 2) - (this->gfx->GetTextureWidth("options") / 2), 260);
	auto creditsPosition = sf::Vector2f((windowSize.x / 2) - (this->gfx->GetTextureWidth("credits") / 2), 320);
	auto preyPosition = sf::Vector2f((windowSize.x / 2) - 250, 500);
	auto predPosition = sf::Vector2f((windowSize.x / 2) + 250, 500);

	this->gfx->DrawSprite(mainTitlePosition, "mainTitle");
	this->gfx->DrawSprite(beginPosition, "begin");
	this->gfx->DrawSprite(optionsPosition, "options");
	this->gfx->DrawSprite(creditsPosition, "credits");
	this->gfx->DrawSprite(preyPosition, "prey", 10.0f);
	this->gfx->DrawSprite(predPosition, "pred", 10.0f);
}

void GUIsim::creditScreen_(sf::RenderWindow& window)
{
	auto windowSize = window.getView().getSize();
	auto titlePosition = sf::Vector2f((windowSize.x / 2) - (this->gfx->GetTextureWidth("mainTitle") / 2), 10);
	auto tablePosition = sf::Vector2f((windowSize.x / 2) - (this->gfx->GetTextureWidth("table") / 2), 210);
	auto backPosition = sf::Vector2f((windowSize.x / 2) - (this->gfx->GetTextureWidth("back") / 2), windowSize.y - 100);

	this->gfx->DrawSprite(titlePosition, "mainTitle");
	this->gfx->DrawSprite(tablePosition, "table");

	for (unsigned int idx = 0; idx < teamAndRoles_.size(); ++idx)
	{
		auto teamAndRolesPosition = sf::Vector2f((windowSize.x / 2) - 175, 300 + (20 * idx));
		this->gfx->DrawText(
			teamAndRolesPosition,
			sf::Color::White,
			10,
			teamAndRoles_[idx].first + ": " + teamAndRoles_[idx].second);
	}

	this->gfx->DrawSprite(backPosition, "back");
}

void GUIsim::optionScreen_(sf::RenderWindow& window)
{
	auto windowSize = window.getView().getSize();
	auto titlePosition = sf::Vector2f((windowSize.x / 2) - (this->gfx->GetTextureWidth("mainTitle") / 2), 10);
	auto smartPosition = sf::Vector2f((windowSize.x / 2) - 100, 500);
	auto togglePosition = sf::Vector2f(windowSize.x / 2, 520);
	auto toggleSize = sf::Vector2f(100, 20);
	auto savePosition = sf::Vector2f((windowSize.x / 2) - (this->gfx->GetTextureWidth("save") / 2), windowSize.y - 100);

	this->gfx->DrawSprite(titlePosition, "mainTitle");

	this->showOption(window, "Grid Size", simulationSettings_.gridSize, 200);
	this->showOption(window, "Obstacles", simulationSettings_.numOfObstacles, 250);
	this->showOption(window, "Food", simulationSettings_.numOfFood, 300);
	this->showOption(window, "Predators", simulationSettings_.numOfPred, 350);
	this->showOption(window, "Prey", simulationSettings_.numOfPrey, 400);
	this->showOption(window, "Time Out x 100", simulationSettings_.simTimeOut, 450);

	//INTELLIGENCE
	sf::Font font;
	font.loadFromFile("assets/8bit.ttf");

	this->gfx->DrawText(smartPosition, sf::Color::White, 10, "Smart AI?");
	this->gfx->DrawRectangle(togglePosition, toggleSize, sf::Color::Black);

	if (simulationSettings_.intelligence == true)
	{
		this->gfx->DrawRectangle(togglePosition, toggleSize, sf::Color::Red);
		this->gfx->DrawText(togglePosition, sf::Color::Red, 10, "ON");
	}
	else
	{
		this->gfx->DrawText(togglePosition, sf::Color::Red, 10, "OFF");
	}

	this->gfx->DrawSprite(savePosition, "save");

	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		isToggled_ = true;
	}

	auto position = sf::Mouse::getPosition(window);
	auto maxSlide = 200;
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
		(position.x > (windowSize.x / 2) - 100) &&
		(position.x < (windowSize.x / 2) + 100))
	{
		if ((position.y > 220) && (position.y < 230)) // Grid Size
		{
			simulationSettings_.gridSize = position.x - ((windowSize.x / 2) - 100);
		}

		else if ((position.y > 270) && (position.y < 280)) // Obstacles
		{
			simulationSettings_.numOfObstacles = position.x - ((windowSize.x / 2) - 100);
		}

		else if ((position.y > 320) && (position.y < 330)) // Food
		{
			simulationSettings_.numOfFood = position.x - ((windowSize.x / 2) - 100);
		}

		else if ((position.y > 370) && (position.y < 380)) // Predator
		{
			simulationSettings_.numOfPred = position.x - ((windowSize.x / 2) - 100);
		}

		else if ((position.y > 420) && (position.y < 430)) // Prey
		{
			simulationSettings_.numOfPrey = position.x - ((windowSize.x / 2) - 100);
		}

		else if ((position.y > 470) && (position.y < 480)) // Prey
		{
			simulationSettings_.simTimeOut = position.x - ((windowSize.x / 2) - 100);
		}

		else if (((position.y > 520) && (position.y < 530)) && (isToggled_))
		{
			simulationSettings_.intelligence = !simulationSettings_.intelligence;
			isToggled_ = false;
		}
	}
}

void GUIsim::showOption(sf::RenderWindow& window, std::string title, unsigned int value, float height) const
{
	auto windowSize = window.getView().getSize();
	auto textPosition = sf::Vector2f((windowSize.x / 2) - 100, height);
	auto barPosition = sf::Vector2f((windowSize.x / 2) - 100, height + 20);
	auto barSize = sf::Vector2f(200, 10);
	auto sliderPosition = sf::Vector2f((windowSize.x / 2) - 100 + value, height + 20);
	auto sliderSize = sf::Vector2f(10, 10);
	auto colorGray = sf::Color(100, 100, 100);

	this->gfx->DrawText(textPosition, sf::Color::White, 10, title + ": " + std::to_string(value));
	this->gfx->DrawRectangle(barPosition, barSize, colorGray);
	this->gfx->DrawRectangle(sliderPosition, sliderSize, sf::Color::Black);
}

simSettings GUIsim::getSettings()
{
	return simulationSettings_;
}

bool GUIsim::isComplete(std::pair<unsigned int, unsigned int> entities, int iterationCount)
{
	simEnd = false;
	if ((entities.first == 0 && simulationSettings_.numOfPrey > 0) || (iterationCount == (simulationSettings_.simTimeOut * 100)) || (entities.second == 0 && simulationSettings_.numOfPred > 0))
		return true;
	else
		return false; //just carry on son
}

void GUIsim::endGameScreen(sf::RenderWindow& window, std::pair<unsigned int, unsigned int> entities, int iterationCount)
{
	auto windowSize = window.getView().getSize();
	auto titlePosition = sf::Vector2f((windowSize.x / 2) - (this->gfx->GetTextureWidth("mainTitle") / 2), 10);
	auto resultsPosition = sf::Vector2f(windowSize.x / 2, 200);
	auto firstPosition = sf::Vector2f(windowSize.x / 2, 225);
	auto secondPosition = sf::Vector2f(windowSize.x / 2, 250);
	auto timePosition = sf::Vector2f(windowSize.x / 2, 330);
	auto settingsPosition = sf::Vector2f(windowSize.x / 2, 340);
	auto gridSizePosition = sf::Vector2f(windowSize.x / 2, 350);
	auto obstaclePosition = sf::Vector2f(windowSize.x / 2, 360);
	auto foodPosition = sf::Vector2f(windowSize.x / 2, 370);
	auto predPosition = sf::Vector2f(windowSize.x / 2, 380);
	auto preyPosition = sf::Vector2f(windowSize.x / 2, 390);
	auto timeoutPosition = sf::Vector2f(windowSize.x / 2, 400);
	auto menuPosition = sf::Vector2f((windowSize.x / 2) - (this->gfx->GetTextureWidth("menu") / 2), windowSize.y - 170);
	auto exitPosition = sf::Vector2f((windowSize.x / 2) - (this->gfx->GetTextureWidth("exit") / 2), windowSize.y - 100);

	this->gfx->DrawSprite(titlePosition, "mainTitle");
	this->gfx->DrawText(resultsPosition, sf::Color::White, 10, "Simulation Results");

	if (entities.first == 0)
	{
		this->gfx->DrawText(firstPosition, sf::Color::White, 10, std::to_string(entities.second) + " Predators remained");
		this->gfx->DrawText(secondPosition, sf::Color::White, 10, "last Prey killed at: ");
	}
	else if (entities.second == 0)
	{
		this->gfx->DrawText(firstPosition, sf::Color::White, 10, std::to_string(entities.first) + " Prey Survived");
		this->gfx->DrawText(secondPosition, sf::Color::White, 10, "last Predator killed at: ");
	}
	else
	{
		this->gfx->DrawText(firstPosition, sf::Color::White, 10, std::to_string(entities.first) + " Prey Survived");
		this->gfx->DrawText(secondPosition, sf::Color::White, 10, std::to_string(entities.second) + " Predators remained");
	}

	this->gfx->DrawText(timePosition, sf::Color::White, 10, "Game Time: " + std::to_string(iterationCount) + " iterations");
	this->gfx->DrawText(settingsPosition, sf::Color::White, 10, "Simulation Settings");
	this->gfx->DrawText(gridSizePosition, sf::Color::White, 10, "GridSize: " + std::to_string(simulationSettings_.gridSize));
	this->gfx->DrawText(obstaclePosition, sf::Color::White, 10, "Obstacles: " + std::to_string(simulationSettings_.numOfObstacles));
	this->gfx->DrawText(foodPosition, sf::Color::White, 10, "Food: " + std::to_string(simulationSettings_.numOfFood));
	this->gfx->DrawText(predPosition, sf::Color::White, 10, "Predators: " + std::to_string(simulationSettings_.numOfPred));
	this->gfx->DrawText(preyPosition, sf::Color::White, 10, "Prey: " + std::to_string(simulationSettings_.numOfPrey));
	this->gfx->DrawText(timeoutPosition, sf::Color::White, 10, "Timeout: " + std::to_string(simulationSettings_.simTimeOut));

	this->gfx->DrawSprite(menuPosition, "menu");
	this->gfx->DrawSprite(exitPosition, "exit");

	auto position = sf::Mouse::getPosition(window);
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
		(position.x > ((windowSize.x / 2) - (this->gfx->GetTextureWidth("menu") / 2))) &&
		((position.x < windowSize.x / 2) + (this->gfx->GetTextureWidth("menu") / 2)))
	{
		if ((position.y > windowSize.y - 170) && (position.y < (windowSize.y - 170) + this->gfx->GetTextureHeight("menu"))) // Menu Button
		{
			simEnd = true;
			gameStarted = false;
		}
		else if ((position.y > windowSize.y - 100) && (position.y < (windowSize.y - 100) + this->gfx->GetTextureHeight("exit"))) // Exit Button
		{
			simEnd = simQuit = true;
		}
	}
}
