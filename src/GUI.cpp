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

void GUIsim::mainScreen_(GWindow& Gwin)
{
	//Main Window Title
	Gwin.drawImage(((Gwin.getWidth() / 2) - mainTitle->getWidth() / 2), 10, mainTitle);

	//draw menu images
	Gwin.drawImage(((Gwin.getWidth() / 2) - (begin->getWidth() / 2)), 200, begin);
	Gwin.drawImage(((Gwin.getWidth() / 2) - (options->getWidth() / 2)), 260, options);
	Gwin.drawImage(((Gwin.getWidth() / 2) - (credits->getWidth() / 2)), 320, credits);

	Gwin.drawImage(100, 500, _Prey2);
	Gwin.drawImage(Gwin.getWidth() - 250, 500, _Pred2);

}
void GUIsim::creditScreen_(GWindow& Gwin)
{
	//Main Window Title
	Gwin.drawImage(((Gwin.getWidth() / 2) - mainTitle->getWidth() / 2), 10, mainTitle);
	Gwin.drawImage(((Gwin.getWidth() / 2) - table->getWidth() / 2), 210, table);

	Gwin.setPenColour(WHITE);
	for (unsigned int idx = 0; idx < teamAndRoles_.size(); ++idx)
	{
		Gwin.writeString((Gwin.getWidth() / 2) - 175, 300 + (20 * idx), teamAndRoles_[idx].first);
		Gwin.writeString((Gwin.getWidth() / 2), 300 + (20 * idx), teamAndRoles_[idx].second);
	}

	Gwin.drawImage(((Gwin.getWidth() / 2) - (back->getWidth() / 2)), Gwin.getHeight() - 100, back);
}

