/********************************************************************
** Program name: Seaweed1.hpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Header file for Seaweed1 class, a derived Obstacle.
********************************************************************/

#ifndef Seaweed_hpp
#define Seaweed_hpp

#include "Obstacle.hpp"
#include "using.hpp"


class Seaweed1 : public Obstacle {
	private:
		static string graphicLines[];
		static int lengthGL;
	
	public:
		Seaweed1(World *world);
		static string* getGraphicLines() {return graphicLines;}
		static int getLengthGL();
};

string Seaweed1::graphicLines[] = {string("(   )"),
								   string(" ) ("),
								   string(" ( )"),
								   string(" ) ("),
								   string("(   )")};
								   
int Seaweed1::lengthGL = 
	sizeof(Seaweed1::getGraphicLines()) / sizeof(string);
								 
#endif /* Seaweed_hpp */

// References
// http://www.cplusplus.com/reference/vector/vector/vector/