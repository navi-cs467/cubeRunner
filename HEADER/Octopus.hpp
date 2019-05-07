/********************************************************************
** Program name: Octopus.hpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Header file for Octopus class, a derived Obstacle.
********************************************************************/

#ifndef Octopus_hpp
#define Octopus_hpp

#include "Obstacle.hpp"
#include "World.hpp"
#include "using.hpp"

class Obstacle;


class Octopus : public Obstacle {
	private:
		static vector<vector<wstring>> graphicLines;
		int color;
		Direction curDirection;
		int sameDirectionMoveCount;
	
	public:
		Octopus(World *world, Direction offScreen = none,
			  int specificGraphic = -1);
		static vector<vector<wstring>> getGraphicLines() {return graphicLines;}
		int getColor() {return color;}
		static vector<vector<wstring>> initializeVectorGraphics();
		Direction getDirection() {return curDirection;}
		void setDirection(Direction newDirection) {curDirection = newDirection;}
		virtual ~Octopus() {}
		
};
								 
#endif /* Octopus_hpp */

// References
// http://www.cplusplus.com/reference/vector/vector/vector/
// https://ubuntuforums.org/showthread.php?t=836043