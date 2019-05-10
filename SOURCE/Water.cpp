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
		if(gameMode == 3)
			obstacleCount = 10;
		else if(gameMode == 2)
			obstacleCount = 15;
		else
			obstacleCount = 25;

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
		initMiniCubes(gameMode * NUM_MCS_HARD);
		
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
	if(gameMode == 3)
		obstacleCount = OBS_COUNT_EASY;
	else if(gameMode == 2)
		obstacleCount = OBS_COUNT_MED;
	else
		obstacleCount = OBS_COUNT_HARD;
	
	//Create offscreen obstacles for one "screens-worth"
	for(int i = 0, random = rand() % 4; 
		i < obstacleCount; i++, random = rand() % 4) {
			if(random == 0) {
				obstacles.push_back(new Seaweed(this, right));
				//updateObsCoords(obstacles.back());
			}
			else if(random == 1) {
				obstacles.push_back(new Coral(this, right));
				//updateObsCoords(obstacles.back());
			}
			else if(random == 2) {
				obstacles.push_back(new Shark(this, right));
				//updateObsCoords(obstacles.back());
			}
			else {
				obstacles.push_back(new Octopus(this, right));
				//updateObsCoords(obstacles.back());
			}
	}
}

void Water::loadOSMCs() {
	initMiniCubes(gameMode * NUM_MCS_HARD, right);
}

void Water::renderWorld() {

	//Clear coordinates with objects tracker
	//obsCoords.clear();

	/* //Clear board
	for(int i = 0; i < LINES; i++)
		memset(Game::getBoard()[i], ' ', COLS * sizeof(char)); */

	//for(int i = 0; i <= 3; i++)
	//	for(int j = 0; j < 3; j++)
	//		Game::setBoard(i, j, ' ');

	//Paint blank background
	attron(COLOR_PAIR(BLUE_BLUE));
	for(int i = 0; i <= bottomRow; i++)
		mvhline(i, 0, ' ', COLS);
	attron(COLOR_PAIR(GREEN_GREEN));
	for(int i = bottomRow + 1; i < LINES - 1; i++)
		mvhline(i, 0, ' ', COLS);

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
					j < Seaweed::getGraphicLines()[(*it)->getGT()][i+xOffset].length() &&
					j + yCoord + yOffset < COLS; j++) {
					/* Game::setBoard(i + xCoord + xOffset,
								   j + yCoord + yOffset,
								   Seaweed::getGraphicLines()[(*it)->getGT()][i][j]); */
					/* obsCoords.insert(make_pair(i + xCoord + xOffset,
											   j + yCoord + yOffset)); */
					if(j - yOffset < 0) continue;
					tmpWChArr[0] = Seaweed::getGraphicLines()[(*it)->getGT()][i+xOffset][j];
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
					j < Coral::getGraphicLines()[(*it)->getGT()][i+xOffset].length() &&
					j + yCoord + yOffset < COLS; j++) {
					/* Game::setBoard(i + xCoord + xOffset, 
								   j + yCoord + yOffset,
								   Coral::getGraphicLines()[(*it)->getGT()][i][j]); */
					/* obsCoords.insert(make_pair(i + xCoord + xOffset, 
											   j + yCoord + yOffset)); */
					if(j - yOffset < 0) continue;
					tmpWChArr[0] = Coral::getGraphicLines()[(*it)->getGT()][i+xOffset][j];
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
			
			/* //Face Shark based on movement direction
			if((*it)->getDirection() == left ||
			   (*it)->getDirection() == left_down ||
			   (*it)->getDirection() == left_up)
					(*it)->setGT(1);			//Shark::graphicLines[1] = sharkLeft
			else if((*it)->getDirection() == right ||
			   (*it)->getDirection() == right_down ||
			   (*it)->getDirection() == right_up)
					(*it)->setGT(0);			//Shark::graphicLines[0] = sharkRight
			//else if(cube->getPosY() <= (*it)->getPosY())
			//	(*it)->setGT(1);				//Shark::graphicLines[1] = sharkLeft
			else
				(*it)->setGT(0);				//Shark::graphicLines[0] = sharkRight */
			
			
			for(int i = 0; i < (*it)->getGTS() + -xOffset &&
				i + xCoord <= bottomRow; i++) 
				for(int j = 0; 
					j < Shark::getGraphicLines()[(*it)->getGT()][i+xOffset].length() &&
					j + yCoord + yOffset < COLS; j++) {
					/* Game::setBoard(i + xCoord + xOffset, 
								   j + yCoord + yOffset,
								   Shark::getGraphicLines()[(*it)->getGT()][i][j]); */
					/* obsCoords.insert(make_pair(i + xCoord + xOffset, 
											   j + yCoord + yOffset)); */
					if(j - yOffset < 0) continue;
					//tmpStr[0] = Shark::getGraphicLines()[(*it)->getGT()][i][j];
					tmpWChArr[0] = Shark::getGraphicLines()[(*it)->getGT()][i+xOffset][j];
					//output to screen
					attron(COLOR_PAIR(color));
					move(i + xCoord + xOffset, j + yCoord);
					printw("%lc", tmpWChArr[0]);
				}
		}

		if(typeid(**it) == typeid(Octopus)) {
			int color = static_cast<Octopus *>(*it)->getColor();
			attron(A_BOLD);
			
			/* //Face Octopus based on movement direction
			if((*it)->getDirection() == left ||
			   (*it)->getDirection() == left_down ||
			   (*it)->getDirection() == left_up)
					(*it)->setGT(1);			//Octopus::graphicLines[1] = sharkLeft
			else if((*it)->getDirection() == right ||
			   (*it)->getDirection() == right_down ||
			   (*it)->getDirection() == right_up)
					(*it)->setGT(0);			//Octopus::graphicLines[0] = sharkRight
			//else if(cube->getPosY() <= (*it)->getPosY())
			//	(*it)->setGT(1);			//Octopus::graphicLines[1] = sharkLeft
			else
				(*it)->setGT(0);			//Octopus::graphicLines[0] = sharkRight */
			
			
			for(int i = 0; i < (*it)->getGTS() + -xOffset &&
				i + xCoord <= bottomRow; i++) 
				for(int j = 0; 
					j < Octopus::getGraphicLines()[(*it)->getGT()][i+xOffset].length() &&
					j + yCoord + yOffset < COLS; j++) {
					/* Game::setBoard(i + xCoord + xOffset, 
								   j + yCoord + yOffset,
								   Octopus::getGraphicLines()[(*it)->getGT()][i][j]); */
					/* obsCoords.insert(make_pair(i + xCoord + xOffset, 
											   j + yCoord + yOffset)); */
					if(j - yOffset < 0) continue;
					tmpWChArr[0] = Octopus::getGraphicLines()[(*it)->getGT()][i+xOffset][j];
					//output to screen
					attron(COLOR_PAIR(color));
					move(i + xCoord + xOffset, j + yCoord);
					printw("%lc", tmpWChArr[0]);
				}
		}
	}

	//Print all the miniCubes
	wchar_t mc[] = L"\u25A0";		
	//char mc = 'c';
	for(set<pair<int, int>>::iterator it = miniCubes.begin();
		it != miniCubes.end(); it++) {
			//Game::setBoard(it->first, it->second,'c');	// '\254' is ascii "square"
			attron(COLOR_PAIR(WHITE_BLUE));
            mvaddwstr(it->first, it->second, mc); //refresh();
			//move(it->first, it->second);
			//printw("%lc", mc);
			//printw("%c", mc);
		}

	refresh();

	//**NOTE: scroll function needs to free memory and delete obstacles as
	//		  they leave the screen!
}

