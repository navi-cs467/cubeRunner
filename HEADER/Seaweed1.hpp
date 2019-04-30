/********************************************************************
** Program name: Seaweed1.hpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Header file for Seaweed1 class, a derived Obstacle.
********************************************************************/

#ifndef Seaweed1_hpp
#define Seaweed1_hpp

#include "Obstacle.hpp"
#include "World.hpp"
#include "using.hpp"

class Obstacle;


class Seaweed1 : public Obstacle {
	private:
		static string graphicLines[];
		static int lengthGL;		//Number of "strings" or rows 
									//that make up the graphicLines[]
		static int color;
	
	public:
		Seaweed1(World *world);
		static string* getGraphicLines() {return graphicLines;}
		static int getLengthGL() {return lengthGL;};
		static int getColor() {return color;}
		virtual ~Seaweed1() {}
		
};
								 
#endif /* Seaweed1_hpp */

// References
// http://www.cplusplus.com/reference/vector/vector/vector/
// https://ubuntuforums.org/showthread.php?t=836043