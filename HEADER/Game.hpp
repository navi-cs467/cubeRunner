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
#include "Cube.hpp"
#include "Water.hpp"
#include "Land.hpp"
#include "Space.hpp"
//#include "include.hpp"
#include "constants.hpp"
#include "transitionAnimation.hpp"
#include "waitingForOtherPlayer.hpp"
#include "noServerAvailable.hpp"
#include "earlyTermTransition.hpp"

//client network functions
#include "client.hpp"

struct GameData {
	char* firstName;
	char* secondName;
	int hours;
	int minutes;
	int seconds;
	int finalScore;
	int playerNum;
	bool earlyTerm;
  int gameMode;
};

class Game {
	private:
		Cube *cube;
		World *world;
		bool isTwoPlayer;
		int score;
		double time;
		int gameMode;			//(1) is Hard, (2) is Normal, (3) is Easy


	public:
		Game(int gameMode, bool isTwoPlayer);
		int getScore() {return score;}
		int getGameMode() {return gameMode;}
		void setScore(int score) {this->score = score;}
		struct GameData playGame(char host[] = NULL, char port[] = NULL,
								 char username[] = NULL);
};

#endif /* Game_hpp */
