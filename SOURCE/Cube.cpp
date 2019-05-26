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


//using namespace std;


//Create Cube
//Cube::Cube(){
Cube::Cube(World *world, int lives) :
currWorld(world), lives(lives) {
	col = 2;
	row = (LINES - (LINES - world->getBottomRow())) / 2;
	isDead = 0;
	score = 0;
	transitionScore = 0;
	//currWorld = 1;
	currWorld = world;
	//lives = 2;
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

//Default Initialization
/*
Cube::Cube(World *world, int lives) :
	currWorld(world), lives(lives) {
	//col = 0;
	//row = 0;
	//currWorld = 1;
	isDead = 0;
	score = 0;
	color = WHITE_BLACK;

	importCubeImage();


	//Update row and col
	row = (LINES - (LINES - world->getBottomRow())) / 2;
	col = 2;

	initializeCubeCoords();
	useLeftCube = 0;


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

}
*/

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
	shotCoords.first = -1; shotCoords.second = -1;
}

//Initialize for World 1
/* void Cube::cubeInitWorld1(void){
	col = 5;
	row = 15;
	currWorld = 1;
	score = 0;
	isDead = 0;
	curDir = none;
	prevDir = none;
	useLeftCube = 0;
	initializeCubeCoords();
	lives = 2;
} */

//Initialize for World 2
/* void Cube::cubeInitWorld2(void){
	col = 5;
	row = 15;
	currWorld = 2;
	transitionScore = score;
	score = 0;
	isDead = 0;
	curDir = none;
	prevDir = none;
	useLeftCube = 0;
	initializeCubeCoords();
	lives = 2;

} */

//Initialize for World 3
/* void Cube::cubeInitWorld3(void){
	col = 5;
	row = 15;
	currWorld = 3;
	transitionScore = score;
	score = 0;
	isDead = 0;
	curDir = none;
	prevDir = right;
	useLeftCube = 0;
	initializeCubeCoords();
	lives = 2;

}  */

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

/*
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
*/
	curDir = right;
	
	//Initialize shotOff and shotCoords
	shotOff = false;
	shotCoords.first = -1; shotCoords.second = -1;
}

//OLD CUBE ONLY
/* void Cube::loadCubeChars(char chars[CUBE_CHARS_HEIGHT][CUBE_CHARS_WIDTH]){
	//Load cubeChars with new values (clientside multiplayer only)

	for(int i = 0; i < CUBE_CHARS_HEIGHT; i++){
		for(int j = 0; j < CUBE_CHARS_WIDTH; j++){
			if(useLeftCube == 0){
				cubeRightChars[i][j] = chars[i][j];
			}
			else{
				cubeLeftChars[i][j] = chars[i][j];
			}
		}
	}



	for(int i = 0; i < CUBE_CHARS_HEIGHT; i++)
		for(int j = 0; j < CUBE_CHARS_WIDTH; j++)
			cubeChars[i][j] = chars[i][j];

} */

