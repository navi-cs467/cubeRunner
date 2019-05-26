/********************************************************************
** Program name: Bat.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Source file for Bat class, a derived Obstacle.
********************************************************************/

#include "../HEADER/Bat.hpp"

Bat::Bat(World *world, Direction offScreen, 
			 int specificGraphic) : Obstacle() {
	
	isStationary = false;
	//curDirection = left;
	//sameDirectionMoveCount = 0;
	createObstacle(world, offScreen, specificGraphic);
	
	//Update size of graphic type array 
	//and length of longest graphic string
	gts = Bat::getGraphicLines()[gt].size();
	longestGS = 0;
	for(int i = 0; i < gts; i++)
		if(Bat::getGraphicLines()[gt][i].size() > longestGS)
			longestGS = Bat::getGraphicLines()[gt][i].size();
		
	//Update World's obsCoords and this Obstacle's
	//nonWSObsCoords with new graphic position
	world->updateObsCoords(this);
	
	//Initialize maxHits based on gameMode
	if(world->getGameMode() == EASY) maxHits = 1;
	else if(world->getGameMode() == NORMAL) maxHits = 2;
	else maxHits = 3;
	
	//Initialize hits and mvsSinceLastHit
	hits = 0;
	mvsSinceLastHit = 0;
}

vector<vector<wstring>> Bat::initializeVectorGraphics () {
	vector<vector<wstring>> tmpV {{L" ^..^ ",
								   L"<, ,> "},
								
								  {L"^..^ ",
								   L"<, ,> "}};
								  
	return tmpV;
}

vector<vector<wstring>> Bat::getGraphicLines() {
	return Bat::_getGraphicLines();
}

vector<vector<wstring>> Bat::graphicLines = 
	Bat::initializeVectorGraphics();	

//Color scheme	
int Bat::color = BLACK_WHITE;	


// References
// https://ubuntuforums.org/showthread.php?t=836043
// https://stackoverflow.com/questions/17663186/initializing-a-two-dimensional-stdvector
// https://stackoverflow.com/questions/1115854/how-to-resolve-error-bad-index-fatal-index-file-corrupt-when-using-git