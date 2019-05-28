/********************************************************************
** Program name: Space.hpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Header file for Space class. The Space class is 
** 				one of three world types for the game. The colors
**				and obstacles are distinct for this world, along
**				with the player movement. The game "scrolls" the 
**				player along at a minimum speed (that increases
**				with game mode difficulty and elapsed time up to
**				a point) from left to right, and the scroll 
**				direction equals the player's current move direction.
********************************************************************/

#ifndef Space_hpp
#define Space_hpp

#include "World.hpp"
#include "Game.hpp"

class Space : public World {
	public:
		Space(bool isTwoPlayer) : World(isTwoPlayer) {bottomRow = LINES - 5;};
		Space(int gameMode, bool isTwoPlayer, bool forServer = false);
		virtual void renderWorld(Cube *cube);
		//virtual void scroll_(bool newObs);
		virtual void scroll_(Cube *cube);
		virtual void loadOSObs(Direction dir);	//Load offscreen Obstacles (for scrolling)
		virtual void loadOSMCs();				//Load offscreen miniCubes (for scrolling)
};

#endif /* Space_hpp */
		
		
// Ref
// http://www.cplusplus.com/reference/utility/pair/pair/
// http://www.cplusplus.com/reference/set/set/find/