/********************************************************************
** Program name: Cube.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 05/12/2019
** Description: Source file for Cube class. The Token class manages player
** 			token movement, displaying of token and death animation,
**			updating player score, and checking for player death conditions.
********************************************************************/


#include <iostream>
#include <fstream>
#include <ncurses.h>
#include "../HEADER/Cube.hpp"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <set>
#include "../HEADER/using.hpp"


Cube::Cube(World *world, int lives) :
currWorld(world), lives(lives) {
	col = 2;
	row = (LINES - (LINES - world->getBottomRow())) / 2;
	isDead = 0;
	score = 0;
	transitionScore = 0;
	currWorld = world;
	importCubeImage();
	initializeCubeCoords();
	curDir = right;
	useLeftCube = 0;
	color = WHITE_BLUE;
	highlightColor = RED_BLUE;
	shotColor = RED_BLUE;
	shotOff = false;
	shotCoords.first = -1; shotCoords.second = -1;

}

void Cube::importCubeImage(void){
	/* fstream inFile;
	string inLine;
	int i = 0, j = 0, k = 0;

	//Left Cube
	inFile.open("gameCubeLeft.txt", ios::in); //sleep(50);

	if(inFile.is_open()){
		k = 0;
		j = 0;
		while(getline(inFile, inLine)){
			for(i = 0; i < inLine.length(); ++i){
				cubeLeftChars[j][i]  = inLine[i];
			}
			++j;
		}
		inFile.close();
	} */

	//Left Cube
	cubeLeftChars[0][0] = ' ';
	cubeLeftChars[0][1] = '_';
	cubeLeftChars[0][2] = '_';
	cubeLeftChars[0][3] = '_';
	cubeLeftChars[0][4] = ' ';
	cubeLeftChars[1][0] = '/';
	cubeLeftChars[1][1] = '_';
	cubeLeftChars[1][2] = '_';
	cubeLeftChars[1][3] = '/';
	cubeLeftChars[1][4] = '\\';
	cubeLeftChars[2][0] = '\\';
	cubeLeftChars[2][1] = '_';
	cubeLeftChars[2][2] = '_';
	cubeLeftChars[2][3] = '\\';
	cubeLeftChars[2][4] = '/';


	/* //Right Cube
	inFile.open("gameCubeRight.txt", ios::in);

	if(inFile.is_open()){
		k = 0;
		j = 0;
		while(getline(inFile, inLine)){
			for(i = 0; i < inLine.length(); ++i){
				cubeRightChars[j][i]  = inLine[i];
			}
			++j;
		}
		inFile.close();
	} */

	//Right Cube
	cubeRightChars[0][0] = ' ';
	cubeRightChars[0][1] = '_';
	cubeRightChars[0][2] = '_';
	cubeRightChars[0][3] = '_';
	cubeRightChars[0][4] = ' ';
	cubeRightChars[1][0] = '/';
	cubeRightChars[1][1] = '\\';
	cubeRightChars[1][2] = '_';
	cubeRightChars[1][3] = '_';
	cubeRightChars[1][4] = '\\';
	cubeRightChars[2][0] = '\\';
	cubeRightChars[2][1] = '/';
	cubeRightChars[2][2] = '_';
	cubeRightChars[2][3] = '_';
	cubeRightChars[2][4] = '/';
}

void Cube::initializeCubeCoords(void){
	int i = 0, j = 0, k = 0;

	k = 0; //Coord Number
	for(i = 0; i < 3; ++i){ //for each row
		for(j = 0; j < 5; ++j){ //for each column
			cubeCoords[k][0] = row + i;  //set row number
			cubeCoords[k][1] = col + j;  //set column number
			++k;
		}
	}
}

void Cube::transitionWorld(World *world) {
	currWorld = world;
	shotOff = false;
	if(typeid(*world) == typeid(Water)) {
		color = WHITE_BLUE;
		highlightColor = RED_BLUE;
		shotColor = RED_BLUE;
	}
	if(typeid(*world) == typeid(Land)) {
		color = BLACK_WHITE;
		highlightColor = RED_WHITE;
		shotColor = RED_WHITE;
	}
	if(typeid(*world) == typeid(Space)) {
		color = WHITE_BLACK;
		highlightColor = RED_BLACK;
		shotColor = RED_BLACK;
	}
	shotCoords.first = -1; shotCoords.second = -1;
}