//OLD CUBE ONLY
/* void Cube::loadCubeCoords(int coords[CUBE_COORDS_HEIGHT][CUBE_COORDS_WIDTH]){
	//Load cubeChars with new values (clientside multiplayer only)
	for(int i = 0; i < CUBE_COORDS_HEIGHT; i++)
		for(int j = 0; j < CUBE_COORDS_WIDTH; j++)
			cubeCoords[i][j] = coords[i][j];
} */


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

	//In World 1 (Water), the token will move one unit in the col/row
	//position as directed by the user(s). If the user(s) is not
	//providing input for an col/row direction or if both a positive col/row
	//and negative col/row
	//if(typeid(*currWorld) == typeid(Water)){
	//if(currWorld == 1){
		colPrev = col;
		rowPrev = row;
		col = col + colInc - colDec;
		row = row + rowInc - rowDec;
		Cube::updateCubeCoords(colPrev, col, rowPrev, row);

	//}
	/* else if(typeid(*currWorld) == typeid(Land)){
	//else if(currWorld == 2){
		colPrev = col;
		rowPrev = row;
		col = col + colInc - colDec;
		row = row + rowInc - rowDec;
		if(rowPrev == row){ 
			row = row + 1;
		}
		Cube::updateCubeCoords(colPrev, col, rowPrev, row);
	} */
	/* else if(typeid(*currWorld) == typeid(Space)){
	//else if(currWorld == 3){
		colPrev = col;
		rowPrev = row;
		col = col + colInc - colDec;
		row = row + rowInc - rowDec;
		if(curDir == none){
			refresh();
			if((prevDir == up)||(prevDir == right_up)||(prevDir == left_up)){
				row = row - 1;
			}
			if((prevDir == down)||(prevDir == right_down)||(prevDir == left_down)){
				row = row + 1;
			}
			if((prevDir == right)||(prevDir == right_up)||(prevDir == right_down)){
				col = col + 1;
			}
			if((prevDir == left)||(prevDir == left_up)||(prevDir == left_down)){
				col = col - 1;
			}

		}

		Cube::updateCubeCoords(colPrev, col, rowPrev, row);
	} */

	
	

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
	
	/* if(useLeftCube == 0){
		mvaddch(cubeCoords[0][0],cubeCoords[0][1],cubeRightChars[0][0]);
		mvaddch(cubeCoords[1][0],cubeCoords[1][1],cubeRightChars[0][1]);
		mvaddch(cubeCoords[2][0],cubeCoords[2][1],cubeRightChars[0][2]);
		mvaddch(cubeCoords[3][0],cubeCoords[3][1],cubeRightChars[0][3]);
		mvaddch(cubeCoords[4][0],cubeCoords[4][1],cubeRightChars[0][4]);
		mvaddch(cubeCoords[5][0],cubeCoords[5][1],cubeRightChars[1][0]);
		mvaddch(cubeCoords[6][0],cubeCoords[6][1],cubeRightChars[1][1]);
		mvaddch(cubeCoords[7][0],cubeCoords[7][1],cubeRightChars[1][2]);
		mvaddch(cubeCoords[8][0],cubeCoords[8][1],cubeRightChars[1][3]);
		mvaddch(cubeCoords[9][0],cubeCoords[9][1],cubeRightChars[1][4]);
		mvaddch(cubeCoords[10][0],cubeCoords[10][1],cubeRightChars[2][0]);
		mvaddch(cubeCoords[11][0],cubeCoords[11][1],cubeRightChars[2][1]);
		mvaddch(cubeCoords[12][0],cubeCoords[12][1],cubeRightChars[2][2]);
		mvaddch(cubeCoords[13][0],cubeCoords[13][1],cubeRightChars[2][3]);
		mvaddch(cubeCoords[14][0],cubeCoords[14][1],cubeRightChars[2][4]);
	}
	if(useLeftCube == 1){
		mvaddch(cubeCoords[0][0],cubeCoords[0][1],cubeLeftChars[0][0]);
		mvaddch(cubeCoords[1][0],cubeCoords[1][1],cubeLeftChars[0][1]);
		mvaddch(cubeCoords[2][0],cubeCoords[2][1],cubeLeftChars[0][2]);
		mvaddch(cubeCoords[3][0],cubeCoords[3][1],cubeLeftChars[0][3]);
		mvaddch(cubeCoords[4][0],cubeCoords[4][1],cubeLeftChars[0][4]);
		mvaddch(cubeCoords[5][0],cubeCoords[5][1],cubeLeftChars[1][0]);
		mvaddch(cubeCoords[6][0],cubeCoords[6][1],cubeLeftChars[1][1]);
		mvaddch(cubeCoords[7][0],cubeCoords[7][1],cubeLeftChars[1][2]);
		mvaddch(cubeCoords[8][0],cubeCoords[8][1],cubeLeftChars[1][3]);
		mvaddch(cubeCoords[9][0],cubeCoords[9][1],cubeLeftChars[1][4]);
		mvaddch(cubeCoords[10][0],cubeCoords[10][1],cubeLeftChars[2][0]);
		mvaddch(cubeCoords[11][0],cubeCoords[11][1],cubeLeftChars[2][1]);
		mvaddch(cubeCoords[12][0],cubeCoords[12][1],cubeLeftChars[2][2]);
		mvaddch(cubeCoords[13][0],cubeCoords[13][1],cubeLeftChars[2][3]);
		mvaddch(cubeCoords[14][0],cubeCoords[14][1],cubeLeftChars[2][4]);
	} */
	
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

		//attroff(COLOR_PAIR(1));
	}

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
	for(i = (col-1); i <= (col+5); ++i){ //draw top/bottom sides
		mvaddch((row-1),i,'@');
		mvaddch((row+3),i,'@');
	}
	for(i = row; i <= (row+2); ++i){ //draw left/right sides
		mvaddch(i,(col-1),'@');
		mvaddch(i,(col+5),'@');
	}
	refresh();
	for(int i = 0; *userInput != 10 && i < 500000 / 100; i++)
		usleep(50);

	attroff(COLOR_PAIR(1));
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);
	attron(COLOR_PAIR(2));

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
	for(int i = 0; *userInput != 10 && i < 450000 / 100; i++)
		usleep(50);

	attroff(COLOR_PAIR(2));
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	attron(COLOR_PAIR(3));


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
	for(int i = 0; *userInput != 10 && i < 400000 / 100; i++)
		usleep(50);

	attroff(COLOR_PAIR(3));
	init_pair(4, COLOR_BLUE, COLOR_BLACK);
	attron(COLOR_PAIR(4));


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
	for(int i = 0; *userInput != 10 && i < 350000 / 100; i++)
		usleep(50);

	attroff(COLOR_PAIR(4));
	init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
	attron(COLOR_PAIR(5));

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
	for(int i = 0; *userInput != 10 && i < 300000 / 100; i++)
		usleep(50);

	attroff(COLOR_PAIR(5));
	init_pair(6, COLOR_CYAN, COLOR_BLACK);
	attron(COLOR_PAIR(6));

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

	for(int i = 0; *userInput != 10 && i < 300000 / 100; i++)
		usleep(50);

	attroff(COLOR_PAIR(6));
	init_pair(7, COLOR_WHITE, COLOR_BLACK);
	attron(COLOR_PAIR(7));

	for(k = 0; k < 5; ++k){
		//Token Flash Part 1 - blank
		for(i = row; i <= (row+2); ++i){
			for(j = col; j <= (col+4); ++j){ 
				mvaddch(i,j,' ');
			}
		}
		refresh();
		for(int i = 0; *userInput != 10 && i < 500000 / 100; i++)
			usleep(50);

		//Token Flash Part 2 - appear
		for(i = row; i <= (row+2); ++i){
			for(j = col; j <= (col+4); ++j){ 
				mvaddch(i,j,'X');
			}
		}
		refresh();
		for(int i = 0; *userInput != 10 && i < 500000 / 100; i++)
			usleep(50);
	}

	attroff(COLOR_PAIR(7));
	
	attron(COLOR_PAIR(YELLOW_BLACK));
	mvaddstr(LINES - 1, 15, "Death! (Press Enter to Continue)");
	refresh();

	//Block here until user presses Enter key
	while(*userInput != 10){}

	//Clear Enter key prompt
	attron(COLOR_PAIR(BLACK_BLACK));
	mvaddstr(LINES - 1, 15, "                                ");
	refresh();

}

