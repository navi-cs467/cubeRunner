/********************************************************************
** Program name: Shark.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Source file for Shark class, a derived Obstacle.
********************************************************************/

#include "../HEADER/Shark.hpp"

Shark::Shark(World *world, Direction offScreen, 
			 int specificGraphic) : Obstacle() {
	
	isStationary = false;
	curDirection = left;
	sameDirectionMoveCount = 0;
	createObstacle(world, graphicLines, offScreen, specificGraphic);
	
	//Update size of graphic type array 
	//and length of longest graphic string
	gts = Shark::getGraphicLines()[gt].size();
	longestGS = 0;
	for(int i = 0; i < gts; i++)
		if(Shark::getGraphicLines()[gt][i].size() > longestGS)
			longestGS = Shark::getGraphicLines()[gt][i].size();
	
	//Update World's obsCoords with new graphic position
	world->updateObsCoords(this);
}

vector<vector<string>> Shark::initializeVectorGraphics () {
	vector<vector<string>> tmpV {{string("            ,`\\"),
								  string(",\\      ____)  \\________________"),
								  string("\\ `────´                       /"),
								  string("{ ,──._         « « «  <o  ___/"),
								  string(" `     `─.__    ´ ´ ´     /´´´"),
								  string("            \\/---|  )--.  \\."),
								  string("                 ;,´    `-´")},
								
								 {string("                 /´,"),
								  string("________________/  (____      /,"),
								  string("\\                       `────´ /"),
								  string(" \\___  o>  » » »         .──, }"),
								  string("  ```\\     ` ` `    __.─´"),
								  string("    ./  .--(  |---\\/"),
								  string("     `-´    `,;")}};
								  
	return tmpV;
}

vector<vector<string>> Shark::graphicLines = 
	Shark::initializeVectorGraphics();	  

//Color scheme	
int Shark::color = WHITE_BLUE;

// References
// https://ubuntuforums.org/showthread.php?t=836043
// https://stackoverflow.com/questions/17663186/initializing-a-two-dimensional-stdvector
// https://stackoverflow.com/questions/1115854/how-to-resolve-error-bad-index-fatal-index-file-corrupt-when-using-git