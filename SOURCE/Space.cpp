/********************************************************************
** Program name: Space.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Source file for Space class. The Space class is
** 				one of three world types for the game. The colors
**				and obstacles are distinct for this world, along
**				with the player movement. The game "scrolls" the
**				player along at a minimum speed (that increases
**				with game mode difficulty and elapsed time up to
**				a point) from left to right, and the cube's
**				vertical position remains constant until modified
**				by the player.
********************************************************************/

#include "../HEADER/Space.hpp"

Space::Space(int gameMode, bool isTwoPlayer, bool forServer) :
	World(gameMode, isTwoPlayer, forServer) {
		//Set bottom row to LINES - 2, to allow for
		//score, life count, and timer display @ LINES - 1
		bottomRow = LINES - 2;

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
					obstacles.push_back(new Asteroid(this));
					//updateObsCoords(obstacles.back());
				}
				else if(random == 1) {
					obstacles.push_back(new Planet(this));
					//updateObsCoords(obstacles.back());
				}
				else if(random == 2) {
					obstacles.push_back(new Comet(this));
					//updateObsCoords(obstacles.back());
				}
				else {
					obstacles.push_back(new Spaceship(this));
					//updateObsCoords(obstacles.back());
				}
		}
		
		//Load miniCubes
		initMiniCubes(NUM_MCS_EASY / (gameMode));
		
		//Create offscreen obs and minCubes for scrolling
		loadOSObs(right);
		loadOSMCs(right);

		if(!forServer) {
			clear();  // curses clear-screen call

			//Paint all but last 4 lines of the screen blue
			attron(COLOR_PAIR(BLACK_BLACK));
			for (int y = 0; y <= bottomRow; y++)
				mvhline(y, 0, ' ', COLS);

			//Paint LINES - 4 to LINES - 1 brown (as the "ocean floor")
			attron(COLOR_PAIR(GREEN_GREEN));
			for (int y = bottomRow + 1; y < LINES - 1; y++)
				mvhline(y, 0, ' ', COLS);

			//refresh();

			//renderWorld();
		}

		//Last line is reserved for life count, timer, and score display
}

void Space::loadOSObs(Direction dir) {
	
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
				obstacles.push_back(new Asteroid(this, dir));
			}
			else if(random == 1) {
				obstacles.push_back(new Planet(this, dir));
			}
			else if(random == 2) {
				obstacles.push_back(new Comet(this, dir));
			}
			else {
				obstacles.push_back(new Spaceship(this, dir));
			}
	}
}

void Space::loadOSMCs(Direction dir) {
	initMiniCubes(NUM_MCS_EASY / gameMode, dir);
}

