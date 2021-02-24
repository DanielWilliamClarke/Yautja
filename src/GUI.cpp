#include "GUI.h"

GUIsim::GUIsim()
{
	currentScreen_ = mainScreen;
	simSettings defaultSettings = { 30, 50, 5, 10, 50, 20, false };
	simulationSettings_ = defaultSettings;
	gameStarted = false;
	simEnd = false;
	simQuit = false;
	sleepSliderV_ = 400;

	teamAndRoles_.push_back(std::pair<std::string, std::string>("Aiden Gallagher", "Team Leader, Researcher"));
	teamAndRoles_.push_back(std::pair<std::string, std::string>("Alex Rudd", "Programmer"));
	teamAndRoles_.push_back(std::pair<std::string, std::string>("Daniel Clarke", "Graphics Designer, Programmer"));
	teamAndRoles_.push_back(std::pair<std::string, std::string>("Emma Hall", "Researcher"));
	teamAndRoles_.push_back(std::pair<std::string, std::string>("Lewis Kell", "Deputy Leader"));
	teamAndRoles_.push_back(std::pair<std::string, std::string>("Michael Percivals", "Programmer"));
	teamAndRoles_.push_back(std::pair<std::string, std::string>("Xiaorong Li", "Researcher"));

	this->textures.insert({
		{ "background", this->loadTexture("assets/GUI/Background.png") },
		{ "begin", this->loadTexture("assets/GUI/BeginButton.png") },
		{ "options", this->loadTexture("assets/GUI/OptionsButton.png") },
		{ "credits", this->loadTexture("assets/GUI/CreditsButton.png") },
		{ "mainTitle", this->loadTexture("assets/GUI/MainTitle.png") },
		{ "save", this->loadTexture("assets/GUI/SaveButton.png") },
		{ "menu", this->loadTexture("assets/GUI/ResetButton.png") },
		{ "exit", this->loadTexture("assets/GUI/ExitButton.png") },
		{ "table", this->loadTexture("assets/GUI/Table.png") },
		{ "back", this->loadTexture("assets/GUI/BackButton.png") },
		{ "prey", this->loadTexture("assets/Sprites/8bit prey.png") },
		{ "pred", this->loadTexture("assets/Sprites/8bit pred.png") },
		{ "berry", this->loadTexture("assets/Sprites/8bit berry.png") },
		{ "rock", this->loadTexture("assets/Sprites/8bit rock.png") },
		});
}

std::shared_ptr<sf::Texture> GUIsim::loadTexture(std::string texturePath) const
{
	auto texture = std::make_shared<sf::Texture>();
	texture->loadFromFile(texturePath);
	return texture;
}

std::shared_ptr<sf::Texture> GUIsim::findTexture(std::string textureName) const
{
	return this->textures.at(textureName);
}

unsigned int GUIsim::statusBar(sf::RenderTarget& window)
{
	auto windowSize = window.getView().getSize();
	auto position = sf::Mouse::getPosition();
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && (position.y > windowSize.y - 80) && (position.y < windowSize.y - 50)
		&& (position.x > (windowSize.x / 2) - 196) && (position.x < (windowSize.x / 2) + 206))
	{
		sleepSliderV_ = (position.x - 5) - ((windowSize.x / 2) - 200);
	}

	sf::Text text;
	text.setPosition(windowSize.x / 2 - 200, windowSize.y - 85);
	text.setFillColor(sf::Color::White);
	text.setString("Soeed: " + std::to_string(sleepSliderV_));
	window.draw(text);

	sf::RectangleShape bar;
	bar.setFillColor(sf::Color(34, 34, 34));
	bar.setPosition(windowSize.x / 2 - 200, windowSize.y - 70);
	bar.setSize(sf::Vector2f(
		windowSize.x / 2 + 200, windowSize.y - 60));
	window.draw(bar);

	sf::RectangleShape handle;
	handle.setFillColor(sf::Color::Green);
	handle.setPosition((windowSize.x / 2 - 200) + sleepSliderV_, windowSize.y - 75);
	handle.setSize(sf::Vector2f(
		(windowSize.x / 2 + 200) + sleepSliderV_ + 10, windowSize.y - 55));
	window.draw(handle);

	return 400 - sleepSliderV_;
}