//void Water::scroll_(bool newObs) {
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
		//remove all associated obsCoords if Obstacle goes
		//completely offscreen a full screen width to the left
		//or a full-screen width above
		if((*it)->getPosY() <= -COLS) {
			//Remove all associated obsCoords
			for(int i = 0; i <(*it)->getGTS(); i++) 
				for(int j = 0; j < (*it)->getLongestGS(); j++) 
					itObs = obsCoords.find
							(make_pair(i + xCoord, j + yCoord));
					if(itObs != obsCoords.end()) obsCoords.erase(itObs);
			//Free memory
			delete *it;
			//Remove pointer from list
			obstacles.erase(it);
			it = obstacles.begin();
		}
		else if((*it)->getPosX() <= -LINES) {
			//Remove all associated obsCoords
			for(int i = 0; i < (*it)->getGTS(); i++) 
				for(int j = 0; j <(*it)->getLongestGS(); j++) 
					itObs = obsCoords.find
							(make_pair(i + xCoord, j + yCoord));
					if(itObs != obsCoords.end()) obsCoords.erase(itObs);
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
			//Do not keep any obsCoord that is a full screen size
			//offscreen to the left or a full screen width offscreen
			//above
			//if(it->second > -COLS && it->first > -LINES)
			newObsCoords.insert(make_pair(it->first, it->second - 1));
			//Set to blank background where obsCoords will be removed
			mvaddstr(it->first, it->second, " ");
	}
	swap(obsCoords, newObsCoords);
	
	
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
	
	/* if(newObs) {
		int random = rand() % 4;
		if(random == 0) {
			obstacles.push_back(new Seaweed(this, right));
			//updateObsCoords(obstacles.back());
		}
		else if(random == 1) {
			obstacles.push_back(new Coral(this, right));
			//updateObsCoords(obstacles.back());
		}
		else if(random == 2) {
			obstacles.push_back(new Shark(this, right));
			//updateObsCoords(obstacles.back());
		}
		else {
			obstacles.push_back(new Octopus(this, right));
			//updateObsCoords(obstacles.back());
		}
	} */
}

// References
// https://stackoverflow.com/questions/35162938/cannot-correctly-print-extended-ascii-characters-in-ncurses
