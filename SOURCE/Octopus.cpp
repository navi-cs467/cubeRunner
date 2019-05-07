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
	
	//Update size of graphic type array 
	//and length of longest graphic string
	gts = Octopus::getGraphicLines()[gt].size();
	longestGS = 0;
	for(int i = 0; i < gts; i++)
		if(Octopus::getGraphicLines()[gt][i].size() > longestGS)
			longestGS = Octopus::getGraphicLines()[gt][i].size();
		
	//Update World's obsCoords with new graphic position
	world->updateObsCoords(this);
}

vector<vector<wstring>> Octopus::initializeVectorGraphics () {
	vector<vector<wstring>> tmpV {{L"  oOOo",
								   L" O    O",
								   L"O  o o O",
								   L" OooooO",
								   L"//// \\\\\\\\",
								   L"\\\\\\\\ ////",
								   L"//// \\\\\\\\"},
								
								  {L"  oOOo",
								   L" O    O",
								   L"O o o  O",
								   L" OooooO",
								   L"//// \\\\\\\\",
								   L"\\\\\\\\ ////",
								   L"//// \\\\\\\\"}};
								  
	return tmpV;
}

vector<vector<wstring>> Octopus::graphicLines = 
	Octopus::initializeVectorGraphics();	  


// References
// https://ubuntuforums.org/showthread.php?t=836043
// https://stackoverflow.com/questions/17663186/initializing-a-two-dimensional-stdvector
// https://stackoverflow.com/questions/1115854/how-to-resolve-error-bad-index-fatal-index-file-corrupt-when-using-git