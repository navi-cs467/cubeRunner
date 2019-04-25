/********************************************************************
** Program name: Water.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Source file for Water class. The Water class is 
** 				one of three world types for the game. The colors
**				and obstacles are distinct for this world, along
**				with the player movement. The game "scrolls" the 
**				player along at a minimum speed (that increases
**				with game mode difficulty and elapsed time up to
**				a point) from left to right, and the cube's 
**				vertical position remains constant until modified
**				by the player.
********************************************************************/

#include "Water.hpp"

Water::Water(int gameMode, int numPlayers) : 
	World(gameMode, numPlayers) {
		int obstacleCount;
		if(gameMode == 3)
			obstacleCount = 4;
		else if(gameMode == 2)
			obstacleCount = 7;
		else
			obstacleCount = 10;
		for(int i = 0, random = rand() % 4; 
			i < obstacleCount; i++, rand = rand % 4) {
				if(random == 0) {
					obstacles.push_back(new Seaweed());
					updateObsCoords(obstacles.back());
				}
				else if(random == 1) {
					obstacles.push_back(new Coral());
					updateObsCoords(obstacles.back());
				}
				else if(random == 2) {
					obstacles.push_back(new Shark());
					updateObsCoords(obstacles.back());
				}
				else {
					obstacles.push_back(new Octopus());
					updateObsCoords(obstacles.back());
				}
		}
		
		clear();  // curses clear-screen call
	
		//Paint screen blue
		attron(COLOR_PAIR(BLUE_BLUE));
		for (int y = 0; y < LINES; y++) {
			mvhline(y, 0, ' ', COLS);
		}
		refresh();
}

void World::renderWorld() {
	
	WINDOW* subscrns[obstacles.size()];
	int obsNum = 0;
	
	for(list<Obstacle*>::iterator it; it != obstacles.end(); it++, obsNum++) {

		int xCoord = (*it)->getXCoord, yCoord = (*it)->getYCoord,
			
			//Used when obstacle is partially off-screen (left or top)
			//(Not needed for right or bottom because ncurses will 
			// print partial sub-windows if print coordinates
			// exceed LINES or COLS)
			xOffset = 0, yOffset = 0;
			
		if(xCoord < 0) xOffset = -xCoord;
		if(yCoord < 0) yOffset = -yCoord;
			
		if(typeid(**it) == typeid(Seaweed))
			for(int i = xOffset; i < Seaweed::graphicLines.length(); i++) {
				if(yCoord < 0) xOffset = -yCoord;
				subscrns[obsNum] = newwin(
				
			
		
	