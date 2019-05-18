/********************************************************************
** Program name: Token.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 05/12/2019
** Description: Source file for Token class. The Token class manages player
** 			token movement, displaying of token and death animation,
**			updating player score, and checking for player death conditions.
********************************************************************/


#include <iostream>
#include <ncurses.h>
#include "../HEADER/Cube.hpp"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <set>
#include "../HEADER/using.hpp"


//using namespace std;


//Default Initialization
Cube::Cube(World *world, int lives) :
	currWorld(world), lives(lives) {
	col = 0;
	row = 0;
	//currWorld = 1;
	isDead = 0;
	score = 0;
	color = WHITE_BLACK;

	cubeChars[0][0] = '#';
	cubeChars[0][1] = '#';
	cubeChars[0][2] = '#';
	cubeChars[0][3] = '#';
	cubeChars[1][0] = '#';
	cubeChars[1][1] = ' ';
	cubeChars[1][2] = ' ';
	cubeChars[1][3] = '#';
	cubeChars[2][0] = '#';
	cubeChars[2][1] = ' ';
	cubeChars[2][2] = ' ';
	cubeChars[2][3] = '#';
	cubeChars[3][0] = '#';
	cubeChars[3][1] = '#';
	cubeChars[3][2] = '#';
	cubeChars[3][3] = '#';

	cubeCoords[0][0] = (LINES - (LINES - world->getBottomRow())) / 2;
	cubeCoords[0][1] = 2;
	cubeCoords[1][0] = (LINES - (LINES - world->getBottomRow())) / 2;
	cubeCoords[1][1] = 3;
	cubeCoords[2][0] = (LINES - (LINES - world->getBottomRow())) / 2;
	cubeCoords[2][1] = 4;
	cubeCoords[3][0] = (LINES - (LINES - world->getBottomRow())) / 2;
	cubeCoords[3][1] = 5;
	cubeCoords[4][0] = ((LINES - (LINES - world->getBottomRow())) / 2) + 1;
	cubeCoords[4][1] = 2;
	cubeCoords[5][0] = ((LINES - (LINES - world->getBottomRow())) / 2) + 1;
	cubeCoords[5][1] = 3;
	cubeCoords[6][0] = ((LINES - (LINES - world->getBottomRow())) / 2) + 1;
	cubeCoords[6][1] = 4;
	cubeCoords[7][0] = ((LINES - (LINES - world->getBottomRow())) / 2) + 1;
	cubeCoords[7][1] = 5;
	cubeCoords[8][0] = ((LINES - (LINES - world->getBottomRow())) / 2) + 2;
	cubeCoords[8][1] = 2;
	cubeCoords[9][0] = ((LINES - (LINES - world->getBottomRow())) / 2) + 2;
	cubeCoords[9][1] = 3;
	cubeCoords[10][0] = ((LINES - (LINES - world->getBottomRow())) / 2) + 2;
	cubeCoords[10][1] = 4;
	cubeCoords[11][0] = ((LINES - (LINES - world->getBottomRow())) / 2) + 2;
	cubeCoords[11][1] = 5;
	cubeCoords[12][0] = ((LINES - (LINES - world->getBottomRow())) / 2) + 3;
	cubeCoords[12][1] = 2;
	cubeCoords[13][0] = ((LINES - (LINES - world->getBottomRow())) / 2) + 3;
	cubeCoords[13][1] = 3;
	cubeCoords[14][0] = ((LINES - (LINES - world->getBottomRow())) / 2) + 3;
	cubeCoords[14][1] = 4;
	cubeCoords[15][0] = ((LINES - (LINES - world->getBottomRow())) / 2) + 3;
	cubeCoords[15][1] = 5;

	curDir = right;

	//Update row and col
	row = cubeCoords[0][0];
	col = cubeCoords[0][1];

}

/* //Initialize for World 1
void Cube::cubeInitWorld1(void){
	col = 10;
	row = 10;
	currWorld = 1;
	score = 0;
	isDead = 0;
}

//Initialize for World 2
void Cube::cubeInitWorld2(void){
	col = 0;
	row = 0;
	currWorld = 2;
	score = 0;
	isDead = 0;
}

//Initialize for World 3
void Cube::cubeInitWorld3(void){
	col = 0;
	row = 0;
	currWorld = 3;
	score = 0;
	isDead = 0;
} */