void Cube::cubeReset(World *world){
	col = 0;
	row = 0;
	isDead = 0;

	importCubeImage();

	//Update row and col
	row = cubeCoords[0][0] = (LINES - (LINES - world->getBottomRow())) / 2;
	col = cubeCoords[0][1] = 2;

	initializeCubeCoords();
	useLeftCube = 0;
	
	curDir = right;

	//Initialize shotOff and shotCoords
	shotOff = false;
	shotCoords.first = -1; shotCoords.second = -1;
}

//Update the cube token position based on input by the user(s)
void Cube::updateCubePosition(bool colInc, bool colDec, bool rowInc, bool rowDec,
							  bool userInputInitiated){
	int colIncInt = 0, colDecInt = 0, rowIncInt = 0, rowDecInt = 0;

	if(colInc == 1){
		colIncInt = 1;
	}
	if(colDec == 1){
		colDecInt = 1;
	}
	if(rowInc == 1 && row + cubeHeight <= currWorld->getBottomRow()){
		rowIncInt = 1;
	}
	if(rowDec == 1){
		rowDecInt = 1;
	}

	//Update curDir member
	if(userInputInitiated) {
		if(colInc == 0 && colDec == 1 && rowInc == 0 && rowDec == 0 ){
			prevDir = curDir;
			curDir = left;
			useLeftCube = 1;
		}
		else if(colInc == 1 && colDec == 0 && rowInc == 0 && rowDec == 0 ){
			prevDir = curDir;
			curDir = right;
			useLeftCube = 0;
		}
		else if(colInc == 0 && colDec == 0 && rowInc == 0 && rowDec == 1 ){
			prevDir = curDir;
			curDir = up;
		}
		else if(colInc == 0 && colDec == 0 && rowInc == 1 && rowDec == 0 ){
			prevDir = curDir;
			curDir = down;
		}
		else if(colInc == 0 && colDec == 1 && rowInc == 0 && rowDec == 1 ){
			prevDir = curDir;
			curDir = left_up;
			useLeftCube = 1;
		}
		else if(colInc == 0 && colDec == 1 && rowInc == 1 && rowDec == 0 ){
			prevDir = curDir;
			curDir = left_down;
			useLeftCube = 1;
		}
		else if(colInc == 1 && colDec == 0 && rowInc == 0 && rowDec == 1 ){
			prevDir = curDir;
			curDir = right_up;
			useLeftCube = 0;
		}
		else if(colInc == 1 && colDec == 0 && rowInc == 1 && rowDec == 0 ){
			prevDir = curDir;
			curDir = right_down;
			useLeftCube = 0;
		}
		else{ //no user directional input
			curDir = none;
		}
	}

	updateCubePositionHelper(colDecInt, colIncInt, rowIncInt, rowDecInt);
}

void Cube::updateCubePositionHelper(int colDec, int colInc, int rowInc, int rowDec){

	int colPrev = 0, rowPrev = 0;
	colPrev = col;
	rowPrev = row;
	col = col + colInc - colDec;
	row = row + rowInc - rowDec;
	Cube::updateCubeCoords(colPrev, col, rowPrev, row);
}



int Cube::updateCubeCoords(int colPrev, int colCurr, int rowPrev, int rowCurr){
	int colChange = 0, rowChange = 0;
	int i = 0, j = 0;

	colChange = colCurr - colPrev;
	rowChange = rowCurr - rowPrev;

	for(i = 0; i < CUBE_COORDS_HEIGHT; ++i){
		cubeCoords[i][0] = cubeCoords[i][0] + rowChange;
		cubeCoords[i][1] = cubeCoords[i][1] + colChange;

	}


	return 0;
}

