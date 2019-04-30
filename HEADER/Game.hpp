/********************************************************************
** Program name: Game.hpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Header file for Game class. The Game class is the
** 				maintainer and executer of the game state and 
**				progression.
********************************************************************/

#ifndef Game_hpp
#define Game_hpp

//#include "World.hpp"
class World;
#include "Water.hpp"
//#include "include.hpp"
#include "constants.hpp"

class Game {
	private:
		//Cube cube;
		World *world;
		bool isTwoPlayer;
		int score;
		double time;
		int gameMode;			//(1) is Hard, (2) is Normal, (3) is Easy
		
		static char **board;	//Last line of screen is reserved
								//for life count, time, and score
								//display.

	public:
		Game(int gameMode, bool isTwoPlayer);
		int getScore() {return score;}
		int getGameMode() {return gameMode;}
		void setScore(int score) {this->score = score;}
		int playGame();
		static char** getBoard() {return board;}
		static void setBoard(int x, int y, char ch) {board[x][y] = ch;}
};

#endif /* Game_hpp */