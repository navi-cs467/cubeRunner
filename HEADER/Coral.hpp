/********************************************************************
** Program name: Coral.hpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Header file for Coral class, a derived Obstacle.
********************************************************************/

#ifndef Coral_hpp
#define Coral_hpp

#include "Obstacle.hpp"
#include "World.hpp"
#include "using.hpp"

class Obstacle;


class Coral : public Obstacle {
	private:
		static vector<vector<wstring>> graphicLines;
		static int colorSeed;
	
	public:
		Coral(int posX, int posY, int gt, int gts) :		
			Obstacle(posX, posY, gt, gts) {}			//Constructor for client	
		Coral(World *world, Direction offScreen = none, 
			int specificGraphic = -1);					//Constructor for server and 1-player
		static vector<vector<wstring>> getGraphicLines() {return graphicLines;}
		static int getColorSeed() {return colorSeed;}
		static vector<vector<wstring>> initializeVectorGraphics();
		virtual ~Coral() {}
		
};
								 
#endif /* Coral_hpp */

// References
// http://www.cplusplus.com/reference/vector/vector/vector/
// https://ubuntuforums.org/showthread.php?t=836043