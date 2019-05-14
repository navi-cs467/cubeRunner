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
		static vector<vector<wstring>> graphicLines;
		static int color;
	
	public:
		Seaweed(int posX, int posY, int gt, int gts) :
			Obstacle(posX, posY, gt, gts) {}				//Constructor for client
		Seaweed(World *world, Direction offScreen = none, 
				int specificGraphic = -1);					//Constructor for server and 1-player
		static vector<vector<wstring>> getGraphicLines() {return graphicLines;}
		static int getColor() {return color;}
		static vector<vector<wstring>> initializeVectorGraphics();
		virtual ~Seaweed() {}
		
};
								 
#endif /* Seaweed_hpp */

// References
// http://www.cplusplus.com/reference/vector/vector/vector/
// https://ubuntuforums.org/showthread.php?t=836043