void Space::renderWorld(Cube *cube) {

	//clear();
	//refresh();

	//Paint blank background
	attron(COLOR_PAIR(BLACK_BLACK));
	for(int i = 0; i <= bottomRow; i++)
		mvhline(i, 0, ' ', COLS);
	
	//Print all the miniCubes
	wchar_t mc[] = L"\u25A0";		
	//char mc = 'c';
	for(set<pair<int, int>>::iterator it = miniCubes.begin();
		it != miniCubes.end(); it++) {
			attron(COLOR_PAIR(WHITE_BLACK));
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
		xOffset = 0, 			//xOffset not needed for Space
		yOffset = 0;

		//Determine offsets	if necessary (i.e. when part of graphic is off screen)
		if(xCoord < 0) xOffset = -xCoord;
		if(yCoord < 0) yOffset = -yCoord;

		//Temporary c-string used in call to mvaddstr below
		wchar_t tmpWChArr[2]; tmpWChArr[1] = '\0';
		
		if(typeid(**it) == typeid(Asteroid)) {
			for(int i = 0; i < (*it)->getGTS() + -xOffset &&
				i + xCoord <= bottomRow; i++)
				for(int j = 0;
					j < Asteroid::_getGraphicLines()[(*it)->getGT()][i+xOffset].length() &&
					j + yCoord + yOffset < COLS; j++) {
					
					//Continue if character is off-screen
					if(j - yOffset < 0) continue;
					
					tmpWChArr[0] = Asteroid::_getGraphicLines()[(*it)->getGT()][i+xOffset][j];
					
					//output to screen
					move(i + xCoord + xOffset, j + yCoord);
					if((*it)->getHoles().
						find(make_pair(i + xCoord + xOffset, j + yCoord)) == 
							(*it)->getHoles().end()) {
								attron(COLOR_PAIR(Asteroid::getColor()));
								printw("%lc", tmpWChArr[0]);
					}
					else {
						attron(COLOR_PAIR(BLACK_BLACK));
						printw("%lc", " ");
					}
				}
		}
		
		else if(typeid(**it) == typeid(Planet)) {
			int nextColor = Planet::getColorSeed();
			attron(A_BOLD);
			for(int i = 0; i < (*it)->getGTS() + -xOffset &&
				i + xCoord <= bottomRow; i++) 
				for(int j = 0; 
					j < Planet::_getGraphicLines()[(*it)->getGT()][i+xOffset].length() &&
					j + yCoord + yOffset < COLS; j++) {

					//Continue if character is off-screen
					if(j - yOffset < 0) continue;
					
					tmpWChArr[0] = Planet::_getGraphicLines()[(*it)->getGT()][i+xOffset][j];
					
					//To address a strange background color issue
					//with partially off-screen right - Planet Only
					attron(COLOR_PAIR(BLACK_BLACK));
					mvaddstr(i + xCoord + xOffset, j + yCoord, " ");
					
					//output to screen
					move(i + xCoord + xOffset, j + yCoord);
					if((*it)->getHoles().find(make_pair(i + xCoord + xOffset, j + yCoord)) == 
					   (*it)->getHoles().end()) {
						attron(COLOR_PAIR(nextColor++));
						printw("%lc", tmpWChArr[0]);
					}
					else {
						attron(COLOR_PAIR(BLACK_BLACK));
						printw("%lc", " ");
					}
					if(nextColor == 6) nextColor = 1;
				}
		}
			
		else if(typeid(**it) == typeid(Comet)) {
			int color = static_cast<Comet*>(*it)->getColor();
			attron(A_BOLD);
			if((*it)->getHits()) {
				attron(COLOR_PAIR(RED_BLACK));
			}
			else {
				attron(COLOR_PAIR(color));
			}
			
			for(int i = 0; i < (*it)->getGTS() + -xOffset &&
				i + xCoord <= bottomRow; i++) 
				for(int j = 0; 
					j < Comet::_getGraphicLines()[(*it)->getGT()][i+xOffset].length() &&
					j + yCoord + yOffset < COLS; j++) {
					
					//Continue if character is off-screen
					if(j - yOffset < 0) continue;
					
					tmpWChArr[0] = Comet::_getGraphicLines()[(*it)->getGT()][i+xOffset][j];
					
					//Print remaining hits counter in first whitespace available
					//if Obstacle->hits > 0
					if((*it)->getHits() && i == 0 && j == 0 && 
						xOffset == 0 && yOffset == 0) {
						move(i + xCoord + xOffset, j + yCoord);
						attron(COLOR_PAIR(WHITE_BLACK));
						printw("%lc", (*it)->getMaxHits() - (*it)->getHits() + '0');
						attron(COLOR_PAIR(RED_BLACK));
					}
					//output to screen
					else {
						move(i + xCoord + xOffset, j + yCoord);
						printw("%lc", tmpWChArr[0]);
					}
				}
		}
		
		else if(typeid(**it) == typeid(Spaceship)) {
			int color = Spaceship::getColor();
			attron(A_BOLD);
			if((*it)->getHits()) {
				attron(COLOR_PAIR(RED_BLACK));
			}
			else {
				attron(COLOR_PAIR(color));
			}
			
			for(int i = 0; i < (*it)->getGTS() + -xOffset &&
				i + xCoord <= bottomRow; i++) 
				for(int j = 0; 
					j < Spaceship::_getGraphicLines()[(*it)->getGT()][i+xOffset].length() &&
					j + yCoord + yOffset < COLS; j++) {
					if(j - yOffset < 0) continue;
					tmpWChArr[0] = Spaceship::_getGraphicLines()[(*it)->getGT()][i+xOffset][j];
					
					//Print remaining hits counter in first whitespace available
					//if Obstacle->hits > 0
					if((*it)->getHits() && i == 0 && j == 0 &&
						xOffset == 0 && yOffset == 0) {
						move(i + xCoord + xOffset, j + yCoord);
						attron(COLOR_PAIR(WHITE_BLACK));
						printw("%lc", (*it)->getMaxHits() - (*it)->getHits() + '0');
						attron(COLOR_PAIR(RED_BLACK));
					}
					//output to screen
					else {
						move(i + xCoord + xOffset, j + yCoord);
						printw("%lc", tmpWChArr[0]);
					}					
				}
		} 
	}
	cube->processShot(); 
	
	//refresh();

}

