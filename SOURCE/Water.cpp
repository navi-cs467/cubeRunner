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
		//3 lines of green
		bottomRow = LINES - 5;

		int obstacleCount;
		if(gameMode == HARD)
			obstacleCount = 15;
		else if(gameMode == NORMAL)
			obstacleCount = 10;
		else
			obstacleCount = 8;

		for(int i = 0, random = rand() % 4; 
			i < obstacleCount; i++, random = rand() % 4) {
				if(random == 0) {
					obstacles.push_back(new Seaweed(this));
					//updateObsCoords(obstacles.back());
				}
				else if(random == 1) {
					obstacles.push_back(new Coral(this));
					//updateObsCoords(obstacles.back());
				}
				else if(random == 2) {
					obstacles.push_back(new Shark(this));
					//updateObsCoords(obstacles.back());
				}
				else {
					obstacles.push_back(new Octopus(this));
					//updateObsCoords(obstacles.back());
				}
		}
		
		//Load miniCubes
		initMiniCubes(NUM_MCS_EASY / (gameMode));
		
		//Create offscreen obs and minCubes for scrolling
		loadOSObs();
		loadOSMCs();

		clear();  // curses clear-screen call

		//Paint all but last 4 lines of the screen blue
		attron(COLOR_PAIR(BLUE_BLUE));
		for (int y = 0; y <= bottomRow; y++)
			mvhline(y, 0, ' ', COLS);

		//Paint LINES - 4 to LINES - 1 brown (as the "ocean floor")
		attron(COLOR_PAIR(GREEN_GREEN));
		for (int y = bottomRow + 1; y < LINES - 1; y++)
			mvhline(y, 0, ' ', COLS);

		refresh();

		renderWorld();

		//Last line is reserved for life count, timer, and score display
}

void Water::loadOSObs() {
	
	int obstacleCount;
	if(gameMode == HARD)
		obstacleCount = OBS_COUNT_HARD;
	else if(gameMode == NORMAL)
		obstacleCount = OBS_COUNT_MED;
	else
		obstacleCount = OBS_COUNT_EASY;
	
	//Create offscreen obstacles for one "screens-worth"
	for(int i = 0, random = rand() % 4; 
		i < obstacleCount; i++, random = rand() % 4) {
			if(random == 0) {
				obstacles.push_back(new Seaweed(this, right));
			}
			else if(random == 1) {
				obstacles.push_back(new Coral(this, right));
			}
			else if(random == 2) {
				obstacles.push_back(new Shark(this, right));
			}
			else {
				obstacles.push_back(new Octopus(this, right));
			}
	}
}

void Water::loadOSMCs() {
	initMiniCubes(NUM_MCS_EASY / gameMode, right);
}

void Water::renderWorld() {

	//Paint blank background
	attron(COLOR_PAIR(BLUE_BLUE));
	for(int i = 0; i <= bottomRow; i++)
		mvhline(i, 0, ' ', COLS);
	attron(COLOR_PAIR(GREEN_GREEN));
	for(int i = bottomRow + 1; i < LINES - 1; i++)
		mvhline(i, 0, ' ', COLS);
	
	//Print all the miniCubes
	wchar_t mc[] = L"\u25A0";		
	//char mc = 'c';
	for(set<pair<int, int>>::iterator it = miniCubes.begin();
		it != miniCubes.end(); it++) {
			attron(COLOR_PAIR(WHITE_BLUE));
            mvaddwstr(it->first, it->second, mc); //refresh();
		}
	
	//Print all Obstacles
	for(list<Obstacle*>::iterator it = obstacles.begin();
			it != obstacles.end(); it++) {

		int xCoord = (*it)->getPosX(), yCoord = (*it)->getPosY(),

		//Used when obstacle is partially off-screen (left or top)
		//(Not needed for right or bottom because ncurses will
		// print partial sub-windows if print coordinates
		// exceed LINES or COLS)
		xOffset = 0, 			//xOffset not needed for Water
		yOffset = 0;

		//Determine offsets	if necessary (i.e. when part of graphic is off screen)
		if(xCoord < 0) xOffset = -xCoord;
		if(yCoord < 0) yOffset = -yCoord;

		//Temporary c-string used in call to mvaddstr below
		wchar_t tmpWChArr[2]; tmpWChArr[1] = '\0';
		
		if(typeid(**it) == typeid(Seaweed)) {
			attron(COLOR_PAIR(Seaweed::getColor()));
			for(int i = 0; i < (*it)->getGTS() + -xOffset &&
				i + xCoord <= bottomRow; i++)
				for(int j = 0;
					j < Seaweed::_getGraphicLines()[(*it)->getGT()][i+xOffset].length() &&
					j + yCoord + yOffset < COLS; j++) {
					
					//Continue if character is off-screen
					if(j - yOffset < 0) continue;
					
					tmpWChArr[0] = Seaweed::_getGraphicLines()[(*it)->getGT()][i+xOffset][j];
					
					//output to screen
					move(i + xCoord + xOffset, j + yCoord);
					printw("%lc", tmpWChArr[0]);
				}
		}
				
		if(typeid(**it) == typeid(Coral)) {
			int nextColor = Coral::getColorSeed();
			attron(A_BOLD);
			for(int i = 0; i < (*it)->getGTS() + -xOffset &&
				i + xCoord <= bottomRow; i++) 
				for(int j = 0; 
					j < Coral::_getGraphicLines()[(*it)->getGT()][i+xOffset].length() &&
					j + yCoord + yOffset < COLS; j++) {

					//Continue if character is off-screen
					if(j - yOffset < 0) continue;
					
					tmpWChArr[0] = Coral::_getGraphicLines()[(*it)->getGT()][i+xOffset][j];
					
					//To address a strange background color issue
					//with partially off-screen right - Coral Only
					attron(COLOR_PAIR(BLUE_BLUE));
					mvaddstr(i + xCoord + xOffset, j + yCoord, " ");
					
					//output to screen
					attron(COLOR_PAIR(nextColor++));
					move(i + xCoord + xOffset, j + yCoord);
					printw("%lc", tmpWChArr[0]);
					if(nextColor == 35) nextColor = 30;
				}
		}
				
		if(typeid(**it) == typeid(Shark)) {
			int color = Shark::getColor();
			attron(A_BOLD);
			
			for(int i = 0; i < (*it)->getGTS() + -xOffset &&
				i + xCoord <= bottomRow; i++) 
				for(int j = 0; 
					j < Shark::_getGraphicLines()[(*it)->getGT()][i+xOffset].length() &&
					j + yCoord + yOffset < COLS; j++) {
					
					//Continue if character is off-screen
					if(j - yOffset < 0) continue;
					
					tmpWChArr[0] = Shark::_getGraphicLines()[(*it)->getGT()][i+xOffset][j];
					
					//output to screen
					attron(COLOR_PAIR(color));
					move(i + xCoord + xOffset, j + yCoord);
					printw("%lc", tmpWChArr[0]);
				}
		}

		if(typeid(**it) == typeid(Octopus)) {
			int color = static_cast<Octopus *>(*it)->getColor();
			attron(A_BOLD);
			
			for(int i = 0; i < (*it)->getGTS() + -xOffset &&
				i + xCoord <= bottomRow; i++) 
				for(int j = 0; 
					j < Octopus::_getGraphicLines()[(*it)->getGT()][i+xOffset].length() &&
					j + yCoord + yOffset < COLS; j++) {
					if(j - yOffset < 0) continue;
					tmpWChArr[0] = Octopus::_getGraphicLines()[(*it)->getGT()][i+xOffset][j];
					//output to screen
					attron(COLOR_PAIR(color));
					move(i + xCoord + xOffset, j + yCoord);
					printw("%lc", tmpWChArr[0]);
				}
		}
	}

	refresh();

}