void Cube::cubeReset(World *world){
	col = 0;
	row = 0;
	isDead = 0;

	cubeChars[0][0] = '#';
	cubeChars[0][1] = '#';
	cubeChars[0][2] = '#';
	cubeChars[0][3] = '#';
	cubeChars[1][0] = '#';
	cubeChars[1][1] = ' ';
	cubeChars[1][2] = ' ';
	cubeChars[1][3] = '#';
	cubeChars[2][0] = '#';
	cubeChars[2][1] = ' ';
	cubeChars[2][2] = ' ';
	cubeChars[2][3] = '#';
	cubeChars[3][0] = '#';
	cubeChars[3][1] = '#';
	cubeChars[3][2] = '#';
	cubeChars[3][3] = '#';

	cubeCoords[0][0] = (LINES - (LINES - world->getBottomRow())) / 2;
	cubeCoords[0][1] = 2;
	cubeCoords[1][0] = (LINES - (LINES - world->getBottomRow())) / 2;
	cubeCoords[1][1] = 3;
	cubeCoords[2][0] = (LINES - (LINES - world->getBottomRow())) / 2;
	cubeCoords[2][1] = 4;
	cubeCoords[3][0] = (LINES - (LINES - world->getBottomRow())) / 2;
	cubeCoords[3][1] = 5;
	cubeCoords[4][0] = ((LINES - (LINES - world->getBottomRow())) / 2) + 1;
	cubeCoords[4][1] = 2;
	cubeCoords[5][0] = ((LINES - (LINES - world->getBottomRow())) / 2) + 1;
	cubeCoords[5][1] = 3;
	cubeCoords[6][0] = ((LINES - (LINES - world->getBottomRow())) / 2) + 1;
	cubeCoords[6][1] = 4;
	cubeCoords[7][0] = ((LINES - (LINES - world->getBottomRow())) / 2) + 1;
	cubeCoords[7][1] = 5;
	cubeCoords[8][0] = ((LINES - (LINES - world->getBottomRow())) / 2) + 2;
	cubeCoords[8][1] = 2;
	cubeCoords[9][0] = ((LINES - (LINES - world->getBottomRow())) / 2) + 2;
	cubeCoords[9][1] = 3;
	cubeCoords[10][0] = ((LINES - (LINES - world->getBottomRow())) / 2) + 2;
	cubeCoords[10][1] = 4;
	cubeCoords[11][0] = ((LINES - (LINES - world->getBottomRow())) / 2) + 2;
	cubeCoords[11][1] = 5;
	cubeCoords[12][0] = ((LINES - (LINES - world->getBottomRow())) / 2) + 3;
	cubeCoords[12][1] = 2;
	cubeCoords[13][0] = ((LINES - (LINES - world->getBottomRow())) / 2) + 3;
	cubeCoords[13][1] = 3;
	cubeCoords[14][0] = ((LINES - (LINES - world->getBottomRow())) / 2) + 3;
	cubeCoords[14][1] = 4;
	cubeCoords[15][0] = ((LINES - (LINES - world->getBottomRow())) / 2) + 3;
	cubeCoords[15][1] = 5;

	curDir = right;

	//Update row and col
	row = cubeCoords[0][0];
	col = cubeCoords[0][1];
}

void Cube::loadCubeChars(char chars[CUBE_CHARS_HEIGHT][CUBE_CHARS_WIDTH]){
	//Load cubeChars with new values (clientside multiplayer only)
	for(int i = 0; i < CUBE_CHARS_HEIGHT; i++)
		for(int j = 0; j < CUBE_CHARS_WIDTH; j++)
			cubeChars[i][j] = chars[i][j];
}

void Cube::loadCubeCoords(int coords[CUBE_COORDS_HEIGHT][CUBE_COORDS_WIDTH]){
	//Load cubeChars with new values (clientside multiplayer only)
	for(int i = 0; i < CUBE_COORDS_HEIGHT; i++)
		for(int j = 0; j < CUBE_COORDS_WIDTH; j++)
			cubeCoords[i][j] = coords[i][j];
}