void Cube::drawCube(void){

	attron(COLOR_PAIR(color));
	attron(A_BOLD);

	//Use right cube
	if(useLeftCube == 0){
		move(cubeCoords[0][0],cubeCoords[0][1]); printw("%c", cubeRightChars[0][0]);
		move(cubeCoords[1][0],cubeCoords[1][1]); printw("%c", cubeRightChars[0][1]);
		move(cubeCoords[2][0],cubeCoords[2][1]); printw("%c", cubeRightChars[0][2]);
		move(cubeCoords[3][0],cubeCoords[3][1]); printw("%c", cubeRightChars[0][3]);
		move(cubeCoords[4][0],cubeCoords[4][1]); printw("%c", cubeRightChars[0][4]);
		move(cubeCoords[5][0],cubeCoords[5][1]); printw("%c", cubeRightChars[1][0]);
		move(cubeCoords[6][0],cubeCoords[6][1]); printw("%c", cubeRightChars[1][1]);
		move(cubeCoords[7][0],cubeCoords[7][1]); printw("%c", cubeRightChars[1][2]);
		move(cubeCoords[8][0],cubeCoords[8][1]); printw("%c", cubeRightChars[1][3]);
		move(cubeCoords[9][0],cubeCoords[9][1]); printw("%c", cubeRightChars[1][4]);
		move(cubeCoords[10][0],cubeCoords[10][1]); printw("%c", cubeRightChars[2][0]);
		move(cubeCoords[11][0],cubeCoords[11][1]); printw("%c", cubeRightChars[2][1]);
		move(cubeCoords[12][0],cubeCoords[12][1]); printw("%c", cubeRightChars[2][2]);
		move(cubeCoords[13][0],cubeCoords[13][1]); printw("%c", cubeRightChars[2][3]);
		move(cubeCoords[14][0],cubeCoords[14][1]); printw("%c", cubeRightChars[2][4]);
	}

	//Use left cube
	else {
		move(cubeCoords[0][0],cubeCoords[0][1]); printw("%c", cubeLeftChars[0][0]);
		move(cubeCoords[1][0],cubeCoords[1][1]); printw("%c", cubeLeftChars[0][1]);
		move(cubeCoords[2][0],cubeCoords[2][1]); printw("%c", cubeLeftChars[0][2]);
		move(cubeCoords[3][0],cubeCoords[3][1]); printw("%c", cubeLeftChars[0][3]);
		move(cubeCoords[4][0],cubeCoords[4][1]); printw("%c", cubeLeftChars[0][4]);
		move(cubeCoords[5][0],cubeCoords[5][1]); printw("%c", cubeLeftChars[1][0]);
		move(cubeCoords[6][0],cubeCoords[6][1]); printw("%c", cubeLeftChars[1][1]);
		move(cubeCoords[7][0],cubeCoords[7][1]); printw("%c", cubeLeftChars[1][2]);
		move(cubeCoords[8][0],cubeCoords[8][1]); printw("%c", cubeLeftChars[1][3]);
		move(cubeCoords[9][0],cubeCoords[9][1]); printw("%c", cubeLeftChars[1][4]);
		move(cubeCoords[10][0],cubeCoords[10][1]); printw("%c", cubeLeftChars[2][0]);
		move(cubeCoords[11][0],cubeCoords[11][1]); printw("%c", cubeLeftChars[2][1]);
		move(cubeCoords[12][0],cubeCoords[12][1]); printw("%c", cubeLeftChars[2][2]);
		move(cubeCoords[13][0],cubeCoords[13][1]); printw("%c", cubeLeftChars[2][3]);
		move(cubeCoords[14][0],cubeCoords[14][1]); printw("%c", cubeLeftChars[2][4]);
	}

	//Add Direction Color Highlight
	if(curDir != none){
		//start_color();
		//init_pair(1, COLOR WHITE, COLOR_BLACK);
		attron(COLOR_PAIR(highlightColor));

		if((curDir == right) || (curDir == right_up) || (curDir == right_down)){ //right side highlight
			//mvaddch(cubeCoords[9][0],cubeCoords[9][1],cubeRightChars[1][4]);
			move(cubeCoords[9][0],cubeCoords[9][1]); printw("%c", cubeRightChars[1][4]);
			//mvaddch(cubeCoords[14][0],cubeCoords[14][1],cubeRightChars[2][4]);
			move(cubeCoords[14][0],cubeCoords[14][1]); printw("%c", cubeRightChars[2][4]);
		}
		if((curDir == left) || (curDir == left_up) || (curDir == left_down)){ //left side highlight
			//mvaddch(cubeCoords[5][0],cubeCoords[5][1],cubeLeftChars[1][0]);
			move(cubeCoords[5][0],cubeCoords[5][1]); printw("%c", cubeRightChars[1][0]);
			//mvaddch(cubeCoords[10][0],cubeCoords[10][1],cubeLeftChars[2][0]);
			move(cubeCoords[10][0],cubeCoords[10][1]); printw("%c", cubeRightChars[2][0]);
		}
		if((curDir == up) || (curDir == right_up) || (curDir == left_up)){ //top highlight
			//mvaddch(cubeCoords[1][0],cubeCoords[1][1],cubeRightChars[0][1]);
			move(cubeCoords[1][0],cubeCoords[1][1]); printw("%c", cubeRightChars[0][1]);
			//mvaddch(cubeCoords[2][0],cubeCoords[2][1],cubeRightChars[0][2]);
			move(cubeCoords[2][0],cubeCoords[2][1]); printw("%c", cubeRightChars[0][2]);
			//mvaddch(cubeCoords[3][0],cubeCoords[3][1],cubeRightChars[0][3]);
			move(cubeCoords[3][0],cubeCoords[3][1]); printw("%c", cubeRightChars[0][3]);
		}

		if((curDir == down) || (curDir == right_down) || (curDir == left_down)){ //bottom highlight
			if(useLeftCube == 0){
				//mvaddch(cubeCoords[12][0],cubeCoords[12][1],cubeRightChars[2][2]);
				move(cubeCoords[12][0],cubeCoords[12][1]); printw("%c", cubeRightChars[2][2]);
				//mvaddch(cubeCoords[13][0],cubeCoords[13][1],cubeRightChars[2][3]);
				move(cubeCoords[13][0],cubeCoords[13][1]); printw("%c", cubeRightChars[2][3]);
			}
			else{
				//mvaddch(cubeCoords[11][0],cubeCoords[11][1],cubeLeftChars[2][1]);
				//move(cubeCoords[11][0],cubeCoords[11][1]); printw("%c", cubeRightChars[2][1]);
				move(cubeCoords[11][0],cubeCoords[11][1]); printw("%c", cubeLeftChars[2][1]);
				//mvaddch(cubeCoords[12][0],cubeCoords[12][1],cubeLeftChars[2][2]);
				//move(cubeCoords[12][0],cubeCoords[12][1]); printw("%c", cubeRightChars[2][2]);
				move(cubeCoords[12][0],cubeCoords[12][1]); printw("%c", cubeLeftChars[2][2]);

			}
		}
	}
} 

