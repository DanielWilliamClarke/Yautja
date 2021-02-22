#include "gwin.h"
#include <iostream>
#include <fstream>
#include "segment.h"

#define BUTTONWIDTH 250
#define BUTTONHEIGHT 50

enum ScreenTypes {
	mainScreen = 0,
	optionScreen,
	creditScreen,
};

struct simSettings {
	unsigned int gridSize;
	unsigned int numOfObstacles;
	unsigned int numOfPred;
	unsigned int numOfPrey;
	unsigned int numOfFood;
	unsigned int simTimeOut;
	bool intelligence;
};


class GUIsim
{
public:
	//std::ofstream logFile_;
	bool isComplete(std::pair<unsigned int, unsigned int> entities, int iterationCount);
	bool gameStarted;
	bool simEnd;
	bool simQuit;
	GUIsim();
	void mouseEvent(int mouseX, int mouseY, GWindow& Gwin);
	simSettings getSettings();
	void drawMenu(GWindow & Gwin);
	unsigned int statusBar(GWindow & Gwin);
	void endGameScreen(GWindow& Gwin, std::pair<unsigned int, unsigned int> entities, int iterationCount);
	void legend(GWindow& Gwin);
	void outPut(std::pair<unsigned int, unsigned int> entities, int iterationCount);
	
private:
	ScreenTypes currentScreen_;
	simSettings simulationSettings_;
	unsigned int sleepSliderV_;
	unsigned int defeatIteration_;
	std::vector< std::pair<std::string, std::string> > teamAndRoles_;

	void mainScreen_(GWindow& Gwin);
	void optionScreen_(GWindow& Gwin);
	void creditScreen_(GWindow& Gwin);
	
	bool gameEnd();

	GImage *background;
	GImage *begin;
	GImage *options;
	GImage *credits;
	GImage *mainTitle;
	GImage *save;
	GImage *menu;
	GImage *exit;
	GImage *table;
	GImage *back;
	GImage *done;

	GImage *_Prey;
	GImage *_Pred;
	GImage *_Prey2;
	GImage *_Pred2;
	GImage *_Berry;
	GImage *_Rock;

	bool isToggled_;
};
	