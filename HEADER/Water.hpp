/********************************************************************
** Program name: Water.hpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Header file for Water class. The Water class is 
** 				one of three world types for the game. The colors
**				and obstacles are distinct for this world, along
**				with the player movement. The game "scrolls" the 
**				player along at a minimum speed (that increases
**				with game mode difficulty and elapsed time up to
**				a point) from left to right, and the cube's 
**				vertical position remains constant until modified
**				by the player.
********************************************************************/

#ifndef Water_hpp
#define Water_hpp

#include "World.hpp"
#include "Game.hpp"

class Water : public World {
	public:
		Water(bool isTwoPlayer) : World(isTwoPlayer) {};
		Water(int gameMode, bool isTwoPlayer, bool forServer = false);
		virtual void renderWorld(Cube *cube);
		//virtual void scroll_(bool newObs);
		virtual void scroll_();
		virtual void loadOSObs();	//Load offscreen Obstacles (for scrolling)
		virtual void loadOSMCs();	//Load offscreen miniCubes (for scrolling)
};

#endif /* Water_hpp */
		
		
// Ref
// http://www.cplusplus.com/reference/utility/pair/pair/
// http://www.cplusplus.com/reference/set/set/find/