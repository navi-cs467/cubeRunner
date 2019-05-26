/********************************************************************
** Program name: Rock.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Source file for Rock class, a derived Obstacle.
********************************************************************/

#include "../HEADER/Rock.hpp"

Rock::Rock(World *world, Direction offScreen,
			 int specificGraphic) : Obstacle() {
	
	isStationary = true;
	createObstacle(world, offScreen, specificGraphic);
	
	//Update size of graphic type array 
	//and length of longest graphic string
	gts = Rock::getGraphicLines()[gt].size();
	longestGS = 0;
	for(int i = 0; i < gts; i++)
		if(Rock::getGraphicLines()[gt][i].size() > longestGS)
			longestGS = Rock::getGraphicLines()[gt][i].size();
		
	//Update World's obsCoords and this Obstacle's
	//nonWSObsCoords with new graphic position
	world->updateObsCoords(this);
}

vector<vector<wstring>> Rock::initializeVectorGraphics () {
	vector<vector<wstring>> tmpV {{L" __ ",
								   L"/  \\",
								   L"\\__/"},
								
								  {L"    _____ ",
								   L"___/   __\\",
								   L"\\     \\_   ",
								   L" \\     /   ",
								   L"/__   \\   ",
								   L"   \\__/   "},
								   
								  {L"    _____         ",
								   L"   /     \\   ___  ",
								   L"  /       \\_/   \\ ",
								   L"  \\              \\",
								   L"   \\___      /\\__/",
								   L"       \\____/     "}};
								  
	return tmpV;
}

vector<vector<wstring>> Rock::getGraphicLines() {
	return Rock::_getGraphicLines();
}

vector<vector<wstring>> Rock::graphicLines = 
	Rock::initializeVectorGraphics();	  

//Color scheme	
int Rock::color = BLACK_WHITE;

// References
// https://ubuntuforums.org/showthread.php?t=836043
// https://stackoverflow.com/questions/17663186/initializing-a-two-dimensional-stdvector
// https://stackoverflow.com/questions/1115854/how-to-resolve-error-bad-index-fatal-index-file-corrupt-when-using-git