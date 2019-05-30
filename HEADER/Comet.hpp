/********************************************************************
** Program name: Comet.hpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Header file for Comet class, a derived Obstacle.
********************************************************************/

#ifndef Comet_hpp
#define Comet_hpp

#include "Obstacle.hpp"
#include "World.hpp"
#include "using.hpp"

class Obstacle;


class Comet : public Obstacle {
	private:
		static vector<vector<wstring>> graphicLines;
		int color;
		//Direction curDirection;
		//int sameDirectionMoveCount;
	
	public:
		Comet(int type, int posX, int posY, int gt, int gts,
				int colorOrColorSeed, int hits, int gm) :
			Obstacle(type, posX, posY, gt, gts, colorOrColorSeed, hits, gm) 
			{
				color = colorOrColorSeed;
				isStationary = false;
			}		//Constructor for client
		Comet(World *world, Direction offScreen = none,
			  int specificGraphic = -1);					//Constructor for server and 1-player
		static vector<vector<wstring>> _getGraphicLines() {return graphicLines;}
		int getColor() {return color;}
		static vector<vector<wstring>> initializeVectorGraphics();
		//Direction getDirection() {return curDirection;}
		//void setDirection(Direction newDirection) {curDirection = newDirection;}
		virtual vector<vector<wstring>> getGraphicLines() override;
		virtual void move(World *world) override;
		virtual ~Comet() {}
		
};
								 
#endif /* Comet_hpp */

// References
// http://www.cplusplus.com/reference/vector/vector/vector/
// https://ubuntuforums.org/showthread.php?t=836043