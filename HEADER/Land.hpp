/********************************************************************
** Program name: Land.hpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Header file for Land class. The Land class is 
** 				one of three world types for the game. The colors
**				and obstacles are distinct for this world, along
**				with the player movement. The game "scrolls" the 
**				player along at a minimum speed (that increases
**				with game mode difficulty and elapsed time up to
**				a point) from left to right, and the cube's 
**				vertical position tends downward unless modified
**				by the player.
********************************************************************/

#ifndef Land_hpp
#define Land_hpp

#include "World.hpp"
#include "Game.hpp"

class Land : public World {
	public:
		Land(bool isTwoPlayer) : World(isTwoPlayer) {bottomRow = LINES - 5;};
		Land(int gameMode, bool isTwoPlayer, bool forServer = false);
		virtual void renderWorld(Cube *cube);
		virtual void scroll_(Cube *cube, Direction lockedDirection);
		virtual void loadOSObs(Direction dir);	//Load offscreen Obstacles (for scrolling)
		virtual void loadOSMCs(Direction dir);	//Load offscreen miniCubes (for scrolling)
};

#endif /* Land_hpp */
		
		
// Ref
// http://www.cplusplus.com/reference/utility/pair/pair/
// http://www.cplusplus.com/reference/set/set/find/