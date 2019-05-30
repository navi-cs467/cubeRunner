/********************************************************************
** Program name: Token.hpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 05/12/2019
** Description: Header file for Token class. The Token class manages player
** 			token movement, displaying of token and death animation,
**			updating player score, and checking for player death conditions.
********************************************************************/


#ifndef Token_hpp
#define Token_hpp


#include <ncurses.h>
#include <set>
#include "using.hpp"
#include "Direction.hpp"
#include "World.hpp"
#include "Water.hpp"
#include <fstream>
#include <iostream>

#define CUBE_CHARS_WIDTH 5
#define CUBE_CHARS_HEIGHT 3
#define CUBE_COORDS_WIDTH 2
#define CUBE_COORDS_HEIGHT 15


class Cube{
	private:
		int col, row;
		//int currWorld;
		World *currWorld;
		char cubeLeftChars[CUBE_CHARS_HEIGHT][CUBE_CHARS_WIDTH];
		char cubeRightChars[CUBE_CHARS_HEIGHT][CUBE_CHARS_WIDTH];
		int cubeCoords[CUBE_COORDS_HEIGHT][CUBE_COORDS_WIDTH];	 //row, col
		void updateCubePositionHelper(int, int, int, int);
		bool isDead; //0 = alive, 1 = dead
		int score; //points total
		int transitionScore;	//points since last world transition
		int lives;
		int color;
		int highlightColor;
		Direction curDir;
		Direction prevDir;
		static const int cubeHeight = 3;
		static const int cubeWidth = 5;
		void importCubeImage(void);
		bool useLeftCube; //0= right, 1 = left
		bool shotOff;
		pair<int, int> shotCoords;
		int shotColor;
		Direction shotDir;
		Obstacle *lastObCollided;

    public:
		//Cube(void);
		Cube(World *world, int lives);
		void transitionWorld(World *world);
		void cubeInitWorld1(void);
		void cubeInitWorld2(void);
		void cubeInitWorld3(void);
		void cubeReset(World *world);
		void initializeCubeCoords(void);
		int updateCubeCoords(int, int, int, int);
		void updateCubePosition(bool, bool, bool, bool, bool = true);
		int getCubePositionCol(void){return col;}  					//Return Cube's Current Column
		void setCubePositionCol(int col){this->col = col;}  				//Set Cube's Current Column (multiplayer only)
		int getCubePositionRow(void){return row;}  					//Return Cube's Current Row
		void setCubePositionRow(int row){this->row = row;}  				//Set Cube's Current Row (multiplayer only)
		//char (*getCubeChars(void))[4]{return cubeChars;}				//Return's Cube's characters
		//void loadCubeChars(char chars[CUBE_CHARS_HEIGHT][CUBE_CHARS_WIDTH]);							//Load Cube's characters (multiplayer only)
		//int (*getCubeCoords(void))[2]{return cubeCoords;}			//Return Cube's coordinates
		//void loadCubeCoords(int coords[CUBE_COORDS_HEIGHT][CUBE_COORDS_WIDTH]);							//Return Cube's coordinates
		//int getCubeCurrWorld(void){return currWorld;}  				//Return Cube's World
		World* getCubeCurrWorld(void){return currWorld;}
		void setCubeCurrWorld(World *newWorld){currWorld = newWorld;}  //Set Cube's World
		void drawCube(void); 										   //Draw token on console - ncurses
		void drawCubeDeath(int *userInput, int obCollisionType = 0); 									//Explosion Animation
		void drawCubeDeath(void);
		bool getCubeIsDead(void){return isDead;} 					//Return Token's isDead Status
		int getCubeScore(void){return score;} 						//Return Token's current score
		int getCubeTransitionScore(void){return transitionScore;}	//Return Cube's transitionScore
		void resetCubeTransitionScore(void){transitionScore = 0;}	//Reset Cube's transitionScore
		int checkCubeCollision(World *world); 						//Check for collision with minicubes and obstacles (updates score and isDead status)
		void checkCubeCollision(set<pair<int,int> >, set<pair<int,int> >);
		int getCubeLives(void){return lives;}
		void setCubeLives(int lives){this->lives = lives;}		//(multiplayer only)
		int getColor(void){return color;}
		void setCubeScore(int score){this->score = score;}
		Direction getCubeDirection(void){return curDir;}
		void setCubeDirection(Direction newDir) {curDir = newDir;}
		void setTransitionScore(int transitionScore) 
			{this->transitionScore = transitionScore;}
		bool getUseLeftCube() {return useLeftCube;}
		void setUseLeftCube(bool useLeftCube) {this->useLeftCube = useLeftCube;}
		void fireShot();
		void moveShot();
		void processShot();
		int getShotDir() {return shotDir;}
		void printShot();
		pair<int, int> getShotCoords() {return shotCoords;}
		void setShotCoords(int x, int y) {
			shotCoords.first = x;
			shotCoords.second = y;
		}
};




#endif

// References
// https://stackoverflow.com/questions/5201708/how-to-return-a-2d-array-to-a-function-in-c
