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

void Water::renderWorld() {
	
	//Clear coordinates with objects tracker
	obsCoords.clear();
	
	//Clear board
	for(int i = 0; i < LINES; i++) 
		memset(Game::board[i], ' ', COLS * sizeof(int));
			
	
	int obsNum = 0;
	for(list<Obstacle*>::iterator it; it != obstacles.end(); it++, obsNum++) {

		int xCoord = (*it)->getXCoord, yCoord = (*it)->getYCoord,
			
			//Used when obstacle is partially off-screen (left or top)
			//(Not needed for right or bottom because ncurses will 
			// print partial sub-windows if print coordinates
			// exceed LINES or COLS)
			xOffset = 0, yOffset = 0;
			
		//Determine offsets	if necessary (i.e. part of graphic is off screen)
		if(xCoord < 0) xOffset = -xCoord;
		if(yCoord < 0) yOffset = -yCoord;
		
		if(typeid(**it) == typeid(Seaweed))
			for(int i = xCoord + xOffset; 
				i < Seaweed::graphicLines.length() && i < LINES - 1; i++) 
				for(j = yCoord + yOffset; 
					j < Seaweed::graphicLines[i].length() && j < COLS; j++) {
					Game::getBoard()[i][j] = Seaweed::graphicLines[i][j];
					obsCoords.insert(make_pair(i, j));
				}
				
		if(typeid(**it) == typeid(Coral))
			for(int i = xCoord + xOffset; 
				i < Coral::graphicLines.length() && i < LINES - 1; i++) 
				for(j = yCoord + yOffset; 
					j < Coral::graphicLines[i].length() && j < COLS; j++) {
					Game::getBoard()[i][j] = Coral::graphicLines[i][j];
					obsCoords.insert(make_pair(i, j));
				}
				
		if(typeid(**it) == typeid(Shark))
			for(int i = xCoord + xOffset; 
				i < Shark::graphicLines.length() && i < LINES - 1; i++) 
				for(j = yCoord + yOffset; 
					j < Shark::graphicLines[i].length() && j < COLS; j++) {
					Game::getBoard()[i][j] = Shark::graphicLines[i][j];
					obsCoords.insert(make_pair(i, j));
				}
				
		if(typeid(**it) == typeid(Octopus))
			for(int i = xCoord + xOffset; 
				i < Octopus::graphicLines.length() && i < LINES - 1; i++) 
				for(j = yCoord + yOffset; 
					j < Octopus::graphicLines[i].length() && j < COLS; j++) {
					Game::getBoard()[i][j] = Octopus::graphicLines[i][j];
					obsCoords.insert(make_pair(i, j));
				}
	}
		
	//Print all the miniCubes
	for(set<pair<int, int>::iterator it = miniCubes.begin();
		it != miniCubes.end(); it++)
		Game::board[*it.first][*it.second] = '/254';
		
	//Temp string to hold single character
	char tmpStr[2]; tempStr[1] = '/0';
	
	//Output board to screen
	for(int i = 0; i < LINES - 1; i++)
		for(int j =0; j < COLS; j++) {
			tmpStr[0] = Game::board[i][j];
			mvaddr(i, j, tmpStr);
		}
	refresh();
}
			
		
	