//void Cube::drawCubeDeath(int *userInput){
/* void Cube::drawCubeDeath(void){

	
	int i = 0, j = 0, k = 0;

	//*userInput = 0;

	start_color();			
	init_pair(1, COLOR_RED, COLOR_BLACK);
	attron(COLOR_PAIR(1));


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
	usleep(500000);	

	attroff(COLOR_PAIR(1));
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);
	attron(COLOR_PAIR(2));


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
	usleep(450000);

	attroff(COLOR_PAIR(2));
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	attron(COLOR_PAIR(3));


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
	usleep(400000);

	attroff(COLOR_PAIR(3));
	init_pair(4, COLOR_BLUE, COLOR_BLACK);
	attron(COLOR_PAIR(4));


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
	usleep(350000);

	attroff(COLOR_PAIR(4));
	init_pair(5, COLOR_MAGENTA, COLOR_BLACK);
	attron(COLOR_PAIR(5));

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
	usleep(300000);

	attroff(COLOR_PAIR(5));
	init_pair(6, COLOR_CYAN, COLOR_BLACK);
	attron(COLOR_PAIR(6));

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
	usleep(300000);

	for(i = (col-2); i <= (col+6); (i=i+2)){ //draw top/bottom sides
		mvaddch((row-7),i,'@');
		mvaddch((row+9),i,'@');
	}
	for(i = (row-3); i <= (row+5); (i=i+2)){ //draw left/right sides
		mvaddch(i,(col-7),'@');
		mvaddch(i,(col+11),'@');
	}
	refresh();
	usleep(300000);


	attroff(COLOR_PAIR(6));
	init_pair(7, COLOR_WHITE, COLOR_BLACK);
	attron(COLOR_PAIR(7));

	for(k = 0; k < 5; ++k){
		//Token Flash Part 1 - blank
		for(i = row; i <= (row+2); ++i){
			for(j = col; j <= (col+4); ++j){ 
				mvaddch(i,j,' ');
			}
		}
		refresh();
		usleep(500000);

		//Token Flash Part 2 - X's
		for(i = row; i <= (row+2); ++i){
			for(j = col; j <= (col+4); ++j){ 
				mvaddch(i,j,'X');
			}
		}
		refresh();
		usleep(500000);
	}

	attroff(COLOR_PAIR(7));
} */


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
				}
			}
		}
	}
}

