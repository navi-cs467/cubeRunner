/********************************************************************
** Program name: Seaweed.hpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Header file for Seaweed class, a derived Obstacle.
********************************************************************/

#ifndef Seaweed_hpp
#define Seaweed_hpp

#include "Obstacle.hpp"
#include "World.hpp"
#include "using.hpp"

class Obstacle;


class Seaweed : public Obstacle {
	private:
		static vector<vector<string>> graphicLines;
		static int color;
	
	public:
		Seaweed(World *world, Direction offScreen = none, 
				int specificGraphic = -1);
		static vector<vector<string>> getGraphicLines() {return graphicLines;}
		static int getColor() {return color;}
		static vector<vector<string>> initializeVectorGraphics();
		virtual ~Seaweed() {}
		
};
								 
#endif /* Seaweed_hpp */

// References
// http://www.cplusplus.com/reference/vector/vector/vector/
// https://ubuntuforums.org/showthread.php?t=836043