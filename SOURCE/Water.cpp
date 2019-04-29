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

#include "../HEADER/Water.hpp"

Water::Water(int gameMode, bool isTwoPlayer) : 
	World(gameMode, isTwoPlayer) {
		//Set bottom row to LINES - 5, to allow for 
		//score, life count, and timer display +
		//3 lines of brown
		bottomRow = LINES - 5;
		
		int obstacleCount;
		if(gameMode == 3)
			obstacleCount = 4;
		else if(gameMode == 2)
			obstacleCount = 7;
		else
			obstacleCount = 10;
		for(int i = 0, random = rand() % 4; 
			i < obstacleCount; i++) {
				//if(random == 0) {
				if(1) {
					obstacles.push_back(new Seaweed(this));
					updateObsCoords(obstacles.back());
				}
				else if(random == 1) {
					obstacles.push_back(new Coral(this));
					updateObsCoords(obstacles.back());
				}
				else if(random == 2) {
					obstacles.push_back(new Shark(this));
					updateObsCoords(obstacles.back());
				}
				else {
					obstacles.push_back(new Octopus(this));
					updateObsCoords(obstacles.back());
				}
				rand = rand % 4;
		}
		
		//Load miniCubes here, making sure not to encroach on
		//obstacles...
		
		clear();  // curses clear-screen call
	
		//Paint all but last 4 lines of the screen blue
		attron(COLOR_PAIR(BLUE_BLUE));
		for (int y = 0; y < LINES - 4; y++) 
			mvhline(y, 0, ' ', COLS);
		
		//Paint LINES - 4 to LINES - 1 brown (as the "ocean floor")
		attron(COLOR_PAIR(BROWN_BROWN));
		for (int y = LINES - 4; y < LINES - 1; y++)
			mvhline(y, 0, ' ', COLS);
		
		renderWorld();
		
		refresh();
		
		//Last line is reserved for life count, timer, and score display
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
				i < Seaweed::graphicLines.length() && i < LINES - 4; i++) 
				for(j = yCoord + yOffset; 
					j < Seaweed::graphicLines[i].length() && j < COLS; j++) {
					Game::getBoard()[i][j] = Seaweed::graphicLines[i][j];
					obsCoords.insert(make_pair(i, j));
				}
				
		if(typeid(**it) == typeid(Coral))
			for(int i = xCoord + xOffset; 
				i < Coral::graphicLines.length() && i < LINES - 4; i++) 
				for(j = yCoord + yOffset; 
					j < Coral::graphicLines[i].length() && j < COLS; j++) {
					Game::getBoard()[i][j] = Coral::graphicLines[i][j];
					obsCoords.insert(make_pair(i, j));
				}
				
		if(typeid(**it) == typeid(Shark))
			for(int i = xCoord + xOffset; 
				i < Shark::graphicLines.length() && i < LINES - 4; i++) 
				for(j = yCoord + yOffset; 
					j < Shark::graphicLines[i].length() && j < COLS; j++) {
					Game::getBoard()[i][j] = Shark::graphicLines[i][j];
					obsCoords.insert(make_pair(i, j));
				}
				
		if(typeid(**it) == typeid(Octopus))
			for(int i = xCoord + xOffset; 
				i < Octopus::graphicLines.length() && i < LINES - 4; i++) 
				for(j = yCoord + yOffset; 
					j < Octopus::graphicLines[i].length() && j < COLS; j++) {
					Game::getBoard()[i][j] = Octopus::graphicLines[i][j];
					obsCoords.insert(make_pair(i, j));
				}
	}
		
	//Print all the miniCubes
	for(set<pair<int, int>::iterator it = miniCubes.begin();
		it != miniCubes.end(); it++)
		Game::board[*it.first][*it.second] = '\254';	// '\254' is ascii "square"
		
	//Temp string to hold single character
	char tmpStr[2]; tempStr[1] = '\0';
	
	//Output board to screen
	for(int i = 0; i < LINES - 1; i++)
		for(int j =0; j < COLS; j++) {
			tmpStr[0] = Game::board[i][j];
			mvaddr(i, j, tmpStr);
		}
	refresh();
	
	//**NOTE: scroll function needs to free memory and delete obstacles as
	//		  they leave the screen!
}
			
		
	