void GUIsim::legend(sf::RenderTarget& window)
{
	auto windowSize = window.getView().getSize();
	sf::Sprite sprite;
	sf::Text text;

	sprite.setTexture(*this->findTexture("prey"));
	sprite.setColor(sf::Color(255, 255, 255, 200));
	sprite.setPosition((windowSize.x / 2) - 60, windowSize.y - 30);
	window.draw(sprite);

	text.setPosition((windowSize.x / 2) - 100, windowSize.y - 30);
	text.setFillColor(sf::Color::White);
	text.setString("Prey");
	window.draw(text);

	sprite.setTexture(*this->findTexture("pred"));
	sprite.setColor(sf::Color(255, 255, 255, 200));
	sprite.setPosition((windowSize.x / 2) - 30, windowSize.y - 30);
	window.draw(sprite);

	text.setPosition((windowSize.x / 2) - 30, windowSize.y - 30);
	text.setFillColor(sf::Color::White);
	text.setString("Predators");
	window.draw(text);

	sprite.setTexture(*this->findTexture("berry"));
	sprite.setColor(sf::Color(255, 255, 255, 200));
	sprite.setPosition((windowSize.x / 2) + 100, windowSize.y - 30);
	window.draw(sprite);

	text.setPosition((windowSize.x / 2) + 60, windowSize.y - 30);
	text.setFillColor(sf::Color::White);
	text.setString("Food");
	window.draw(text);

	sprite.setTexture(*this->findTexture("rock"));
	sprite.setColor(sf::Color(255, 255, 255, 200));
	sprite.setPosition((windowSize.x / 2) + 180, windowSize.y - 30);
	window.draw(sprite);

	text.setPosition((windowSize.x / 2) + 140, windowSize.y - 30);
	text.setFillColor(sf::Color::White);
	text.setString("Rock");
	window.draw(text);
}

void GUIsim::mouseEvent(int mouseX, int mouseY, sf::RenderTarget& window)
{
	auto windowSize = window.getView().getSize();

	sf::Sprite sprite;
	sprite.setTexture(*this->findTexture("background"));
	sprite.setColor(sf::Color(255, 255, 255, 200));
	sprite.setPosition(0, 0);
	window.draw(sprite);

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
		if ((mouseY > 200) && (mouseY < 200 + this->findTexture("begin")->getSize().y))
			gameStarted = true;
		else if ((mouseY > 260) && (mouseY < 250 + this->findTexture("options")->getSize().y))
			currentScreen_ = optionScreen;
		else if ((mouseY > 320) && (mouseY < 320 + this->findTexture("credits")->getSize().y))
			currentScreen_ = creditScreen;
		break;
	case optionScreen:
		if ((mouseY > windowSize.y - 100) && (mouseY < windowSize.y - this->findTexture("back")->getSize().y))
			currentScreen_ = mainScreen;
		break;
	case creditScreen:
		if ((mouseY > windowSize.y - 100) && (mouseY < windowSize.y - this->findTexture("back")->getSize().y))
			currentScreen_ = mainScreen;
		break;
	}
}

