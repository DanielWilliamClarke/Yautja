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
		{ "prey", this->loadTexture("assets/Sprites/prey.png") },
		{ "pred", this->loadTexture("assets/Sprites/pred.png") },
		{ "berry", this->loadTexture("assets/Sprites/berry.png") },
		{ "rock", this->loadTexture("assets/Sprites/rock.png") },
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

unsigned int GUIsim::statusBar(sf::RenderWindow& window)
{
	auto windowSize = window.getView().getSize();
	auto position = sf::Mouse::getPosition(window);
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

void GUIsim::legend(sf::RenderWindow& window)
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

void GUIsim::mouseEvent(int mouseX, int mouseY, sf::RenderWindow& window)
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

void GUIsim::drawMenu(sf::RenderWindow& window)
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

void GUIsim::mainScreen_(sf::RenderWindow& window)
{
	auto windowSize = window.getView().getSize();
	sf::Sprite mainTitleSprite;
	auto mainTitleTexture = this->findTexture("mainTitle");
	mainTitleSprite.setTexture(*mainTitleTexture);
	mainTitleSprite.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2), 10);
	window.draw(mainTitleSprite);

	sf::Sprite beginSprite;
	auto beginTexture = this->findTexture("begin");
	beginSprite.setTexture(*beginTexture);
	beginSprite.setPosition((windowSize.x / 2) - (beginTexture->getSize().x / 2), 200);
	window.draw(beginSprite);

	sf::Sprite optionSprite;
	auto optionsTexture = this->findTexture("options");
	optionSprite.setTexture(*optionsTexture);
	optionSprite.setPosition((windowSize.x / 2) - (optionsTexture->getSize().x / 2), 260);
	window.draw(optionSprite);

	sf::Sprite creditsSprite;
	auto creditsTexture = this->findTexture("credits");
	creditsSprite.setTexture(*creditsTexture);
	creditsSprite.setPosition((windowSize.x / 2) - (creditsTexture->getSize().x / 2), 320);
	window.draw(creditsSprite);

	sf::Sprite preySprite;
	preySprite.setTexture(*this->findTexture("prey"));
	preySprite.setPosition((windowSize.x / 2) - 250, 500);
	preySprite.scale(10.0f, 10.0f);
	window.draw(preySprite);

	sf::Sprite predSprite;
	predSprite.setTexture(*this->findTexture("pred"));
	predSprite.setPosition((windowSize.x / 2) + 250, 500);
	predSprite.scale(10.0f, 10.0f);
	window.draw(predSprite);
}

void GUIsim::creditScreen_(sf::RenderWindow& window)
{
	auto windowSize = window.getView().getSize();

	sf::Sprite mainTitleSprite;
	auto mainTitleTexture = this->findTexture("mainTitle");
	mainTitleSprite.setTexture(*mainTitleTexture);
	mainTitleSprite.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2), 10);
	window.draw(mainTitleSprite);

	sf::Sprite tableSprite;
	auto tableTexture = this->findTexture("table");
	tableSprite.setTexture(*tableTexture);
	tableSprite.setPosition((windowSize.x / 2) - (tableTexture->getSize().x / 2), 210);
	window.draw(tableSprite);

	sf::Font font;
	font.loadFromFile("assets/8bit.ttf");

	for (unsigned int idx = 0; idx < teamAndRoles_.size(); ++idx)
	{
		sf::Text text;
		text.setFont(font);
		text.setCharacterSize(24);
		text.setPosition((windowSize.x / 2) - 175, 300 + (20 * idx));
		text.setFillColor(sf::Color::White);
		text.setString(teamAndRoles_[idx].first + ": " + teamAndRoles_[idx].second);
		window.draw(text);
	}

	sf::Sprite backSprite;
	auto backTexture = this->findTexture("back");
	backSprite.setTexture(*backTexture);
	backSprite.setColor(sf::Color(255, 255, 255, 200));
	backSprite.setPosition((windowSize.x / 2) - (backTexture->getSize().x / 2), windowSize.y - 100);
	window.draw(backSprite);
}

