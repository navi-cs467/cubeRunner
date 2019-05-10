/********************************************************************
** Program name: World.hpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Header file for World class. The World class is 
** 				abstract and provides non-derived class specific
**				member declarations for the dynamic screen layout 
**				during gameplay.
********************************************************************/

#ifndef World_hpp
#define World_hpp

class Game;
//#include "Game.hpp"
#include "Obstacle.hpp"
#include "Seaweed.hpp"
#include "Coral.hpp"
#include "Shark.hpp"
#include "Octopus.hpp"
#include "using.hpp"

class World {
	protected:
		list<Obstacle*> obstacles;
		set<pair<int, int>> miniCubes, obsCoords;
		//vector<WINDOW*> subscrns;
		int gameMode;
		int bottomRow;
		bool isTwoPlayer;
		
	public:
		World(int gameMode, bool isTwoPlayer) :
			gameMode(gameMode), isTwoPlayer(isTwoPlayer) {}
		void updateObsCoords(Obstacle *ob);
		//void updateMiniCubes(pair<int, int>);
		void initMiniCubes(int numMiniCubes, 
						   Direction offScreenDirection = none);
		virtual void renderWorld() = 0;
		//virtual void scroll_(bool newObs) = 0;
		virtual void scroll_() = 0;
		virtual void loadOSObs() = 0;	//Load offscreen Obstacles (for scrolling)
		virtual void loadOSMCs() = 0;	//Load offscreen miniCubes (for scrolling)
		int getBottomRow() const {return bottomRow;}
		void moveObs();
		
		//The following two functions are used by the Obstacle
		//constructor to ensure a new obstacle is not placed on
		//top of an existing obstacle or minicube.
		set<pair<int, int>>& getObsCoords() {return obsCoords;}
		set<pair<int, int>>& getMiniCubes() {return miniCubes;}
		
};

#endif /* World_hpp */
		
		
// Ref
// http://www.cplusplus.com/reference/utility/pair/pair/
// http://www.cplusplus.com/reference/set/set/find/
// https://www.geeksforgeeks.org/pair-in-cpp-stl/
// https://ubuntuforums.org/showthread.php?t=836043