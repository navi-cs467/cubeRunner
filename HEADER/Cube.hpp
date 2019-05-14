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


class Cube{
	private:
		int col, row;
		int currWorld;
		char cubeChars[4][4];
		int cubeCoords[16][2];	 //row, col	
		void updateCubeTokenPositionHelper(int, int, int, int);
		int updateCubeCharsAndCoords(int, int, int, int, int, int);
		bool isDead; //0 = alive, 1 = dead
		int score; //points total
		int transitionScore;	//points since last world transition
		int lives;
		int color;
		Direction curDir;
		
    public:
		Cube(){}
		Cube(World *world, int lives);
		void cubeInitWorld1(void);
		void cubeInitWorld2(void);
		void cubeInitWorld3(void);
		void cubeReset(World *world);
		void updateCubeTokenPosition(bool, bool, bool, bool);
		int getCubePositionCol(void){return col;}  //Return Cube's Current Column
		int getCubePositionRow(void){return row;}  //Return Cube's Current Row
		int (*getCubeTokenCoords(void))[2]{return cubeCoords;}	//Return Cube's coordinates
		int getCubeCurrWorld(void){return currWorld;}  //Return Cube's World
		void setCubeCurrWorld(int newWorld){currWorld = newWorld;}  //Set Cube's World
		void drawCube(void); //Draw token on console - ncurses
		void drawCubeDeath(void); //Explosion Animation
		bool getCubeIsDead(void){return isDead;} //Return Token's isDead Status
		int getCubeScore(void){return score;} //Return Token's current score
		int getCubeTransitionScore(void){return transitionScore;}	//Return Cube's transitionScore
		void resetCubeTransitionScore(void){transitionScore = 0;}	//Reset Cube's transitionScore
		void checkCubeCollision(World *world); //Check for collision with minicubes and obstacles (updates score and isDead status)
		int getCubeLives(void){return lives;}
		int getColor(void){return color;}
		Direction getCubeDirection(void){return curDir;}
		void setCubeDirection(Direction newDir) {curDir = newDir;}
};



#endif

// References
// https://stackoverflow.com/questions/5201708/how-to-return-a-2d-array-to-a-function-in-c