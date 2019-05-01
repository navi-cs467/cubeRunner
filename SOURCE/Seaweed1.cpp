/********************************************************************
** Program name: Seaweed.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Source file for Seaweed class, a derived Obstacle.
********************************************************************/

#include "../HEADER/Seaweed1.hpp"

Seaweed1::Seaweed1(World *world, int specificGraphic) : Obstacle() {
	
	isStationary = true;
	createObstacle(world, graphicLines, specificGraphic);
}

vector<vector<string>> Seaweed1::initializeVectorGraphics () {
	vector<vector<string>> tmpV {{string("(   )"),
								  string(" ) ("),
								  string(" ( )"),
								  string(" ) ("),
								  string("(   )")},
								
								 {string(" |o |o"),
								  string("  \\/"),
								  string("  /o"),
								  string("o/"),
								  string("|"),
								  string("|")},
								 
								 {string(" |o |o"),
								  string("  \\/"),
								  string("  o\\"),
								  string("    \\o"),
								  string("    |"),
								  string("    |")}};
								  
	return tmpV;
}

vector<vector<string>> Seaweed1::graphicLines = 
	Seaweed1::initializeVectorGraphics();	  

//Color scheme	
int Seaweed1::color = GREEN_BLUE;

// References
// https://ubuntuforums.org/showthread.php?t=836043