/* void Cube::checkCubeCollision(set<pair<int, int> > miniCubes, set<pair<int, int> > obsCoords){
	int i = 0;
	set<pair<int,int> >::iterator setIterator;
	
	//Check MiniCube Collisions
	for(setIterator=miniCubes.begin(); setIterator!=miniCubes.end(); ++setIterator){
		for(i = 0; i < 15; ++i){
			if((setIterator->first == cubeCoords[i][0]) && (setIterator->second == cubeCoords[i][1])){ //token collision with minicube
				//delete minicube entry
				miniCubes.erase(setIterator);
				//increment score
				++score;
				mvaddstr(39,0,"SCORE INC");
			}
		}
	}	

	//Check Obstacle Collisions
	for(setIterator=obsCoords.begin(); setIterator!=obsCoords.end(); ++setIterator){
		for(i = 0; i < 15; ++i){
			if((setIterator->first == cubeCoords[i][0]) && (setIterator->second == cubeCoords[i][1])){ //token collision with minicube
				//set isDead status to true (1)
				isDead = 1;
				mvaddstr(39,0,"DEAD");

			}
		}
	}

} */

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



/* int main () {
  Cube cubeA;
  char userInput[2];
  int i = 0;
  int numMoves = 30;
  int* cubeDeathParameter;

  set<pair<int,int> > miniCubeCoords;
  set<pair<int,int> > obsCoords;

  miniCubeCoords.insert(make_pair(13, 20));
  obsCoords.insert(make_pair(20, 20));

  cubeA.Cube::cubeInitWorld3();

  initscr(); //Start Curses

  cubeA.Cube::drawCube();
  refresh();

  while(1){
    mvaddstr(13,20,"M");
    mvaddstr(20,20,"B");

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
      //cubeA.Cube::drawCubeDeath(cubeDeathParameter);
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

    if(cubeA.Cube::IsDead() == 1){
	cubeA.Cube::drawCubeDeath();
	clear();
	cubeA.Cube::cubeInitWorld3();
	cubeA.Cube::drawCube();
	refresh();
	
    }

  }



  getch(); //Wait for user input
  endwin(); //End Curses


  return 0;
}  */

//http://www.cplusplus.com/doc/tutorial/classes/
//http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/printw.html
//http://www.tldp.org/HOWTO/NCURSES-Programming-HOWTO/color.html
//https://www.quora.com/How-can-I-use-set-in-C++-to-store-a-pair-of-integers
//http://www.cplusplus.com/reference/set/set/erase/
