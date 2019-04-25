/********************************************************************
** Program name: Game.hpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Header file for Game class. The Game class is the
** 				maintainer and executer of the game state and 
**				progression.
********************************************************************/

class Game {
	private:
		Cube cube;
		World *world;
		bool isTwoPlayers;
		int score;
		double time;
		int gameMode;		//(1) is Hard, (2) is Normal, (3) is Easy

	public:
		Game(bool isTwoPlayers, int gameMode);
		int getScore() {return score;}
		void setScore(int score); {this->score = score;}
		int playGame();
};