void Cube::drawCubeDeath(int *userInput, int obCollisionType){

	attron(A_BOLD);
	attron(COLOR_PAIR(BLACK_BLACK));
	mvhline(LINES - 1, 0, ' ', COLS);
	attron(COLOR_PAIR(YELLOW_BLACK));
	if(obCollisionType) {
		if(obCollisionType == 1)
			mvaddstr(LINES - 1, 15, "Death! Seaweed Collision. (Press Enter to Continue)");
		else if(obCollisionType == 2)
			mvaddstr(LINES - 1, 15, "Death! Coral Collision. (Press Enter to Continue)");
		else if(obCollisionType == 3)
			mvaddstr(LINES - 1, 15, "Death! Shark Attack. (Press Enter to Continue)");
		else if(obCollisionType == 4)
			mvaddstr(LINES - 1, 15, "Death! Octopus Attack. (Press Enter to Continue)");
		else if(obCollisionType == 5)
			mvaddstr(LINES - 1, 15, "Death! Tree Collision. (Press Enter to Continue)");
		else if(obCollisionType == 6)
			mvaddstr(LINES - 1, 15, "Death! Rock Collision. (Press Enter to Continue)");
		else if(obCollisionType == 7)
			mvaddstr(LINES - 1, 15, "Death! Bird Attack. (Press Enter to Continue)");
		else if(obCollisionType == 8)
			mvaddstr(LINES - 1, 15, "Death! Bat Attack. (Press Enter to Continue)");
		else if(obCollisionType == 9)
			mvaddstr(LINES - 1, 15, "Death! Asteroid Collision. (Press Enter to Continue)");
		else if(obCollisionType == 10)
			mvaddstr(LINES - 1, 15, "Death! Planet Collision. (Press Enter to Continue)");
		else if(obCollisionType == 11)
			mvaddstr(LINES - 1, 15, "Death! Comet Collision. (Press Enter to Continue)");
		else if(obCollisionType == 12)
			mvaddstr(LINES - 1, 15, "Death! Spaceship Collision. (Press Enter to Continue)");
	}
	else {
		if(typeid(*lastObCollided) == typeid(Seaweed))
			mvaddstr(LINES - 1, 15, "Death! Seaweed Collision. (Press Enter to Continue)");
		else if(typeid(*lastObCollided) == typeid(Coral))
			mvaddstr(LINES - 1, 15, "Death! Coral Collision. (Press Enter to Continue)");
		else if(typeid(*lastObCollided) == typeid(Shark))
			mvaddstr(LINES - 1, 15, "Death! Shark Attack. (Press Enter to Continue)");
		else if(typeid(*lastObCollided) == typeid(Octopus))
			mvaddstr(LINES - 1, 15, "Death! Octopus Attack. (Press Enter to Continue)");
		else if(typeid(*lastObCollided) == typeid(Tree))
			mvaddstr(LINES - 1, 15, "Death! Tree Collision. (Press Enter to Continue)");
		else if(typeid(*lastObCollided) == typeid(Rock))
			mvaddstr(LINES - 1, 15, "Death! Rock Collision. (Press Enter to Continue)");
		else if(typeid(*lastObCollided) == typeid(Bird))
			mvaddstr(LINES - 1, 15, "Death! Bird Attack. (Press Enter to Continue)");
		else if(typeid(*lastObCollided) == typeid(Bat))
			mvaddstr(LINES - 1, 15, "Death! Bat Attack. (Press Enter to Continue)");
		else if(typeid(*lastObCollided) == typeid(Asteroid))
			mvaddstr(LINES - 1, 15, "Death! Asteroid Collision. (Press Enter to Continue)");
		else if(typeid(*lastObCollided) == typeid(Planet))
			mvaddstr(LINES - 1, 15, "Death! Planet Collision. (Press Enter to Continue)");
		else if(typeid(*lastObCollided) == typeid(Comet))
			mvaddstr(LINES - 1, 15, "Death! Comet Collision. (Press Enter to Continue)");
		else if(typeid(*lastObCollided) == typeid(Spaceship))
			mvaddstr(LINES - 1, 15, "Death! Spaceship Collision. (Press Enter to Continue)");
	}
	refresh();

	int i = 0, j = 0, k = 0;

	// start_color();
	// init_pair(1, COLOR_RED, COLOR_BLACK);
	attron(COLOR_PAIR(RED_BLACK));

	//So animation isn't skipped if last user input is enter
	*userInput = 0;

	//Death Part 1 - small ring around token
	for(i = (col-1); i <= (col+5); ++i){ //draw top/bottom sides
		mvaddch((row-1),i,'@');
		mvaddch((row+3),i,'@');
	}
	for(i = row; i <= (row+2); ++i){ //draw left/right sides
		mvaddch(i,(col-1),'@');
		mvaddch(i,(col+5),'@');
	}
	refresh();
	for(int i = 0; *userInput != 10 &&
				   *userInput != 'q' &&
				   *userInput != 'Q' &&
				   *userInput != KEY_END &&
				   *userInput != 27 && i < 500000 / 100; i++)
		usleep(50);

	attroff(COLOR_PAIR(RED_BLACK));
	// init_pair(2, COLOR_YELLOW, COLOR_BLACK);
	attron(COLOR_PAIR(YELLOW_BLACK));

	//Death Part 2 - larger ring around token
	for(i = (col-2); i <= (col+6); ++i){ //draw top/bottom sides
		mvaddch((row-2),i,'@');
		mvaddch((row+4),i,'@');
	}
	for(i = (row-1); i <= (row+3); ++i){ //draw left/right sides
		mvaddch(i,(col-2),'@');
		mvaddch(i,(col+6),'@');
	}
	refresh();
	for(int i = 0; *userInput != 10 &&
				   *userInput != 'q' &&
				   *userInput != 'Q' &&
				   *userInput != KEY_END &&
				   *userInput != 27 && i < 450000 / 100; i++)
		usleep(50);

	attroff(COLOR_PAIR(YELLOW_BLACK));
	// init_pair(3, COLOR_GREEN, COLOR_BLACK);
	attron(COLOR_PAIR(GREEN_BLACK));


	//Death Part 3 - first broken ring around token
	for(i = (col-3); i <= (col+7); (i=i+2)){ //draw top/bottom sides
		mvaddch((row-3),i,'@');
		mvaddch((row+5),i,'@');
	}
	for(i = (row-1); i <= (row+3); (i=i+2)){ //draw left/right sides
		mvaddch(i,(col-3),'@');
		mvaddch(i,(col+7),'@');
	}
	refresh();
	for(int i = 0; *userInput != 10 &&
				   *userInput != 'q' &&
				   *userInput != 'Q' &&
				   *userInput != KEY_END &&
				   *userInput != 27 && i < 400000 / 100; i++)
		usleep(50);

	attroff(COLOR_PAIR(GREEN_BLACK));
	// init_pair(4, COLOR_BLUE, COLOR_BLACK);
	attron(COLOR_PAIR(BLUE_BLACK));


	//Death Part 4 - second broken ring around token
	for(i = (col-4); i <= (col+8); (i=i+2)){ //draw top/bottom sides
		mvaddch((row-4),i,'@');
		mvaddch((row+6),i,'@');
	}
	for(i = (row-2); i <= (row+4); (i=i+2)){ //draw left/right sides
		mvaddch(i,(col-4),'@');
		mvaddch(i,(col+8),'@');
	}
	refresh();
	for(int i = 0; *userInput != 10 &&
				   *userInput != 'q' &&
				   *userInput != 'Q' &&
				   *userInput != KEY_END &&
				   *userInput != 27 && i < 350000 / 100; i++)
		usleep(50);

	attroff(COLOR_PAIR(BLUE_BLACK));
	// init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
	attron(COLOR_PAIR(MAGENTA_BLACK));

	//Death Part 5 - third broken ring around token
	for(i = (col-5); i <= (col+9); (i=i+2)){ //draw top/bottom sides
		mvaddch((row-5),i,'@');
		mvaddch((row+7),i,'@');
	}
	for(i = (row-3); i <= (row+5); (i=i+2)){ //draw left/right sides
		mvaddch(i,(col-5),'@');
		mvaddch(i,(col+9),'@');
	}
	refresh();
	for(int i = 0; *userInput != 10 &&
				   *userInput != 'q' &&
				   *userInput != 'Q' &&
				   *userInput != KEY_END &&
				   *userInput != 27 && i < 300000 / 100; i++)
		usleep(50);

	attroff(COLOR_PAIR(MAGENTA_BLACK));
	// init_pair(6, COLOR_CYAN, COLOR_BLACK);
	attron(COLOR_PAIR(CYAN_BLACK));

	//Death Part 6 - fourth broken ring around token (shorter)
	for(i = (col-3); i <= (col+7); (i=i+2)){ //draw top/bottom sides
		mvaddch((row-6),i,'@');
		mvaddch((row+8),i,'@');
	}
	for(i = (row-4); i <= (row+6); (i=i+2)){ //draw left/right sides
		mvaddch(i,(col-6),'@');
		mvaddch(i,(col+10),'@');
	}
	refresh();
	for(i = (col-2); i <= (col+6); (i=i+2)){ //draw top/bottom sides
		mvaddch((row-7),i,'@');
		mvaddch((row+9),i,'@');
	}
	for(i = (row-3); i <= (row+5); (i=i+2)){ //draw left/right sides
		mvaddch(i,(col-7),'@');
		mvaddch(i,(col+11),'@');
	}
	refresh();

	for(int i = 0; *userInput != 10 &&
				   *userInput != 'q' &&
				   *userInput != 'Q' &&
				   *userInput != KEY_END &&
				   *userInput != 27 && i < 300000 / 100; i++)
		usleep(50);

	attroff(COLOR_PAIR(CYAN_BLACK));
	// init_pair(7, COLOR_WHITE, COLOR_BLACK);
	attron(COLOR_PAIR(WHITE_BLACK));

	for(k = 0; k < 5; ++k){
		//Token Flash Part 1 - blank
		for(i = row; i <= (row+2); ++i){
			for(j = col; j <= (col+4); ++j){
				mvaddch(i,j,' ');
			}
		}
		refresh();
		for(int i = 0; *userInput != 10 &&
					   *userInput != 'q' &&
					   *userInput != 'Q' &&
					   *userInput != KEY_END &&
					   *userInput != 27 && i < 500000 / 100; i++)
			usleep(50);

		//Token Flash Part 2 - appear
		for(i = row; i <= (row+2); ++i){
			for(j = col; j <= (col+4); ++j){
				mvaddch(i,j,'X');
			}
		}
		refresh();
		for(int i = 0; *userInput != 10 &&
					   *userInput != 'q' &&
					   *userInput != 'Q' &&
					   *userInput != KEY_END &&
					   *userInput != 27 && i < 500000 / 100; i++)
			usleep(50);
	}

	attroff(COLOR_PAIR(WHITE_BLACK));

	//Block here until user presses Enter key
	while(*userInput != 10 &&
		  *userInput != 'q' &&
		  *userInput != 'Q' &&
		  *userInput != KEY_END &&
		  *userInput != 27){}

	//Clear Enter key prompt
	attron(COLOR_PAIR(BLACK_BLACK));
	mvhline(LINES - 1, 15, ' ', 75);
	refresh();

	if(currWorld->getIsTwoPlayer()) {
		attron(COLOR_PAIR(YELLOW_BLACK));
		mvprintw(LINES - 1, 15, "Waiting for other player to press enter...");
		refresh();
	}
}

