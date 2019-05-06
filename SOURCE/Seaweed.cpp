/********************************************************************
** Program name: Seaweed.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Source file for Seaweed class, a derived Obstacle.
********************************************************************/

#include "../HEADER/Seaweed.hpp"

Seaweed::Seaweed(World *world, Direction offScreen,
				 int specificGraphic) : Obstacle() {
	
	isStationary = true;
	createObstacle(world, graphicLines, offScreen, specificGraphic);
	
	//Update size of graphic type array 
	//and length of longest graphic string
	gts = Seaweed::getGraphicLines()[gt].size();
	longestGS = 0;
	for(int i = 0; i < gts; i++)
		if(Seaweed::getGraphicLines()[gt][i].size() > longestGS)
			longestGS = Seaweed::getGraphicLines()[gt][i].size();
		
	//Update World's obsCoords with new graphic position
	world->updateObsCoords(this);
}

vector<vector<string>> Seaweed::initializeVectorGraphics () {
	vector<vector<string>> tmpV {{string("\\\\   //"),
								  string("//   \\\\"),
								  string("\\\\   //"),
								  string(" \\\\ //"),
								  string(" // \\\\"),
								  string(" \\\\ //"),
								  string(" // \\\\"),
								  string(" \\\\ //"),
								  string(" |||||")},
								
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

vector<vector<string>> Seaweed::graphicLines = 
	Seaweed::initializeVectorGraphics();	  

//Color scheme	
int Seaweed::color = GREEN_BLUE;

// References
// https://ubuntuforums.org/showthread.php?t=836043
// https://stackoverflow.com/questions/17663186/initializing-a-two-dimensional-stdvector
// https://stackoverflow.com/questions/1115854/how-to-resolve-error-bad-index-fatal-index-file-corrupt-when-using-git