void GUIsim::drawMenu(sf::RenderTarget& window)
{
	sf::Sprite sprite;
	sprite.setTexture(*this->findTexture("background"));
	sprite.setColor(sf::Color(255, 255, 255, 200));
	sprite.setPosition(0, 0);
	window.draw(sprite);

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

void GUIsim::mainScreen_(sf::RenderTarget& window)
{
	auto windowSize = window.getView().getSize();
	sf::Sprite sprite;

	auto mainTitleTexture = this->findTexture("mainTitle");
	sprite.setTexture(*mainTitleTexture);
	sprite.setColor(sf::Color(255, 255, 255, 200));
	sprite.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2), 10);
	window.draw(sprite);

	auto beginTexture = this->findTexture("begin");
	sprite.setTexture(*beginTexture);
	sprite.setColor(sf::Color(255, 255, 255, 200));
	sprite.setPosition((windowSize.x / 2) - (beginTexture->getSize().x / 2), 200);
	window.draw(sprite);

	auto optionsTexture = this->findTexture("options");
	sprite.setTexture(*optionsTexture);
	sprite.setColor(sf::Color(255, 255, 255, 200));
	sprite.setPosition((windowSize.x / 2) - (optionsTexture->getSize().x / 2), 260);
	window.draw(sprite);

	auto creditsTexture = this->findTexture("credits");
	sprite.setTexture(*creditsTexture);
	sprite.setColor(sf::Color(255, 255, 255, 200));
	sprite.setPosition((windowSize.x / 2) - (creditsTexture->getSize().x / 2), 320);
	window.draw(sprite);

	sprite.setTexture(*this->findTexture("prey"));
	sprite.setColor(sf::Color(255, 255, 255, 200));
	sprite.setPosition(100, 500);
	sprite.scale(4.0f, 4.0f);
	window.draw(sprite);

	sprite.setTexture(*this->findTexture("pred"));
	sprite.setColor(sf::Color(255, 255, 255, 200));
	sprite.setPosition(windowSize.x + 250, 500);
	sprite.scale(4.0f, 4.0f);
	window.draw(sprite);
}

void GUIsim::creditScreen_(sf::RenderTarget& window)
{
	auto windowSize = window.getView().getSize();
	sf::Sprite sprite;

	auto mainTitleTexture = this->findTexture("mainTitle");
	sprite.setTexture(*mainTitleTexture);
	sprite.setColor(sf::Color(255, 255, 255, 200));
	sprite.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2), 10);
	window.draw(sprite);

	auto tableTexture = this->findTexture("table");
	sprite.setTexture(*tableTexture);
	sprite.setColor(sf::Color(255, 255, 255, 200));
	sprite.setPosition((windowSize.x / 2) - (tableTexture->getSize().x / 2), 210);
	window.draw(sprite);

	sf::Text text;
	for (unsigned int idx = 0; idx < teamAndRoles_.size(); ++idx)
	{
		text.setPosition((windowSize.x / 2) - 175, 300 + (20 * idx));
		text.setFillColor(sf::Color::White);
		text.setString(teamAndRoles_[idx].first + ": " + teamAndRoles_[idx].second);
		window.draw(text);
	}

	auto backTexture = this->findTexture("back");
	sprite.setTexture(*backTexture);
	sprite.setColor(sf::Color(255, 255, 255, 200));
	sprite.setPosition((windowSize.x / 2) - (backTexture->getSize().x / 2), windowSize.x - 100);
	window.draw(sprite);
}