int Cube::checkCubeCollision(World *world){

	list<Obstacle*>::iterator obs;
	set<pair<int,int> >::iterator mcs, nonWSObs;

	//Check MiniCube Collisions
	for(mcs = world->getMiniCubes().begin();
		mcs != world->getMiniCubes().end();
		++mcs){
		for(int i = 0; i < cubeHeight * cubeWidth; ++i){
			if((mcs->first == cubeCoords[i][0]) && (mcs->second == cubeCoords[i][1])){ //token collision with minicube
				//delete minicube entry
				world->getMiniCubes().erase(mcs);
				//increment score
				score += 10;
				transitionScore += 10;
				}
		}
	}

	bool obCollisionDetected = false;
	//Check Obstacle Collisions
	for(obs = world->getObstacles().begin();
		obs != world->getObstacles().end() &&
		!obCollisionDetected; obs++) {

		//Short-circuit this Obstacle check if possible
		if((*obs)->getPosX() > row + cubeHeight - 1 ||
		   (*obs)->getPosX() + (*obs)->getGTS() - 1 < row ||
		   (*obs)->getPosY() > col + cubeWidth - 1 ||
		   (*obs)->getPosY() + (*obs)->getLongestGS() - 1 < col)
			continue;

		for(nonWSObs = (*obs)->getNonWSObsCoords().begin();
			nonWSObs != (*obs)->getNonWSObsCoords().end() &&
			!obCollisionDetected;
			++nonWSObs){
			for(int i = 0; i < cubeWidth * cubeHeight &&
					!obCollisionDetected; ++i){
				if((nonWSObs->first == cubeCoords[i][0]) &&
				(nonWSObs->second == cubeCoords[i][1])){ //token collision with Obstacle
					//set isDead status to true (1)
					isDead = 1;
					lives--;
					obCollisionDetected = true;
					lastObCollided = *obs;
					if(typeid(*lastObCollided) == typeid(Seaweed))
						return 1;
					else if(typeid(*lastObCollided) == typeid(Coral))
						return 2;
					else if(typeid(*lastObCollided) == typeid(Shark))
						return 3;
					else if(typeid(*lastObCollided) == typeid(Octopus))
						return 4;
					else if(typeid(*lastObCollided) == typeid(Tree))
						return 5;
					else if(typeid(*lastObCollided) == typeid(Rock))
						return 6;
					else if(typeid(*lastObCollided) == typeid(Bird))
						return 7;
					else if(typeid(*lastObCollided) == typeid(Bat))
						return 8;
					else if(typeid(*lastObCollided) == typeid(Asteroid))
						return 9;
					else if(typeid(*lastObCollided) == typeid(Planet))
						return 10;
					else if(typeid(*lastObCollided) == typeid(Comet))
						return 11;
					else if(typeid(*lastObCollided) == typeid(Spaceship))
						return 12;

				}
			}
		}
	}
	return 0;
}

