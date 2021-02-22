// Filename: main.cpp
// Author: Alex Rudd
// ID: N0321342
// Version: 1.0
// Date: 30/11/2011
// Description: main
#include "stdafx.h"
#include "gwin.h"
#include "environment.h"
#include "GUI.h"

int main()
{
	GWindow Gwin(800, 700);
	Gwin.showDebugOverlay(false);
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
			Gwin.clear();
			menu.drawMenu(Gwin);

			if(Mouse.isLeftDown())
				menu.mouseEvent(Mouse.x, Mouse.y, Gwin);

			Gwin.refresh();
		}

		PreyPref.smartAI = menu.getSettings().intelligence; //set AI Toggle
		level = new Environment(menu.getSettings().gridSize, menu.getSettings().numOfFood, menu.getSettings().numOfObstacles, menu.getSettings().numOfPrey, menu.getSettings().numOfPred, PreyPref, PredPref, Gwin);
		iterationCount = 0;
		
		while(!menu.isComplete(level->getEntities(), iterationCount))
		{
			Gwin.clear(DIMGRAY);

			level->iterateSimulation(iterationCount++);
			level->drawSimulation(Gwin);
			menu.legend(Gwin);

			Gsleep(menu.statusBar(Gwin));

			Gwin.setPenColour(WHITE);
			Gwin.writeText(10, Gwin.getHeight()-50, "Iteration:");
			Gwin.writeInt(60, Gwin.getHeight()-50, iterationCount);
			Gwin.writeString("/");
			Gwin.writeInt(menu.getSettings().simTimeOut*100);
			Gwin.setPenColour(BLUE);

			Gwin.refresh();
		}

		while(!menu.simEnd)
		{
			Gwin.clear();
			menu.endGameScreen(Gwin, level->getEntities(), iterationCount);

			Gwin.refresh();
		}
		delete level;
	}
	return 0;
}