void GUIsim::optionScreen_(sf::RenderTarget& window)
{
	auto windowSize = window.getView().getSize();
	sf::RectangleShape reactangle;
	sf::Sprite sprite;
	sf::Text text;

	auto mainTitleTexture = this->findTexture("mainTitle");
	sprite.setTexture(*mainTitleTexture);
	sprite.setColor(sf::Color(255, 255, 255, 200));
	sprite.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2), 10);
	window.draw(sprite);

	//GRID SIZE
	text.setPosition((windowSize.x / 2) - 100, 200);
	text.setFillColor(sf::Color::Black);
	text.setString("Grid Size: " + simulationSettings_.gridSize);
	window.draw(text);

	reactangle.setFillColor(sf::Color(34, 34, 34));
	reactangle.setPosition(
		windowSize.x / 2 - 100,
		220);
	reactangle.setSize(sf::Vector2f(
		windowSize.x / 2 + 100,
		230));
	window.draw(reactangle);

	reactangle.setFillColor(sf::Color::Black);
	reactangle.setPosition(
		(windowSize.x / 2) - 100 + simulationSettings_.gridSize,
		215);
	reactangle.setSize(sf::Vector2f(
		(windowSize.x / 2 - 100) + simulationSettings_.gridSize + 10,
		235));
	window.draw(reactangle);

	//OBSTACLES
	text.setPosition((windowSize.x / 2) - 100, 250);
	text.setFillColor(sf::Color::Black);
	text.setString("Obstacles: " + simulationSettings_.numOfObstacles);
	window.draw(text);

	reactangle.setFillColor(sf::Color(34, 34, 34));
	reactangle.setPosition(
		windowSize.x / 2 - 100,
		270);
	reactangle.setSize(sf::Vector2f(
		windowSize.x / 2 + 100,
		280));
	window.draw(reactangle);

	reactangle.setFillColor(sf::Color::Black);
	reactangle.setPosition(
		(windowSize.x / 2) - 100 + simulationSettings_.numOfObstacles,
		265);
	reactangle.setSize(sf::Vector2f(
		(windowSize.x / 2 - 100) + simulationSettings_.numOfObstacles + 10,
		285));
	window.draw(reactangle);


	//FOOD
	text.setPosition((windowSize.x / 2) - 100, 300);
	text.setFillColor(sf::Color::Black);
	text.setString("Food: " + simulationSettings_.numOfFood);
	window.draw(text);

	reactangle.setFillColor(sf::Color(34, 34, 34));
	reactangle.setPosition(
		windowSize.x / 2 - 100,
		320);
	reactangle.setSize(sf::Vector2f(
		windowSize.x / 2 + 100,
		330));
	window.draw(reactangle);

	reactangle.setFillColor(sf::Color::Black);
	reactangle.setPosition(
		(windowSize.x / 2) - 100 + simulationSettings_.numOfFood,
		315);
	reactangle.setSize(sf::Vector2f(
		(windowSize.x / 2 - 100) + simulationSettings_.numOfFood + 10,
		335));
	window.draw(reactangle);

	//PREDATORS
	text.setPosition((windowSize.x / 2) - 100, 350);
	text.setFillColor(sf::Color::Black);
	text.setString("Predators: " + simulationSettings_.numOfPred);
	window.draw(text);

	reactangle.setFillColor(sf::Color(34, 34, 34));
	reactangle.setPosition(
		windowSize.x / 2 - 100,
		370);
	reactangle.setSize(sf::Vector2f(
		windowSize.x / 2 + 100,
		380));
	window.draw(reactangle);

	reactangle.setFillColor(sf::Color::Black);
	reactangle.setPosition(
		(windowSize.x / 2) - 100 + simulationSettings_.numOfPred,
		365);
	reactangle.setSize(sf::Vector2f(
		(windowSize.x / 2 - 100) + simulationSettings_.numOfPred + 10,
		385));
	window.draw(reactangle);

	//PREY
	text.setPosition((windowSize.x / 2) - 100, 400);
	text.setFillColor(sf::Color::Black);
	text.setString("Prey: " + simulationSettings_.numOfPrey);
	window.draw(text);

	reactangle.setFillColor(sf::Color(34, 34, 34));
	reactangle.setPosition(
		windowSize.x / 2 - 100,
		420);
	reactangle.setSize(sf::Vector2f(
		windowSize.x / 2 + 100,
		430));
	window.draw(reactangle);

	reactangle.setFillColor(sf::Color::Black);
	reactangle.setPosition(
		(windowSize.x / 2) - 100 + simulationSettings_.numOfPrey,
		415);
	reactangle.setSize(sf::Vector2f(
		(windowSize.x / 2 - 100) + simulationSettings_.numOfPrey + 10,
		435));
	window.draw(reactangle);

	//TIME OUT
	text.setPosition((windowSize.x / 2) - 100, 450);
	text.setFillColor(sf::Color::Black);
	text.setString("Time Out x 100: " + simulationSettings_.simTimeOut);
	window.draw(text);

	reactangle.setFillColor(sf::Color(34, 34, 34));
	reactangle.setPosition(
		windowSize.x / 2 - 100,
		470);
	reactangle.setSize(sf::Vector2f(
		windowSize.x / 2 + 100,
		480));
	window.draw(reactangle);

	reactangle.setFillColor(sf::Color::Black);
	reactangle.setPosition(
		(windowSize.x / 2) - 100 + simulationSettings_.simTimeOut,
		465);
	reactangle.setSize(sf::Vector2f(
		(windowSize.x / 2 - 100) + simulationSettings_.simTimeOut + 10,
		485));
	window.draw(reactangle);

	//INTELLIGENCE
	text.setPosition((windowSize.x / 2) - 100, 500);
	text.setFillColor(sf::Color::Black);
	text.setString("Smart AI?");
	window.draw(text);

	reactangle.setFillColor(sf::Color::Black);
	reactangle.setPosition(
		windowSize.x / 2 - 100,
		520);
	reactangle.setSize(sf::Vector2f(
		windowSize.x / 2 + 100,
		530));
	window.draw(reactangle);

	if (simulationSettings_.intelligence == true)
	{

		reactangle.setOutlineColor(sf::Color::Red);
		reactangle.setPosition(
			windowSize.x / 2 - 100,
			520);
		reactangle.setSize(sf::Vector2f(
			windowSize.x / 2 + 100,
			530));
		window.draw(reactangle);

		text.setPosition((windowSize.x / 2), 500);
		text.setFillColor(sf::Color::Red);
		text.setString("ON");
		window.draw(text);

	}
	else {
		text.setPosition((windowSize.x / 2), 500);
		text.setFillColor(sf::Color::Red);
		text.setString("OFF");
		window.draw(text);
	}

	//draw save button
	auto saveTexture = this->findTexture("save");
	sprite.setTexture(*saveTexture);
	sprite.setColor(sf::Color(255, 255, 255, 200));
	sprite.setPosition((windowSize.x / 2) - (saveTexture->getSize().x / 2), windowSize.y - 100);
	window.draw(sprite);

	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		isToggled_ = true;
	}

	auto position = sf::Mouse::getPosition();
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
		(position.x > windowSize.x / 2 - 96) && (position.x < windowSize.x / 2 + 106))
	{
		if ((position.y > 200) && (position.y < 250)) // Grid Size
			simulationSettings_.gridSize = (position.x - 5 - (windowSize.x / 2 - 100));
		else if ((position.y > 250) && (position.y < 300)) // Obstacles
			simulationSettings_.numOfObstacles = (position.x - 5 - (windowSize.x / 2 - 100));
		else if ((position.y > 300) && (position.y < 350)) // Food
			simulationSettings_.numOfFood = (position.x - 5 - (windowSize.x / 2 - 100));
		else if ((position.y > 350) && (position.y < 400)) // Predator
			simulationSettings_.numOfPred = (position.x - 5 - (windowSize.x / 2 - 100));
		else if ((position.y > 400) && (position.y < 450)) // Prey
			simulationSettings_.numOfPrey = (position.x - 5 - (windowSize.x / 2 - 100));
		else if ((position.y > 450) && (position.y < 500)) // Prey
			simulationSettings_.simTimeOut = (position.x - 5 - (windowSize.x / 2 - 100));
		else if (((position.y > 500) && (position.y < 550)) && (isToggled_))
		{
			simulationSettings_.intelligence = !simulationSettings_.intelligence;
			isToggled_ = false;
		}
	}
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
		return false;//just carry on son
}