void Cube::fireShot() {
	if(!shotOff) {
		if(curDir == right || curDir == right_down || curDir == right_up) {
			//shotCoords.first = row + cubeHeight / 2 - 1;
			shotCoords.first = row + 2;
			shotCoords.second = col + cubeWidth - 1;
			shotDir = right;
		}
		else if(curDir == left || curDir == left_down || curDir == left_up) {
			//shotCoords.first = row + cubeHeight / 2 - 1;
			shotCoords.first = row + 2;
			shotCoords.second = col;
			shotDir = left;
		}
		else if(curDir == up) {
			shotCoords.first = row;
			shotCoords.second = col + cubeWidth / 2;
			shotDir = up;
		}
		else {
			shotCoords.first = row + cubeHeight - 1;
			shotCoords.second = col + cubeWidth / 2;
			shotDir = down;
		}

		shotOff = true;
	}
}

void Cube::moveShot() {
	if(shotOff) {
	//Move shot
	if(shotDir == right)
		shotCoords.second++;
	else if(shotDir == left)
		shotCoords.second--;
	else if(shotDir == up)
		shotCoords.first--;
	else
		shotCoords.first++;
	}
	//shotColor++;
	//if(shotColor == 35) shotColor = 30;
}

void Cube::processShot() {
	if(shotOff) {
		//Check for hits (miniCubes or Obstacles)
		list<Obstacle*>::iterator obsIt;
		set<pair<int,int> >::iterator mcIt, nonWSObsIt;

		//Check MiniCube hits
		for(mcIt = currWorld->getMiniCubes().begin();
			mcIt != currWorld->getMiniCubes().end() &&
			shotOff;
			++mcIt){
			if(mcIt->first == shotCoords.first &&
			   mcIt->second == shotCoords.second) {
				currWorld->getMiniCubes().erase(mcIt);
					shotOff = false;
					shotCoords.first = -1; shotCoords.second = -1;
			}
		}

		//Check Obstacle hits
		for(obsIt = currWorld->getObstacles().begin();
			obsIt != currWorld->getObstacles().end() &&
			shotOff; obsIt++) {

			//Short-circuit this Obstacle check if possible
			if((*obsIt)->getPosX() > shotCoords.first ||
			   (*obsIt)->getPosX() + (*obsIt)->getGTS() - 1 < shotCoords.first ||
			   (*obsIt)->getPosY() > shotCoords.second ||
			   (*obsIt)->getPosY() + (*obsIt)->getLongestGS() - 1 < shotCoords.second)
				continue;

			for(nonWSObsIt = (*obsIt)->getNonWSObsCoords().begin();
				nonWSObsIt != (*obsIt)->getNonWSObsCoords().end() &&
				shotOff;
				++nonWSObsIt){
				if(nonWSObsIt->first == shotCoords.first &&
				   nonWSObsIt->second == shotCoords.second) {
					   //Add the hole if stationary
					   if((*obsIt)->getIsStationary() == true) {
						   (*obsIt)->getHoles().insert
								(make_pair(nonWSObsIt->first, nonWSObsIt->second));
						   //Remove the nonWS ob coord
						   (*obsIt)->getNonWSObsCoords().erase(nonWSObsIt);
					   }
					   //Track hits if non-stationary
					   else {
							//Increment Obstacle's hit counter
							(*obsIt)->incHits();
							(*obsIt)->resetMvsSinceLastHit();

						   //If Obstacle has reached maxHits, delete it and
						   //increment score based on Obstacle's maxHits
						   if((*obsIt)->getMaxHits() == (*obsIt)->getHits()) {
								score += (*obsIt)->getMaxHits() * 10;
								transitionScore += (*obsIt)->getMaxHits() * 10;
								delete *obsIt;
								currWorld->getObstacles().erase(obsIt);
						   }
					   }

					   shotOff = false;
					   shotCoords.first = -1; shotCoords.second = -1;


				}
			}
		}
	}
	//If no hit, print shot indicator at new position
	if(shotOff) {
		if(shotCoords.first > 0 &&
		   shotCoords.first <= currWorld->getBottomRow() &&
		   shotCoords.second > 0 &&
		   shotCoords.second < COLS) {
			   if(!currWorld->getIsTwoPlayer()) {
				   attron(COLOR_PAIR(shotColor));
				   if(shotDir == up || shotDir == down)
					   //mvaddstr(shotCoords.first, shotCoords.second, "|");
					   mvaddstr(shotCoords.first, shotCoords.second, "■");
				   else
					   //mvaddstr(shotCoords.first, shotCoords.second, "_");
					   mvaddstr(shotCoords.first, shotCoords.second, "■");
				   //refresh();
			   }
		}
		//If shot goes offscreen, reset shot
		else {
			shotOff = false;
			shotCoords.first = -1; shotCoords.second = -1;
		}
	}
}

//Multiplayer Client Only
void Cube::printShot() {
	if(shotCoords.first >= 0 && shotCoords.first <= currWorld->getBottomRow() &&
	   shotCoords.second >= 0 && shotCoords.second < COLS) {
			attron(COLOR_PAIR(shotColor));
			mvaddstr(shotCoords.first, shotCoords.second, "■");
			//refresh();
	 }
}

//http://www.cplusplus.com/doc/tutorial/classes/
//http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/printw.html
//http://www.tldp.org/HOWTO/NCURSES-Programming-HOWTO/color.html
//https://www.quora.com/How-can-I-use-set-in-C++-to-store-a-pair-of-integers
//http://www.cplusplus.com/reference/set/set/erase/
