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
	//curDirection = left;
	//sameDirectionMoveCount = 0;
	color = rand() % 3 + 32;
	createObstacle(world, offScreen, specificGraphic);
	
	//Update size of graphic type array 
	//and length of longest graphic string
	gts = Octopus::getGraphicLines()[gt].size();
	longestGS = 0;
	for(int i = 0; i < gts; i++)
		if(Octopus::getGraphicLines()[gt][i].size() > longestGS)
			longestGS = Octopus::getGraphicLines()[gt][i].size();
		
	//Update World's obsCoords and this Obstacle's
	//nonWSObsCoords with new graphic position
	world->updateObsCoords(this);
	
	//Initialize maxHits based on gameMode
	if(world->getGameMode() == EASY) maxHits = 2;
	else if(world->getGameMode() == NORMAL) maxHits = 3;
	else maxHits = 4;
	
	//Initialize hits and mvsSinceLastHit
	hits = 0;
	mvsSinceLastHit = 0;
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

vector<vector<wstring>> Octopus::getGraphicLines() {
	return Octopus::_getGraphicLines();
}

vector<vector<wstring>> Octopus::graphicLines = 
	Octopus::initializeVectorGraphics();	  


// References
// https://ubuntuforums.org/showthread.php?t=836043
// https://stackoverflow.com/questions/17663186/initializing-a-two-dimensional-stdvector
// https://stackoverflow.com/questions/1115854/how-to-resolve-error-bad-index-fatal-index-file-corrupt-when-using-git