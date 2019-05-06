/********************************************************************
** Program name: Coral.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Source file for Coral class, a derived Obstacle.
********************************************************************/

#include "../HEADER/Coral.hpp"

Coral::Coral(World *world, Direction offScreen,
			 int specificGraphic) : Obstacle() {
	
	isStationary = true;
	createObstacle(world, graphicLines, offScreen, specificGraphic);
	
	//Update size of graphic type array 
	//and length of longest graphic string
	gts = Coral::getGraphicLines()[gt].size();
	longestGS = 0;
	for(int i = 0; i < gts; i++)
		if(Coral::getGraphicLines()[gt][i].size() > longestGS)
			longestGS = Coral::getGraphicLines()[gt][i].size();
		
	//Update World's obsCoords with new graphic position
	world->updateObsCoords(this);
}

vector<vector<string>> Coral::initializeVectorGraphics () {
	vector<vector<string>> tmpV {{string(" /\\  /"),
								  string("/  \\/      /"),
								  string("\\         /"),
								  string(" \\     /\\/"),
								  string("  \\___/  \\"),
								  string("  /"),
								  string(" /")},
								
								 {string("      \\  /\\"),
								  string("\\      \\/  \\"),
								  string(" \\         /"),
								  string("  \\/\\     /"),
								  string("  /	 \\___/"),
								  string("         \\"),
								  string("          \\")}};
								  
	return tmpV;
}

vector<vector<string>> Coral::graphicLines = 
	Coral::initializeVectorGraphics();	  

//Color scheme	
int Coral::colorSeed = rand() % 5 + 30;

// References
// https://ubuntuforums.org/showthread.php?t=836043
// https://stackoverflow.com/questions/17663186/initializing-a-two-dimensional-stdvector
// https://stackoverflow.com/questions/1115854/how-to-resolve-error-bad-index-fatal-index-file-corrupt-when-using-git