void Water::scroll_() {
	
	/* //Paint blank background
	attron(COLOR_PAIR(BLUE_BLUE));
	for(int i = 0; i <= bottomRow; i++)
		mvhline(i, 0, ' ', COLS);
	attron(COLOR_PAIR(GREEN_GREEN));
	for(int i = bottomRow + 1; i < LINES - 1; i++)
		mvhline(i, 0, ' ', COLS); */
	
	set<pair<int, int>>::iterator itObs;
	
	for(list<Obstacle*>::iterator it = obstacles.begin();
			it != obstacles.end(); it++) {
		(*it)->setPosY((*it)->getPosY() - 1);
		
		int xCoord = (*it)->getPosX(), yCoord = (*it)->getPosY();
		
		//Free memory and delete Obstacle from obstacles and
		//remove all associated obsCoords and nonWSObsCoords if 
		//Obstacle goes completely offscreen a full screen width 
		//to the left or a full-screen width above
		if((*it)->getPosY() <= -COLS || (*it)->getPosX() <= -LINES) {
			//Remove all associated obsCoords
			for(int i = 0; i <(*it)->getGTS(); i++) 
				for(int j = 0; j < (*it)->getLongestGS(); j++) {
					itObs = obsCoords.find
							(make_pair(i + xCoord, j + yCoord));
					if(itObs != obsCoords.end()) obsCoords.erase(itObs);
				}
			//Free memory
			delete *it;
			//Remove pointer from list
			obstacles.erase(it);
			//it = obstacles.begin();
			it--;
		}
	}
	
	attron(COLOR_PAIR(BLUE_BLUE));
	
	//Temporary set of pairs used to update obsCoords values via swap
	set<pair<int, int>> newObsCoords;
	for(set<pair<int, int>>::iterator it = obsCoords.begin();
		it != obsCoords.end(); it++) {
			newObsCoords.insert(make_pair(it->first, it->second - 1));
			//Set to blank background where obsCoords will be removed
			mvaddstr(it->first, it->second, " ");
	}
	swap(obsCoords, newObsCoords);
	
	//Temporary set of pairs used to update nonWSObsCoords values via swap
	set<pair<int, int>> newNonWSObsCoords;	
	for(list<Obstacle*>::iterator it = obstacles.begin();
			it != obstacles.end(); it++) { 	
		for(set<pair<int, int>>::iterator itNWObsCoords = 
				(*it)->getNonWSObsCoords().begin();
				itNWObsCoords != (*it)->getNonWSObsCoords().end(); 
				itNWObsCoords++) {
					newNonWSObsCoords.
						insert(make_pair(itNWObsCoords->first, 
							itNWObsCoords->second - 1));
		}
		swap((*it)->getNonWSObsCoords(), newNonWSObsCoords);
		newNonWSObsCoords.clear();
	}
	
	//Temporary set of pairs used to update miniCubes values via swap
	set<pair<int, int>> newMinCubes;
	for(set<pair<int, int>>::iterator it = miniCubes.begin();
		it != miniCubes.end(); it++) {
			//If miniCube won't go offscreen, add it to new set
			if(it->second >= 1)
				newMinCubes.insert(make_pair(it->first, it->second - 1));
				//Set to blank background where miniCubes will be removed
				mvaddstr(it->first, it->second, " ");
	}
	swap(miniCubes, newMinCubes);
}

// References
// https://stackoverflow.com/questions/35162938/cannot-correctly-print-extended-ascii-characters-in-ncurses