void Space::scroll_(Cube *cube) {
	
	/* //Paint blank background
	attron(COLOR_PAIR(BLACK_BLACK));
	for(int i = 0; i <= bottomRow; i++)
		mvhline(i, 0, ' ', COLS); */
	
	set<pair<int, int>>::iterator itObs;
	
	for(list<Obstacle*>::iterator it = obstacles.begin();
			it != obstacles.end(); it++) {
		if(cube->getCubeDirection() == right ||
		   cube->getCubeDirection() == right_up ||
		   cube->getCubeDirection() == right_down)
			(*it)->setPosY((*it)->getPosY() - 1);
		else if(cube->getCubeDirection() == left ||
				cube->getCubeDirection() == left_up ||
				cube->getCubeDirection() == left_down)
			(*it)->setPosY((*it)->getPosY() + 1);
		else if(cube->getCubeDirection() == up)
			(*it)->setPosY((*it)->getPosX() + 1);
		else
			(*it)->setPosY((*it)->getPosY() - 1);
		
		int xCoord = (*it)->getPosX(), yCoord = (*it)->getPosY();
		
		//Free memory and delete Obstacle from obstacles and
		//remove all associated obsCoords and nonWSObsCoords if 
		//Obstacle goes completely offscreen 2 full screen widths 
		//to the left, right, above or below
		if((*it)->getPosY() <= -COLS * 2 ||
		   (*it)->getPosY() >= COLS * 2 ||
		   (*it)->getPosX() <= -LINES * 2 ||
		   (*it)->getPosX() <= -LINES * 2) {
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
	
	attron(COLOR_PAIR(BLACK_BLACK));
	
	//Temporary set of pairs used to update obsCoords values via swap
	set<pair<int, int>> newObsCoords;
	for(set<pair<int, int>>::iterator it = obsCoords.begin();
		it != obsCoords.end(); it++) {
			if(cube->getCubeDirection() == right ||
			   cube->getCubeDirection() == right_up ||
			   cube->getCubeDirection() == right_down)
				newObsCoords.insert(make_pair(it->first, it->second - 1));
			else if(cube->getCubeDirection() == left ||
					cube->getCubeDirection() == left_up ||
					cube->getCubeDirection() == left_down)
				newObsCoords.insert(make_pair(it->first, it->second + 1));
			else if(cube->getCubeDirection() == up)
				newObsCoords.insert(make_pair(it->first + 1, it->second));
			else
				newObsCoords.insert(make_pair(it->first - 1, it->second));
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
					if(cube->getCubeDirection() == right ||
					   cube->getCubeDirection() == right_up ||
					   cube->getCubeDirection() == right_down)
						newNonWSObsCoords.
							insert(make_pair(itNWObsCoords->first, 
								itNWObsCoords->second - 1));
					else if(cube->getCubeDirection() == left ||
							cube->getCubeDirection() == left_up ||
							cube->getCubeDirection() == left_down)
						newNonWSObsCoords.
							insert(make_pair(itNWObsCoords->first, 
								itNWObsCoords->second + 1));
					else if(cube->getCubeDirection() == up)
						newNonWSObsCoords.
							insert(make_pair(itNWObsCoords->first + 1, 
								itNWObsCoords->second));
					else
						newNonWSObsCoords.
							insert(make_pair(itNWObsCoords->first - 1, 
								itNWObsCoords->second));
		}
		swap((*it)->getNonWSObsCoords(), newNonWSObsCoords);
		newNonWSObsCoords.clear();
	}
	
	//Temporary set of pairs used to update holes values via swap
	set<pair<int, int>> newHoles;	
	for(list<Obstacle*>::iterator it = obstacles.begin();
			it != obstacles.end(); it++) { 	
		for(set<pair<int, int>>::iterator itHoles = 
				(*it)->getHoles().begin();
				itHoles != (*it)->getHoles().end(); 
				itHoles++) {
					if(cube->getCubeDirection() == right ||
					   cube->getCubeDirection() == right_up ||
					   cube->getCubeDirection() == right_down)
						newHoles.
							insert(make_pair(itHoles->first, 
								itHoles->second - 1));
					else if(cube->getCubeDirection() == left ||
							cube->getCubeDirection() == left_up ||
							cube->getCubeDirection() == left_down)
						newHoles.
							insert(make_pair(itHoles->first, 
								itHoles->second + 1));
					else if(cube->getCubeDirection() == up)
						newHoles.
							insert(make_pair(itHoles->first + 1, 
								itHoles->second));
					else
						newHoles.
							insert(make_pair(itHoles->first - 1, 
								itHoles->second));
		}
		swap((*it)->getHoles(), newHoles);
		newHoles.clear();
	}
	
	//Temporary set of pairs used to update miniCubes values via swap
	set<pair<int, int>> newMiniCubes;
	for(set<pair<int, int>>::iterator itMiniCubes = miniCubes.begin();
		itMiniCubes != miniCubes.end(); itMiniCubes++) {
			//if miniCube is 2 screen widths offscreen, erase and continue
			if(itMiniCubes->second <= -COLS * 2 ||
			   itMiniCubes->second >= COLS * 2 ||
			   itMiniCubes->first <= -LINES * 2 ||
			   itMiniCubes->first <= -LINES * 2) {
				   miniCubes.erase(itMiniCubes--);
				   continue;
			   }
			if(cube->getCubeDirection() == right ||
			   cube->getCubeDirection() == right_up ||
			   cube->getCubeDirection() == right_down)
				newMiniCubes.
					insert(make_pair(itMiniCubes->first, 
						itMiniCubes->second - 1));
			else if(cube->getCubeDirection() == left ||
					cube->getCubeDirection() == left_up ||
					cube->getCubeDirection() == left_down)
				newMiniCubes.
					insert(make_pair(itMiniCubes->first, 
						itMiniCubes->second + 1));
			else if(cube->getCubeDirection() == up)
				newMiniCubes.
					insert(make_pair(itMiniCubes->first + 1, 
						itMiniCubes->second));
			else
				newMiniCubes.
					insert(make_pair(itMiniCubes->first - 1, 
						itMiniCubes->second));
			//Set to blank background where miniCube used to be
			mvaddstr(itMiniCubes->first, itMiniCubes->second, " ");
	}
	swap(miniCubes, newMiniCubes);
}

// References
// https://stackoverflow.com/questions/35162938/cannot-correctly-print-extended-ascii-characters-in-ncurses
// https://stackoverflow.com/questions/2279379/how-to-convert-integer-to-char-in-c 
