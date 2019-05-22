/********************************************************************
** Program name: Rock.hpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Header file for Rock class, a derived Obstacle.
********************************************************************/

#ifndef Rock_hpp
#define Rock_hpp

#include "Obstacle.hpp"
#include "World.hpp"
#include "using.hpp"

class Obstacle;


class Rock : public Obstacle {
	private:
		static vector<vector<wstring>> graphicLines;
		static int color;
	
	public:
		Rock(int type, int posX, int posY, int gt, int gts, int gm) :		
			Obstacle(type, posX, posY, gt, gts, gm) {}			//Constructor for client	
		Rock(World *world, Direction offScreen = none, 
			int specificGraphic = -1);					//Constructor for server and 1-player
		static vector<vector<wstring>> _getGraphicLines() {return graphicLines;}
		static int getColor() {return color;}
		static vector<vector<wstring>> initializeVectorGraphics();
		virtual vector<vector<wstring>> getGraphicLines() override;
		virtual ~Rock() {}
		
};
								 
#endif /* Rock_hpp */

// References
// http://www.cplusplus.com/reference/vector/vector/vector/
// https://ubuntuforums.org/showthread.php?t=836043