//Update the cube token position based on input by the user(s)
void Cube::updateCubePosition(bool colInc, bool colDec, bool rowInc, bool rowDec){
	int colIncInt = 0, colDecInt = 0, rowIncInt = 0, rowDecInt = 0;

	if(colInc == 1){
		colIncInt = 1;
	}
	if(colDec == 1){
		colDecInt = 1;
	}
	if(rowInc == 1){
		rowIncInt = 1;
	}
	if(rowDec == 1){
		rowDecInt = 1;
	}

	//Update curDir member
	if(colInc == 0 && colDec == 1 && rowInc == 0 && rowDec == 0 ){
		curDir = right;
	}
	else if(colInc == 1 && colDec == 0 && rowInc == 0 && rowDec == 0 ){
		curDir = left;
	}
	else if(colInc == 0 && colDec == 0 && rowInc == 0 && rowDec == 1 ){
		curDir = up;
	}
	else if(colInc == 0 && colDec == 0 && rowInc == 1 && rowDec == 0 ){
		curDir = down;
	}
	else if(colInc == 0 && colDec == 1 && rowInc == 0 && rowDec == 1 ){
		curDir = left_up;
	}
	else if(colInc == 0 && colDec == 1 && rowInc == 1 && rowDec == 0 ){
		curDir = left_down;
	}
	else if(colInc == 1 && colDec == 0 && rowInc == 0 && rowDec == 1 ){
		curDir = right_up;
	}
	else if(colInc == 1 && colDec == 0 && rowInc == 1 && rowDec == 0 ){
		curDir = right_down;
	}

	updateCubePositionHelper(colIncInt, colDecInt, rowIncInt, rowDecInt);
}

void Cube::updateCubePositionHelper(int colInc, int colDec, int rowInc, int rowDec){

	int colPrev = 0, rowPrev = 0;

	//In World 1 (Water), the token will move one unit in the col/row
	//position as directed by the user(s). If the user(s) is not
	//providing input for an col/row direction or if both a positive col/row
	//and negative col/row
	if(typeid(*currWorld) == typeid(Water)){
		colPrev = col;
		rowPrev = row;
		col = col + colInc - colDec;
		row = row + rowInc - rowDec;
	}
	/* else if(typeid(*currWorld) == typeid(Land)){
		colPrev = col;
		rowPrev = row;
		col = col + colInc - colDec;
		row = row + rowInc - rowDec;
	}
	else if(typeid(*currWorld) == typeid(Space)){
		colPrev = col;
		rowPrev = row;
		col = col + colInc - colDec;
		row = row + rowInc - rowDec;
	} */

	Cube::updateCubeCharsAndCoords(colPrev, col, rowPrev, row,1,1);
}