void GUIsim::optionScreen_(sf::RenderWindow& window)
{
	auto windowSize = window.getView().getSize();

	sf::Sprite mainSprite;
	auto mainTitleTexture = this->findTexture("mainTitle");
	mainSprite.setTexture(*mainTitleTexture);
	mainSprite.setColor(sf::Color(255, 255, 255, 200));
	mainSprite.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2), 10);
	window.draw(mainSprite);

	this->showOption(window, "Grid Size", simulationSettings_.gridSize, 200);
	this->showOption(window, "Obstacles", simulationSettings_.numOfObstacles, 250);
	this->showOption(window, "Food", simulationSettings_.numOfFood, 300);
	this->showOption(window, "Predators", simulationSettings_.numOfPred, 350);
	this->showOption(window, "Prey", simulationSettings_.numOfPrey, 400);
	this->showOption(window, "Time Out x 100", simulationSettings_.simTimeOut, 450);
	this->showOption(window, "Time Out x 100", simulationSettings_.simTimeOut, 450);

	//INTELLIGENCE
	sf::Font font;
	font.loadFromFile("assets/8bit.ttf");
	sf::Text text;
	text.setFont(font);
	text.setScale(0.5, 0.5);
	text.setPosition((windowSize.x / 2) - 100, 500);
	text.setFillColor(sf::Color::White);
	text.setString("Smart AI?");
	window.draw(text);

	sf::RectangleShape rectangle;
	rectangle.setFillColor(sf::Color::Black);
	rectangle.setPosition(
		windowSize.x / 2,
		520);
	rectangle.setSize(sf::Vector2f(
		100,
		20));
	window.draw(rectangle);

	if (simulationSettings_.intelligence == true)
	{
		sf::RectangleShape handle;
		handle.setOutlineColor(sf::Color::Red);
		handle.setPosition(
			windowSize.x / 2,
			520);
		handle.setSize(sf::Vector2f(
			100,
			20));
		window.draw(handle);

		sf::Text onText;
		onText.setFont(font);
		onText.setScale(0.5, 0.5);
		onText.setPosition((windowSize.x / 2), 520);
		onText.setFillColor(sf::Color::Red);
		onText.setString("ON");
		window.draw(onText);

	}
	else {
		sf::Text offText;
		offText.setFont(font);
		offText.setScale(0.5, 0.5);
		offText.setPosition((windowSize.x / 2), 520);
		offText.setFillColor(sf::Color::Red);
		offText.setString("OFF");
		window.draw(offText);
	}

	//draw save button
	sf::Sprite saveSprite;
	auto saveTexture = this->findTexture("save");
	saveSprite.setTexture(*saveTexture);
	saveSprite.setColor(sf::Color(255, 255, 255, 200));
	saveSprite.setPosition((windowSize.x / 2) - (saveTexture->getSize().x / 2), windowSize.y - 100);
	window.draw(saveSprite);

	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
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
	sf::Font font;
	font.loadFromFile("assets/8bit.ttf");

	auto windowSize = window.getView().getSize();
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(10);
	text.setPosition((windowSize.x / 2) - 100, height);
	text.setFillColor(sf::Color::White);
	text.setString(title + ": " + std::to_string(value));
	window.draw(text);

	sf::RectangleShape rectangle;
	rectangle.setFillColor(sf::Color(100, 100, 100));
	rectangle.setPosition(
		(windowSize.x / 2) - 100,
		height + 20);
	rectangle.setSize(sf::Vector2f(
		200,
		10));
	window.draw(rectangle);

	sf::RectangleShape handle;
	handle.setFillColor(sf::Color::Black);
	handle.setPosition(
		(windowSize.x / 2) - 100 + value,
		height + 20);
	handle.setSize(sf::Vector2f(
		10,
		10));
	window.draw(handle);
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