void GUIsim::optionScreen_(GWindow& Gwin)
{
	//Main Window Title
	Gwin.drawImage(((Gwin.getWidth() / 2) - mainTitle->getWidth() / 2), 10, mainTitle);

	//GRID SIZE
	Gwin.setPenColour(BLACK);
	Gwin.writeText(Gwin.getWidth() / 2 - 100, 200, "Grid Size");
	Gwin.writeInt(Gwin.getWidth() / 2, 200, simulationSettings_.gridSize);
	Gwin.setPenColour(GRAY);
	Gwin.rectangle(Gwin.getWidth() / 2 - 100, 220, Gwin.getWidth() / 2 + 100, 230);
	Gwin.setPenColour(BLACK);
	Gwin.rectangle((Gwin.getWidth() / 2 - 100) + simulationSettings_.gridSize, 215, (Gwin.getWidth() / 2 - 100) + simulationSettings_.gridSize + 10, 235);

	//OBSTACLES
	Gwin.setPenColour(BLACK);
	Gwin.writeText(Gwin.getWidth() / 2 - 100, 250, "Obstacles");
	Gwin.writeInt(Gwin.getWidth() / 2, 250, simulationSettings_.numOfObstacles);
	Gwin.setPenColour(GRAY);
	Gwin.rectangle(Gwin.getWidth() / 2 - 100, 270, Gwin.getWidth() / 2 + 100, 280);
	Gwin.setPenColour(BLACK);
	Gwin.rectangle((Gwin.getWidth() / 2 - 100) + simulationSettings_.numOfObstacles, 265, (Gwin.getWidth() / 2 - 100) + simulationSettings_.numOfObstacles + 10, 285);

	//FOOD
	Gwin.setPenColour(BLACK);
	Gwin.writeText(Gwin.getWidth() / 2 - 100, 300, "Food");
	Gwin.writeInt(Gwin.getWidth() / 2, 300, simulationSettings_.numOfFood);
	Gwin.setPenColour(GRAY);
	Gwin.rectangle(Gwin.getWidth() / 2 - 100, 320, Gwin.getWidth() / 2 + 100, 330);
	Gwin.setPenColour(BLACK);
	Gwin.rectangle((Gwin.getWidth() / 2 - 100) + simulationSettings_.numOfFood, 315, (Gwin.getWidth() / 2 - 100) + simulationSettings_.numOfFood + 10, 335);

	//PREDATORS
	Gwin.setPenColour(BLACK);
	Gwin.writeText(Gwin.getWidth() / 2 - 100, 350, "Predators");
	Gwin.writeInt(Gwin.getWidth() / 2, 350, simulationSettings_.numOfPred);
	Gwin.setPenColour(GRAY);
	Gwin.rectangle(Gwin.getWidth() / 2 - 100, 370, Gwin.getWidth() / 2 + 100, 380);
	Gwin.setPenColour(BLACK);
	Gwin.rectangle((Gwin.getWidth() / 2 - 100) + simulationSettings_.numOfPred, 365, (Gwin.getWidth() / 2 - 100) + simulationSettings_.numOfPred + 10, 385);

	//PREY
	Gwin.setPenColour(BLACK);
	Gwin.writeText(Gwin.getWidth() / 2 - 100, 400, "Prey");
	Gwin.writeInt(Gwin.getWidth() / 2, 400, simulationSettings_.numOfPrey);
	Gwin.setPenColour(GRAY);
	Gwin.rectangle(Gwin.getWidth() / 2 - 100, 420, Gwin.getWidth() / 2 + 100, 430);
	Gwin.setPenColour(BLACK);
	Gwin.rectangle((Gwin.getWidth() / 2 - 100) + simulationSettings_.numOfPrey, 415, (Gwin.getWidth() / 2 - 100) + simulationSettings_.numOfPrey + 10, 435);

	//TIME OUT
	Gwin.setPenColour(BLACK);
	Gwin.writeText(Gwin.getWidth() / 2 - 100, 450, "Time Out x 100");
	Gwin.writeInt(Gwin.getWidth() / 2, 450, simulationSettings_.simTimeOut);
	Gwin.setPenColour(GRAY);
	Gwin.rectangle(Gwin.getWidth() / 2 - 100, 470, Gwin.getWidth() / 2 + 100, 480);
	Gwin.setPenColour(BLACK);
	Gwin.rectangle((Gwin.getWidth() / 2 - 100) + simulationSettings_.simTimeOut, 465, (Gwin.getWidth() / 2 - 100) + simulationSettings_.simTimeOut + 10, 485);

	//INTELLIGENCE
	Gwin.setPenColour(BLACK);
	Gwin.writeText(Gwin.getWidth() / 2 - 100, 500, "Smart AI?");
	Gwin.setPenColour(BLACK);
	Gwin.rectangle(Gwin.getWidth() / 2 - 100, 520, Gwin.getWidth() / 2 + 100, 530);
	if (simulationSettings_.intelligence == true)
	{
		Gwin.setPenColour(RED);
		Gwin.outlineRectangle(Gwin.getWidth() / 2 - 100, 520, Gwin.getWidth() / 2 + 100, 530);
		Gwin.writeText(Gwin.getWidth() / 2, 500, "ON");
	}
	else
		Gwin.writeText(Gwin.getWidth() / 2, 500, "OFF");


	//draw save button
	Gwin.drawImage(((Gwin.getWidth() / 2) - (save->getWidth() / 2)), Gwin.getHeight() - 100, save);

	if (!Mouse.isLeftDown())
		isToggled_ = true;

	if (Mouse.isLeftDown() && (Mouse.x > Gwin.getWidth() / 2 - 96) && (Mouse.x < Gwin.getWidth() / 2 + 106))
	{
		if ((Mouse.y > 200) && (Mouse.y < 250)) // Grid Size
			simulationSettings_.gridSize = (Mouse.x - 5 - (Gwin.getWidth() / 2 - 100));
		else if ((Mouse.y > 250) && (Mouse.y < 300)) // Obstacles
			simulationSettings_.numOfObstacles = (Mouse.x - 5 - (Gwin.getWidth() / 2 - 100));
		else if ((Mouse.y > 300) && (Mouse.y < 350)) // Food
			simulationSettings_.numOfFood = (Mouse.x - 5 - (Gwin.getWidth() / 2 - 100));
		else if ((Mouse.y > 350) && (Mouse.y < 400)) // Predator
			simulationSettings_.numOfPred = (Mouse.x - 5 - (Gwin.getWidth() / 2 - 100));
		else if ((Mouse.y > 400) && (Mouse.y < 450)) // Prey
			simulationSettings_.numOfPrey = (Mouse.x - 5 - (Gwin.getWidth() / 2 - 100));
		else if ((Mouse.y > 450) && (Mouse.y < 500)) // Prey
			simulationSettings_.simTimeOut = (Mouse.x - 5 - (Gwin.getWidth() / 2 - 100));
		else if (((Mouse.y > 500) && (Mouse.y < 550)) && (isToggled_))
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

void GUIsim::endGameScreen(GWindow& Gwin, std::pair<unsigned int, unsigned int> entities, int iterationCount)
{
	Gwin.drawImage(0, 0, background);
	//Main Window Title
	Gwin.drawImage(((Gwin.getWidth() / 2) - (mainTitle->getWidth() / 2)), 10, mainTitle);
	Gwin.drawImage(((Gwin.getWidth() / 2) - table->getWidth() / 2), 200, table);

	Gwin.setPenColour(WHITE);
	Gwin.writeText(((Gwin.getWidth() / 2) - (mainTitle->getWidth() / 2)), 300, "Simulation Results");

	if (entities.first == 0)
	{
		Gwin.writeInt(((Gwin.getWidth() / 2) - (mainTitle->getWidth() / 2)), 325, entities.second);
		Gwin.writeText(" Predators remained");
		Gwin.writeText(((Gwin.getWidth() / 2) - (mainTitle->getWidth() / 2)), 350, "last Prey killed at: ");
	}
	else if (entities.second == 0)
	{
		Gwin.writeInt(((Gwin.getWidth() / 2) - (mainTitle->getWidth() / 2)), 325, entities.first);
		Gwin.writeText(" Prey Survived");
		Gwin.writeText(((Gwin.getWidth() / 2) - (mainTitle->getWidth() / 2)), 350, "last Predator died at: ");
	}
	else
	{
		Gwin.writeInt(((Gwin.getWidth() / 2) - (mainTitle->getWidth() / 2)), 325, entities.first);
		Gwin.writeText(" Prey Survived");
		Gwin.writeInt(((Gwin.getWidth() / 2) - (mainTitle->getWidth() / 2)), 350, entities.second);
		Gwin.writeText(" Predators remained");
	}

	Gwin.writeText(((Gwin.getWidth() / 2) - (mainTitle->getWidth() / 2)), 375, "Game Time: ");
	Gwin.writeInt(iterationCount);	Gwin.writeText(" iterations");

	Gwin.writeText(((Gwin.getWidth() / 2) - (mainTitle->getWidth() / 2) + 200), 300, "Simulation Settings");
	Gwin.writeText(((Gwin.getWidth() / 2) - (mainTitle->getWidth() / 2) + 200), 325, "GridSize: ");
	Gwin.writeInt(simulationSettings_.gridSize);
	Gwin.writeText(((Gwin.getWidth() / 2) - (mainTitle->getWidth() / 2) + 200), 335, "Obstacles: ");
	Gwin.writeInt(simulationSettings_.numOfObstacles);
	Gwin.writeText(((Gwin.getWidth() / 2) - (mainTitle->getWidth() / 2) + 200), 345, "Food: ");
	Gwin.writeInt(simulationSettings_.numOfFood);
	Gwin.writeText(((Gwin.getWidth() / 2) - (mainTitle->getWidth() / 2) + 200), 355, "Predator: ");
	Gwin.writeInt(simulationSettings_.numOfPred);
	Gwin.writeText(((Gwin.getWidth() / 2) - (mainTitle->getWidth() / 2) + 200), 365, "Prey: ");
	Gwin.writeInt(simulationSettings_.numOfPrey);
	Gwin.writeText(((Gwin.getWidth() / 2) - (mainTitle->getWidth() / 2) + 200), 375, "Time Out: ");
	Gwin.writeInt(simulationSettings_.simTimeOut);

	//draw save and restart button
	Gwin.drawImage(((Gwin.getWidth() / 2) - (save->getWidth() / 2)), Gwin.getHeight() - 170, menu);
	//draw save and exit button
	Gwin.drawImage(((Gwin.getWidth() / 2) - (save->getWidth() / 2)), Gwin.getHeight() - 100, exit);

	if (Mouse.isLeftDown() && (Mouse.x > ((Gwin.getWidth() / 2) - (menu->getWidth() / 2))) && ((Mouse.x < Gwin.getWidth() / 2) + (menu->getWidth() / 2)))
	{
		if ((Mouse.y > Gwin.getHeight() - 170) && (Mouse.y < (Gwin.getHeight() - 170) + menu->getHeight())) // Menu Button
		{
			simEnd = true;
			gameStarted = false;
		}
		else if ((Mouse.y > Gwin.getHeight() - 100) && (Mouse.y < (Gwin.getHeight() - 100) + exit->getHeight())) // Exit Button
		{
			simEnd = simQuit = true;
		}
	}

}



