/********************************************************************
** Program name: Bird.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Source file for Bird class, a derived Obstacle.
********************************************************************/

#include "../HEADER/Bird.hpp"

Bird::Bird(World *world, Direction offScreen, 
			 int specificGraphic) : Obstacle() {
	
	isStationary = false;
	//curDirection = left;
	//sameDirectionMoveCount = 0;
	color = rand() % 5 + 16;
	createObstacle(world, offScreen, specificGraphic);
	
	//Update size of graphic type array 
	//and length of longest graphic wstring
	gts = Bird::getGraphicLines()[gt].size();
	longestGS = 0;
	for(int i = 0; i < gts; i++)
		if(Bird::getGraphicLines()[gt][i].size() > longestGS)
			longestGS = Bird::getGraphicLines()[gt][i].size();
	
	//Update World's obsCoords and this Obstacle's
	//nonWSObsCoords with new graphic position
	world->updateObsCoords(this);
	
	//Initialize maxHits based on gameMode
	if(world->getGameMode() == EASY) maxHits = 1;
	else if(world->getGameMode() == NORMAL) maxHits = 2;
	else maxHits = 2;
	
	//Initialize hits ands mvsSinceLastHit
	hits = 0;
	mvsSinceLastHit = 0;
}

vector<vector<wstring>> Bird::getGraphicLines() {
	return Bird::_getGraphicLines();
}

vector<vector<wstring>> Bird::initializeVectorGraphics () {
	vector<vector<wstring>> tmpV {{L"    /\\  __  ",
								   L">--/  \\/__O>",
								   L">--\\  /     ",
								   L"    \\/      "},
								  
								  {L"  __  /\\    ",
								   L"<O__\\/  \\--<",
								   L"     \\  /--<",
								   L"      \\/    "}};
								  
	return tmpV;
}

vector<vector<wstring>> Bird::graphicLines = 
	Bird::initializeVectorGraphics();	  


// References
// https://ubuntuforums.org/showthread.php?t=836043
// https://stackoverflow.com/questions/17663186/initializing-a-two-dimensional-stdvector
// https://stackoverflow.com/questions/1115854/how-to-resolve-error-bad-index-fatal-index-file-corrupt-when-using-git
// https://stackoverflow.com/questions/56120664/converting-typeid-to-namespace-for-static-member-access-c?noredirect=1#comment98874550_56120664