int Cube::updateCubeCharsAndCoords(int colPrev, int colCurr, int rowPrev, int rowCurr, int rowDistance, int colDistance){
	int colChange = 0, rowChange = 0;
	int i = 0, j = 0;

	colChange = colCurr - colPrev;
	rowChange = rowCurr - rowPrev;

	//New Direction = NONE
	if((colChange == 0) && (rowChange == 0)){
		cubeChars[1][1] = ' ';
		cubeChars[1][2] = ' ';
		cubeChars[2][1] = ' ';
		cubeChars[2][2] = ' ';
	}

	//New Direction = UP
	else if((colChange == 0) && (rowChange < 0)){
		for(i = 0; i < cubeHeight * cubeWidth; ++i){
			cubeCoords[i][0] = cubeCoords[i][0] - rowDistance;
		}
		cubeChars[1][1] = 'O';
		cubeChars[1][2] = 'O';
		cubeChars[2][1] = ' ';
		cubeChars[2][2] = ' ';
	}

	//New Direction = UP RIGHT
	else if((colChange > 0) && (rowChange < 0)){
		for(i = 0; i < cubeHeight * cubeWidth; ++i){
			cubeCoords[i][0] = cubeCoords[i][0] - rowDistance;
			cubeCoords[i][1] = cubeCoords[i][1] + colDistance;
		}
		cubeChars[1][1] = ' ';
		cubeChars[1][2] = 'O';
		cubeChars[2][1] = ' ';
		cubeChars[2][2] = ' ';
	}

	//New Direction = RIGHT
	//else if((colChange > 0) && (rowChange == 0)){ dir = right; }  // #4
	else if((colChange > 0) && (rowChange == 0)){
		for(i = 0; i < cubeHeight * cubeWidth; ++i){
			cubeCoords[i][1] = cubeCoords[i][1] + colDistance;
		}
		cubeChars[1][1] = ' ';
		cubeChars[1][2] = 'O';
		cubeChars[2][1] = ' ';
		cubeChars[2][2] = 'O';
	}

	//New Direction = DOWN RIGHT
	else if((colChange > 0) && (rowChange > 0)){
		for(i = 0; i < cubeHeight * cubeWidth; ++i){
			cubeCoords[i][0] = cubeCoords[i][0] + rowDistance;
			cubeCoords[i][1] = cubeCoords[i][1] + colDistance;
		}
		cubeChars[1][1] = ' ';
		cubeChars[1][2] = ' ';
		cubeChars[2][1] = ' ';
		cubeChars[2][2] = 'O';
	}

	//New Direction = DOWN
	else if((colChange == 0) && (rowChange > 0)){
		for(i = 0; i < cubeHeight * cubeWidth; ++i){
			cubeCoords[i][0] = cubeCoords[i][0] + rowDistance;
		}
		cubeChars[1][1] = ' ';
		cubeChars[1][2] = ' ';
		cubeChars[2][1] = 'O';
		cubeChars[2][2] = 'O';
	}

	//New Direction = DOWN LEFT
	else if((colChange < 0) && (rowChange > 0)){
		for(i = 0; i < cubeHeight * cubeWidth; ++i){
			cubeCoords[i][0] = cubeCoords[i][0] + rowDistance;
			cubeCoords[i][1] = cubeCoords[i][1] - colDistance;
		}
		cubeChars[1][1] = ' ';
		cubeChars[1][2] = ' ';
		cubeChars[2][1] = 'O';
		cubeChars[2][2] = ' ';
	}

	//New Direction = LEFT
	else if((colChange < 0) && (rowChange == 0)){
		for(i = 0; i < cubeHeight * cubeWidth; ++i){
			cubeCoords[i][1] = cubeCoords[i][1] - colDistance;
		}
		cubeChars[1][1] = 'O';
		cubeChars[1][2] = ' ';
		cubeChars[2][1] = 'O';
		cubeChars[2][2] = ' ';
	}

	//New Direction = UP LEFT
	else if((colChange < 0) && (rowChange < 0)){
		for(i = 0; i < cubeHeight * cubeWidth; ++i){
			cubeCoords[i][0] = cubeCoords[i][0] - rowDistance;
			cubeCoords[i][1] = cubeCoords[i][1] - colDistance;
		}
		cubeChars[1][1] = 'O';
		cubeChars[1][2] = ' ';
		cubeChars[2][1] = ' ';
		cubeChars[2][2] = ' ';
	}

	//Error
	else{ return 1; }

	//Update row and col
	row = cubeCoords[0][0];
	col = cubeCoords[0][1];

	return 0;
}

void Cube::drawCube(void){
	mvaddch(cubeCoords[0][0],cubeCoords[0][1],cubeChars[0][0]);
	mvaddch(cubeCoords[1][0],cubeCoords[1][1],cubeChars[0][1]);
	mvaddch(cubeCoords[2][0],cubeCoords[2][1],cubeChars[0][2]);
	mvaddch(cubeCoords[3][0],cubeCoords[3][1],cubeChars[0][3]);
	mvaddch(cubeCoords[4][0],cubeCoords[4][1],cubeChars[1][0]);
	mvaddch(cubeCoords[5][0],cubeCoords[5][1],cubeChars[1][1]);
	mvaddch(cubeCoords[6][0],cubeCoords[6][1],cubeChars[1][2]);
	mvaddch(cubeCoords[7][0],cubeCoords[7][1],cubeChars[1][3]);
	mvaddch(cubeCoords[8][0],cubeCoords[8][1],cubeChars[2][0]);
	mvaddch(cubeCoords[9][0],cubeCoords[9][1],cubeChars[2][1]);
	mvaddch(cubeCoords[10][0],cubeCoords[10][1],cubeChars[2][2]);
	mvaddch(cubeCoords[11][0],cubeCoords[11][1],cubeChars[2][3]);
	mvaddch(cubeCoords[12][0],cubeCoords[12][1],cubeChars[3][0]);
	mvaddch(cubeCoords[13][0],cubeCoords[13][1],cubeChars[3][1]);
	mvaddch(cubeCoords[14][0],cubeCoords[14][1],cubeChars[3][2]);
	mvaddch(cubeCoords[15][0],cubeCoords[15][1],cubeChars[3][3]);

	refresh();

}