void GUIsim::endGameScreen(sf::RenderWindow& window, std::pair<unsigned int, unsigned int> entities, int iterationCount)
{
	auto windowSize = window.getView().getSize();

	sf::Sprite bgSprite;
	bgSprite.setTexture(*this->findTexture("background"));
	bgSprite.setColor(sf::Color(255, 255, 255, 200));
	bgSprite.setPosition(0, 0);
	window.draw(bgSprite);

	sf::Sprite mainSprite;
	auto mainTitleTexture = this->findTexture("mainTitle");
	mainSprite.setTexture(*mainTitleTexture);
	mainSprite.setColor(sf::Color(255, 255, 255, 200));
	mainSprite.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2), 10);
	window.draw(mainSprite);

	sf::Sprite tableSprite;
	auto tableTexture = this->findTexture("table");
	tableSprite.setTexture(*tableTexture);
	tableSprite.setColor(sf::Color(255, 255, 255, 200));
	tableSprite.setPosition((windowSize.x / 2) - (tableTexture->getSize().x / 2), 210);
	window.draw(tableSprite);

	sf::Font font;
	font.loadFromFile("assets/8bit.ttf");
	sf::Text text;
	text.setFont(font);
	text.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2), 300);
	text.setString("Simulation Results");
	window.draw(text);

	if (entities.first == 0)
	{
		sf::Text predText;
		predText.setFont(font);
		predText.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2), 325);
		predText.setString(std::to_string(entities.second) + " Predators remained");
		window.draw(predText);

		sf::Text predTimeText;
		predTimeText.setFont(font);
		predTimeText.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2), 350);
		predTimeText.setString("last Prey killed at: ");
		window.draw(predTimeText);
	}
	else if (entities.second == 0)
	{
		sf::Text preyText;
		preyText.setFont(font);
		preyText.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2), 325);
		preyText.setString(std::to_string(entities.first) + " Prey Survived");
		window.draw(preyText);

		sf::Text predTimeText;
		predTimeText.setFont(font);
		predTimeText.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2), 350);
		predTimeText.setString("last Predator killed at: ");
		window.draw(predTimeText);
	}
	else
	{
		sf::Text predText;
		predText.setFont(font);
		predText.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2), 325);
		predText.setString(std::to_string(entities.first) + " Prey Survived");
		window.draw(predText);

		sf::Text preyText;
		predText.setFont(font);
		preyText.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2), 350);
		preyText.setString(std::to_string(entities.second) + " Predators remained");
		window.draw(preyText);
	}

	sf::Text gameText;
	gameText.setFont(font);
	gameText.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2), 375);
	gameText.setString("Game Time: " + std::to_string(iterationCount) + " iterations");
	window.draw(gameText);

	sf::Text settingsText;
	settingsText.setFont(font);
	settingsText.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2) + 200, 300);
	settingsText.setString("Simulation Settings");
	window.draw(settingsText);

	sf::Text gridText;
	gridText.setFont(font);
	gridText.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2) + 200, 325);
	gridText.setString("GridSize: " + std::to_string(simulationSettings_.gridSize));
	window.draw(gridText);

	sf::Text obsText;
	obsText.setFont(font);
	obsText.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2) + 200, 335);
	obsText.setString("Obstacles: " + std::to_string(simulationSettings_.numOfObstacles));
	window.draw(obsText);

	sf::Text foodText;
	foodText.setFont(font);
	foodText.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2) + 200, 345);
	foodText.setString("Food: " + std::to_string(simulationSettings_.numOfFood));
	window.draw(foodText);

	sf::Text predsText;
	predsText.setFont(font);
	predsText.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2) + 200, 355);
	predsText.setString("Predators: " + std::to_string(simulationSettings_.numOfPred));
	window.draw(predsText);

	sf::Text preysText;
	preysText.setFont(font);
	preysText.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2) + 200, 365);
	preysText.setString("Prey: " + std::to_string(simulationSettings_.numOfPrey));
	window.draw(preysText);

	sf::Text timeoutText;
	timeoutText.setFont(font);
	timeoutText.setPosition((windowSize.x / 2) - (mainTitleTexture->getSize().x / 2) + 200, 375);
	timeoutText.setString("Timeout: " + std::to_string(simulationSettings_.simTimeOut));
	window.draw(timeoutText);

	sf::Sprite menuSprite;
	auto menuTexture = this->findTexture("menu");
	menuSprite.setTexture(*menuTexture);
	menuSprite.setColor(sf::Color(255, 255, 255, 200));
	menuSprite.setPosition((windowSize.x / 2) - (menuTexture->getSize().x / 2), windowSize.y - 170);
	window.draw(menuSprite);

	sf::Sprite exitSprite;
	auto exitTexture = this->findTexture("exit");
	exitSprite.setTexture(*exitTexture);
	exitSprite.setColor(sf::Color(255, 255, 255, 200));
	exitSprite.setPosition((windowSize.x / 2) - (exitTexture->getSize().x / 2), windowSize.y - 100);
	window.draw(exitSprite);

	auto position = sf::Mouse::getPosition(window);
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
		(position.x > ((windowSize.x / 2) - (menuTexture->getSize().x / 2))) &&
		((position.x < windowSize.x / 2) + (menuTexture->getSize().x / 2)))
	{
		if ((position.y > windowSize.y - 170) && (position.y < (windowSize.y - 170) + menuTexture->getSize().y)) // Menu Button
		{
			simEnd = true;
			gameStarted = false;
		}
		else if ((position.y > windowSize.y - 100) && (position.y < (windowSize.y - 100) + exitTexture->getSize().y)) // Exit Button
		{
			simEnd = simQuit = true;
		}
	}

}



