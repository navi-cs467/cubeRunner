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
				//if(random == 0) {
				if(1) {
					obstacles.push_back(new Seaweed(this));
					updateObsCoords(obstacles.back());
				}
				/* else if(random == 1) {
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
				} */
		}

		initMiniCubes(gameMode * 20);

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

void Water::renderWorld() {

	//Clear coordinates with objects tracker
	obsCoords.clear();

	//Clear board
	for(int i = 0; i < LINES; i++)
		memset(Game::getBoard()[i], ' ', COLS * sizeof(char));

	//for(int i = 0; i <= 3; i++)
	//	for(int j = 0; j < 3; j++)
	//		Game::setBoard(i, j, ' ');


	int obsNum = 0;
	for(list<Obstacle*>::iterator it = obstacles.begin();
			it != obstacles.end(); it++, obsNum++) {

		int xCoord = (*it)->getPosX(), yCoord = (*it)->getPosY(),

			//Used when obstacle is partially off-screen (left or top)
			//(Not needed for right or bottom because ncurses will
			// print partial sub-windows if print coordinates
			// exceed LINES or COLS)
			xOffset = 0, yOffset = 0;

		//Determine offsets	if necessary (i.e. when part of graphic is off screen)
		if(xCoord < 0) xOffset = -xCoord;
		if(yCoord < 0) yOffset = -yCoord;

		//Temporary c-string used in call to mvaddstr below
		char tmpStr[2]; tmpStr[1] = '\0';

		if(typeid(**it) == typeid(Seaweed)) {
			attron(COLOR_PAIR(Seaweed::getColor()));
			for(int i = 0; i < Seaweed::getGraphicLines()[(*it)->getGT()].size() &&
				i + xCoord + xOffset <= bottomRow; i++)
				for(int j = 0;
				j < Seaweed::getGraphicLines()[(*it)->getGT()][i].length() &&
				j + yCoord + yOffset < COLS &&
				j + yCoord + yOffset >= 0; j++) {
					Game::setBoard(i + xCoord + xOffset,
								   j + yCoord + yOffset,
								   Seaweed::getGraphicLines()[(*it)->getGT()][i][j]);
					obsCoords.insert(make_pair(i + xCoord + xOffset,
											   j + yCoord + yOffset));
					tmpStr[0] = Seaweed::getGraphicLines()[(*it)->getGT()][i][j];
					//output to screen
					mvaddstr(i + xCoord + xOffset,
							 j + yCoord + yOffset,
							 tmpStr);
				}
		}

		/* if(typeid(**it) == typeid(Coral))
			for(int i = xCoord + xOffset;
				i < Coral::graphicLines.length() && i < LINES - 4; i++)
				for(int j = yCoord + yOffset;
					j < Coral::graphicLines[i].length() && j < COLS; j++) {
					Game::getBoard()[i][j] = Coral::getGraphicLines()[i][j];
					obsCoords.insert(make_pair(i, j));
				}

		if(typeid(**it) == typeid(Shark))
			for(int i = xCoord + xOffset;
				i < Shark::graphicLines.length() && i < LINES - 4; i++)
				for(int j = yCoord + yOffset;
					j < Shark::graphicLines[i].length() && j < COLS; j++) {
					Game::getBoard()[i][j] = Shark::getGraphicLines()[i][j];
					obsCoords.insert(make_pair(i, j));
				}

		if(typeid(**it) == typeid(Octopus))
			for(int i = xCoord + xOffset;
				i < Octopus::graphicLines.length() && i < LINES - 4; i++)
				for(int j = yCoord + yOffset;
					j < Octopus::graphicLines[i].length() && j < COLS; j++) {
					Game::getBoard()[i][j] = Octopus::getGraphicLines()[i][j];
					obsCoords.insert(make_pair(i, j));
				} */
	}

	//Print all the miniCubes
	wchar_t mc[]  =L"\u25A0";		//Trying to print unicode square '\u25A0'...
	//char mc = 'c';
	attron(COLOR_PAIR(BLUE_BLACK)); attron(A_BOLD);
	for(set<pair<int, int>>::iterator it = miniCubes.begin();
		it != miniCubes.end(); it++) {
			Game::setBoard(it->first, it->second,'c');	// '\254' is ascii "square"
			attron(COLOR_PAIR(BLACK_BLUE));
                        mvaddwstr(it->first, it->second, mc); //refresh();
			//move(it->first, it->second);
			printw("%lc", mc);
			//printw("%c", mc);
		}

	refresh();

	//**NOTE: scroll function needs to free memory and delete obstacles as
	//		  they leave the screen!
}

// References
// https://stackoverflow.com/questions/35162938/cannot-correctly-print-extended-ascii-characters-in-ncurses