void Cube::drawCubeDeath(int *userInput){

	attron(COLOR_PAIR(YELLOW_BLACK));
	mvaddstr(LINES - 1, 15, "Death! (Press Enter to Continue)");
	refresh();

	int i = 0, j = 0, k = 0;

	start_color();
	init_pair(1, COLOR_RED, COLOR_BLACK);
	attron(COLOR_PAIR(1));

	//So animation isn't skipped if last user input is enter
	*userInput = 0;

	//Death Part 1 - small ring around token
	for(i = (row - 1); i <= (row + 4); ++i){ //draw left/right sides
		mvaddch(i,(col-1),'@');
		mvaddch(i,(col+4),'@');
	}
	for(i = col; i <= (col + 4); ++i){ //draw top/bottom sides
		mvaddch((row-1),i,'@');
		mvaddch((row+4),i,'@');
	}
	refresh();
	for(int i = 0; *userInput != 10 && i < 500000 / 100; i++)
		usleep(50);

	attroff(COLOR_PAIR(1));
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);
	attron(COLOR_PAIR(2));

	//Death Part 2 - larger ring around token
	for(i = (row - 2); i <= (row + 5); ++i){ //draw left/right sides
		mvaddch(i,(col-2),'@');
		mvaddch(i,(col+5),'@');
	}
	for(i = (col - 1); i <= (col + 5); ++i){ //draw top/bottom sides
		mvaddch((row-2),i,'@');
		mvaddch((row+5),i,'@');
	}
	refresh();
	for(int i = 0; *userInput != 10 && i < 450000 / 100; i++)
		usleep(50);

	attroff(COLOR_PAIR(2));
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	attron(COLOR_PAIR(3));


	//Death Part 3 - first broken ring around token
	for(i = (row - 3); i <= (row + 6); (i=i+2)){ //draw left/right sides
		mvaddch(i,(col-3),'@');
		mvaddch((i+1),(col+6),'@');
	}
	for(i = (col - 1); i <= (col + 6); (i=i+2)){ //draw top/bottom sides
		mvaddch((row-3),i,'@');
		mvaddch((row+6),(i-1),'@');
	}
	refresh();
	for(int i = 0; *userInput != 10 && i < 400000 / 100; i++)
		usleep(50);

	attroff(COLOR_PAIR(3));
	init_pair(4, COLOR_BLUE, COLOR_BLACK);
	attron(COLOR_PAIR(4));


	//Death Part 4 - second broken ring around token
	for(i = (row - 4); i <= (row + 7); (i=i+2)){ //draw left/right sides
		mvaddch(i,(col-4),'@');
		mvaddch((i+1),(col+7),'@');
	}
	for(i = (col - 2); i <= (col + 7); (i=i+2)){ //draw top/bottom sides
		mvaddch((row-4),i,'@');
		mvaddch((row+7),(i-1),'@');
	}
	refresh();
	for(int i = 0; *userInput != 10 && i < 350000 / 100; i++)
		usleep(50);

	attroff(COLOR_PAIR(4));
	init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
	attron(COLOR_PAIR(5));

	//Death Part 5 - third broken ring around token
	for(i = (row - 5); i <= (row + 8); (i=i+2)){ //draw left/right sides
		mvaddch(i,(col-5),'@');
		mvaddch((i+1),(col+8),'@');
	}
	for(i = (col - 3); i <= (col + 8); (i=i+2)){ //draw top/bottom sides
		mvaddch((row-5),i,'@');
		mvaddch((row+8),(i-1),'@');
	}
	refresh();
	for(int i = 0; *userInput != 10 && i < 300000 / 100; i++)
		usleep(50);

	attroff(COLOR_PAIR(5));
	init_pair(6, COLOR_CYAN, COLOR_BLACK);
	attron(COLOR_PAIR(6));

	//Death Part 6 - fourth broken ring around token (shorter)
	for(i = (row - 2); i <= (row + 6); (i=i+2)){ //draw left/right sides
		mvaddch(i,(col-6),'@');
		mvaddch((i-1),(col+9),'@');
	}
	for(i = (col - 2); i <= (col + 6); (i=i+2)){ //draw top/bottom sides
		mvaddch((row-6),i,'@');
		mvaddch((row+9),(i-1),'@');
	}
	refresh();
	for(int i = 0; *userInput != 10 && i < 300000 / 100; i++)
		usleep(50);

	attroff(COLOR_PAIR(6));
	init_pair(7, COLOR_WHITE, COLOR_BLACK);
	attron(COLOR_PAIR(7));

	for(k = 0; k < 5; ++k){
		//Token Flash Part 1 - blank
		for(i = row; i < (row + 4); ++i){
			for(j = col; j < (col + 4); ++j){
				mvaddch(i,j,' ');
			}
		}
		refresh();
		for(int i = 0; *userInput != 10 && i < 500000 / 100; i++)
			usleep(50);

		//Token Flash Part 2 - appear
		for(i = row; i < (row + 4); ++i){
			for(j = col; j < (col + 4); ++j){
				mvaddch(i,j,'X');
			}
		}
		refresh();
		for(int i = 0; *userInput != 10 && i < 500000 / 100; i++)
			usleep(50);
	}

	attroff(COLOR_PAIR(7));

	//Block here until user presses Enter key
	while(*userInput != 10){}

	//Clear Enter key prompt
	attron(COLOR_PAIR(BLACK_BLACK));
	mvaddstr(LINES - 1, 15, "                                ");
	refresh();

}

