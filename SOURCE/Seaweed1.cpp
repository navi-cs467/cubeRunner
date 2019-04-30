/********************************************************************
** Program name: Seaweed.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Source file for Seaweed class, a derived Obstacle.
********************************************************************/

#include "../HEADER/Seaweed1.hpp"

Seaweed1::Seaweed1(World *world) : Obstacle() {
	
	isStationary = true;
	createObstacle(world, graphicLines, lengthGL);
}

string Seaweed1::graphicLines[] = {string("(   )"),
								   string(" ) ("),
								   string(" ( )"),
								   string(" ) ("),
								   string("(   )")};

int Seaweed1::lengthGL = 5;

//  Not working as intended...							   
/* int Seaweed1::lengthGL = 
	sizeof(Seaweed1::getGraphicLines()) / sizeof(string); */
	
int Seaweed1::color = GREEN_BLUE;

// References
// https://ubuntuforums.org/showthread.php?t=836043