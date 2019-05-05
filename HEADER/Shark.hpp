/********************************************************************
** Program name: Shark.hpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Header file for Shark class, a derived Obstacle.
********************************************************************/

#ifndef Shark_hpp
#define Shark_hpp

#include "Obstacle.hpp"
#include "World.hpp"
#include "using.hpp"

class Obstacle;


class Shark : public Obstacle {
	private:
		static vector<vector<string>> graphicLines;
		static int color;
		Direction curDirection;
		int sameDirectionMoveCount;
	
	public:
		Shark(World *world, Direction offScreen = none,
			  int specificGraphic = -1);
		static vector<vector<string>> getGraphicLines() {return graphicLines;}
		static int getColor() {return color;}
		static vector<vector<string>> initializeVectorGraphics();
		Direction getDirection() {return curDirection;}
		void setDirection(Direction newDirection) {curDirection = newDirection;}
		virtual ~Shark() {}
		
};
								 
#endif /* Shark_hpp */

// References
// http://www.cplusplus.com/reference/vector/vector/vector/
// https://ubuntuforums.org/showthread.php?t=836043