void GUIsim::endGameScreen(sf::RenderTarget& window, std::pair<unsigned int, unsigned int> entities, int iterationCount)
{
	auto windowSize = window.getView().getSize();

	sf::Sprite sprite;
	sf::Text text;

	sprite.setTexture(*this->findTexture("background"));
	sprite.setColor(sf::Color(255, 255, 255, 200));
	sprite.setPosition(0, 0);
	window.draw(sprite);

	auto mainTitleTexture = this->findTexture("mainTitle");
	sprite.setTexture(*mainTitleTexture);
	sprite.setColor(sf::Color(255, 255, 255, 200));
	sprite.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2), 10);
	window.draw(sprite);

	auto tableTexture = this->findTexture("table");
	sprite.setTexture(*tableTexture);
	sprite.setColor(sf::Color(255, 255, 255, 200));
	sprite.setPosition((windowSize.x / 2) - (tableTexture->getSize().x / 2), 210);
	window.draw(sprite);

	text.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2), 300);
	text.setString("Simulation Results");
	window.draw(text);

	if (entities.first == 0)
	{
		text.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2), 325);
		text.setString(std::to_string(entities.second) + " Predators remained");
		window.draw(text);

		text.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2), 350);
		text.setString("last Prey killed at: ");
		window.draw(text);
	}
	else if (entities.second == 0)
	{
		text.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2), 325);
		text.setString(std::to_string(entities.first) + " Prey Survived");
		window.draw(text);

		text.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2), 350);
		text.setString("last Predator killed at: ");
		window.draw(text);
	}
	else
	{
		text.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2), 325);
		text.setString(std::to_string(entities.first) + " Prey Survived");
		window.draw(text);

		text.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2), 350);

		text.setString(std::to_string(entities.second) + " Predators remained");
		window.draw(text);
	}


	text.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2), 375);
	text.setString("Game Time: " + std::to_string(iterationCount) + " iterations");
	window.draw(text);

	text.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2) + 200, 300);
	text.setString("Simulation Settings");
	window.draw(text);

	text.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2) + 200, 325);
	text.setString("GridSize: " + std::to_string(simulationSettings_.gridSize));
	window.draw(text);

	text.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2) + 200, 335);
	text.setString("Obstacles: " + std::to_string(simulationSettings_.numOfObstacles));
	window.draw(text);

	text.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2) + 200, 345);
	text.setString("Food: " + std::to_string(simulationSettings_.numOfFood));
	window.draw(text);

	text.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2) + 200, 355);
	text.setString("Predators: " + std::to_string(simulationSettings_.numOfPred));
	window.draw(text);

	text.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2) + 200, 365);
	text.setString("Prey: " + std::to_string(simulationSettings_.numOfPrey));
	window.draw(text);

	text.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2) + 200, 375);
	text.setString("Timeout: " + std::to_string(simulationSettings_.simTimeOut));
	window.draw(text);

	auto menuTexture = this->findTexture("menu");
	sprite.setTexture(*menuTexture);
	sprite.setColor(sf::Color(255, 255, 255, 200));
	sprite.setPosition((windowSize.x / 2) - (menuTexture->getSize().x / 2), windowSize.y - 170);
	window.draw(sprite);

	auto exitTexture = this->findTexture("exit");
	sprite.setTexture(*exitTexture);
	sprite.setColor(sf::Color(255, 255, 255, 200));
	sprite.setPosition((windowSize.x / 2) - (exitTexture->getSize().x / 2), windowSize.y - 100);
	window.draw(sprite);

	auto position = sf::Mouse::getPosition();
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
		(position.x > ((windowSize.x / 2) - (menuTexture->getSize().x / 2))) &&
		((position.x < windowSize.x / 2) + (menuTexture->getSize().x / 2)))
	{
		if ((position.y > windowSize.y - 170) && (position.y < (windowSize.y - 170) + menuTexture->getSize().y)) // Menu Button
		{
			simEnd = true;
			gameStarted = false;
		}
		else if ((position.y >windowSize.y - 100) && (position.y < (windowSize.y - 100) + exitTexture->getSize().y)) // Exit Button
		{
			simEnd = simQuit = true;
		}
	}

}



