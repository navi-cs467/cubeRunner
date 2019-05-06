/********************************************************************
** Program name: Octopus.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Source file for Octopus class, a derived Obstacle.
********************************************************************/

#include "../HEADER/Octopus.hpp"

Octopus::Octopus(World *world, Direction offScreen, 
			 int specificGraphic) : Obstacle() {
	
	isStationary = false;
	curDirection = left;
	sameDirectionMoveCount = 0;
	color = rand() % 4 + 31;
	createObstacle(world, graphicLines, offScreen, specificGraphic);
}

vector<vector<string>> Octopus::initializeVectorGraphics () {
	vector<vector<string>> tmpV {{string("  oOOo"),
								  string(" O    O"),
								  string("O  o o O"),
								  string(" OooooO"),
								  string("//// \\\\\\\\"),
								  string("\\\\\\\\ ////"),
								  string("//// \\\\\\\\")},
								
								 {string("  oOOo"),
								  string(" O    O"),
								  string("O o o  O"),
								  string(" OooooO"),
								  string("//// \\\\\\\\"),
								  string("\\\\\\\\ ////"),
								  string("//// \\\\\\\\")}};
								  
	return tmpV;
}

vector<vector<string>> Octopus::graphicLines = 
	Octopus::initializeVectorGraphics();	  


// References
// https://ubuntuforums.org/showthread.php?t=836043
// https://stackoverflow.com/questions/17663186/initializing-a-two-dimensional-stdvector
// https://stackoverflow.com/questions/1115854/how-to-resolve-error-bad-index-fatal-index-file-corrupt-when-using-git