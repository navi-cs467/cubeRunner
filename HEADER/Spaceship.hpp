/********************************************************************
** Program name: Spaceship.hpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Header file for Spaceship class, a derived Obstacle.
********************************************************************/

#ifndef Spaceship_hpp
#define Spaceship_hpp

#include "Obstacle.hpp"
#include "World.hpp"
#include "using.hpp"

class Obstacle;


class Spaceship : public Obstacle {
	private:
		static vector<vector<wstring>> graphicLines;
		static int color;
		//Direction curDirection;
		//int sameDirectionMoveCount;
	
	public:
		Spaceship(int type, int posX, int posY, int gt, int gts,
				int colorOrColorSeed, int hits, int gm) :
			Obstacle(type, posX, posY, gt, gts, colorOrColorSeed, hits, gm) 
			{isStationary = false;}			//Constructor for client
		Spaceship(World *world, Direction offScreen = none,
			  int specificGraphic = -1);				//Constructor for server and 1-player
		static vector<vector<wstring>> _getGraphicLines() {return graphicLines;}
		static int getColor() {return color;}
		static vector<vector<wstring>> initializeVectorGraphics();
		//Direction getDirection() {return curDirection;}
		//void setDirection(Direction newDirection) {curDirection = newDirection;}
		virtual vector<vector<wstring>> getGraphicLines() override;
		virtual ~Spaceship() {}
		
};
								 
#endif /* Spaceship_hpp */

// References
// http://www.cplusplus.com/reference/vector/vector/vector/
// https://ubuntuforums.org/showthread.php?t=836043