void Cube::checkCubeCollision(World *world){

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
				score += 10;			}
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
				}
			}
		}
	}
}


/* int main () {
  Cube cubeA;
  char userInput[2];
  int i = 0;
  int numMoves = 30;

  set<pair<int,int> > miniCubeCoords;
  set<pair<int,int> > obsCoords;

  miniCubeCoords.insert(make_pair(13, 20));
  obsCoords.insert(make_pair(20, 20));

  cubeA.Cube::cubeInitWorld1();

  initscr(); //Start Curses

  cubeA.Cube::drawCube();
  mvaddstr(13,20,"M");
  mvaddstr(20,20,"B");
  refresh();

  while(1){
    //Get user input for where the token should be moved next
    mvaddstr(37,0,"IsDead: ");
    printw("%s", cubeA.Cube::getCubeIsDead() ? "true" : "false");
    mvaddstr(36,0,"Score: ");
    printw("%d", cubeA.Cube::getCubeScore());
    mvaddstr(35,0,"Enter Direction >> ");
    getstr(userInput); //Wait for user input

    //Update token position depending on user input
    if(userInput[0] == '4'){
      cubeA.Cube::updateCubePosition(0, 1, 0, 0);
    }
    else if(userInput[0] == '6'){
      cubeA.Cube::updateCubePosition(1, 0, 0, 0);
    }
    else if(userInput[0] == '8'){
      cubeA.Cube::updateCubePosition(0, 0, 0, 1);
    }
    else if(userInput[0] == '2'){
      cubeA.Cube::updateCubePosition(0, 0, 1, 0);
    }
    else if(userInput[0] == '7'){
      cubeA.Cube::updateCubePosition(0, 1, 0, 1);
    }
    else if(userInput[0] == '1'){
      cubeA.Cube::updateCubePosition(0, 1, 1, 0);
    }
    else if(userInput[0] == '9'){
      cubeA.Cube::updateCubePosition(1, 0, 0, 1);
    }
    else if(userInput[0] == '3'){
      cubeA.Cube::updateCubePosition(1, 0, 1, 0);
    }
    else if(userInput[0] == 'd'){
      cubeA.Cube::drawCubeDeath();
    }
    else{
      cubeA.Cube::updateCubePosition(0, 0, 0, 0);
    }

    clear();
    cubeA.Cube::drawCube();
    mvaddstr(13,20,"M");
    mvaddstr(20,20,"B");
    cubeA.Cube::checkCubeCollision(miniCubeCoords, obsCoords);
    refresh();

  }



  getch(); //Wait for user input
  endwin(); //End Curses


  return 0;
} */

//http://www.cplusplus.com/doc/tutorial/classes/
//http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/printw.html
//http://www.tldp.org/HOWTO/NCURSES-Programming-HOWTO/color.html
//https://www.quora.com/How-can-I-use-set-in-C++-to-store-a-pair-of-integers
//http://www.cplusplus.com/reference/set/set/erase/
