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
class Cube;
//#include "Game.hpp"
//#include "Cube.hpp"
#include "Obstacle.hpp"
#include "Seaweed.hpp"
#include "Coral.hpp"
#include "Shark.hpp"
#include "Octopus.hpp"
#include "Tree.hpp"
#include "Rock.hpp"
#include "Bird.hpp"
#include "Bat.hpp"
#include "Asteroid.hpp"
#include "Planet.hpp"
#include "Comet.hpp"
#include "Spaceship.hpp"
#include "using.hpp"

class World {
	protected:
		list<Obstacle*> obstacles;
		set<pair<int, int>> miniCubes, obsCoords; 
		int gameMode;
		int bottomRow;
		bool isTwoPlayer;
		bool forServer;
		
	public:
		World(bool isTwoPlayer) : isTwoPlayer(isTwoPlayer) {}
		World(int gameMode, bool isTwoPlayer, bool forServer = false) :
			gameMode(gameMode), 
			isTwoPlayer(isTwoPlayer), 
			forServer(forServer) {}
		void updateObsCoords(Obstacle *ob);
		//void updateMiniCubes(pair<int, int>);
		void initMiniCubes(int numMiniCubes, 
						   Direction offScreenDirection = none);
		virtual void renderWorld(Cube *cube) = 0;
		//virtual void scroll_(bool newObs) = 0;
		virtual void scroll_(Cube *cube) = 0;
		virtual void loadOSObs(Direction dir) = 0;	//Load offscreen Obstacles (for scrolling)
		virtual void loadOSMCs(Direction dir) = 0;	//Load offscreen miniCubes (for scrolling)
		int getBottomRow() const {return bottomRow;}
		void moveObs();
		void resetPlayer(Cube *cube);
		int getGameMode() {return gameMode;}
		bool getIsTwoPlayer() {return isTwoPlayer;}
		bool getForServer() {return forServer;}
		
		//Method to access
		list<Obstacle*>& getObstacles() {return obstacles;}
		
		//The following functions are used by the Obstacle
		//constructor to ensure a new obstacle is not placed on
		//top of an existing obstacle or minicube.
		set<pair<int, int>>& getObsCoords() {return obsCoords;}
		set<pair<int, int>>& getMiniCubes() {return miniCubes;}
		
		virtual ~World() {}
		
};

#endif /* World_hpp */
		
		
// Ref
// http://www.cplusplus.com/reference/utility/pair/pair/
// http://www.cplusplus.com/reference/set/set/find/
// https://www.geeksforgeeks.org/pair-in-cpp-stl/
// https://ubuntuforums.org/showthread.php?t=836043