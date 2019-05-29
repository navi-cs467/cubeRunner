/********************************************************************
** Program name: Spaceship.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Source file for Spaceship class, a derived Obstacle.
********************************************************************/

#include "../HEADER/Spaceship.hpp"

Spaceship::Spaceship(World *world, Direction offScreen, 
			 int specificGraphic) : Obstacle() {
	
	isStationary = false;
	//curDirection = left;
	//sameDirectionMoveCount = 0;
	createObstacle(world, offScreen, specificGraphic);
	
	//Update size of graphic type array 
	//and length of longest graphic wstring
	gts = Spaceship::getGraphicLines()[gt].size();
	longestGS = 0;
	for(int i = 0; i < gts; i++)
		if(Spaceship::getGraphicLines()[gt][i].size() > longestGS)
			longestGS = Spaceship::getGraphicLines()[gt][i].size();
	
	//Update World's obsCoords and this Obstacle's
	//nonWSObsCoords with new graphic position
	world->updateObsCoords(this);
	
	//Initialize maxHits based on gameMode
	if(world->getGameMode() == EASY) maxHits = 2;
	else if(world->getGameMode() == NORMAL) maxHits = 3;
	else maxHits = 4;
	
	//Initialize hits ands mvsSinceLastHit
	hits = 0;
	mvsSinceLastHit = 0;
}

vector<vector<wstring>> Spaceship::getGraphicLines() {
	return Spaceship::_getGraphicLines();
}

vector<vector<wstring>> Spaceship::initializeVectorGraphics () {
	vector<vector<wstring>> tmpV {{L" \\_____",
								    L"==_____|>",
								    L" /"},
									
								   {L"  _____/",
								    L"<|_____==",
								    L"       \\"},
		
								  {L" /\\",
								   L"/  \\",
								   L"| u|",
								   L"| s|",
								   L"| a|",
								   L"/||\\"},
								   
								  {L" /\\",
								   L"/  \\",
								   L"| o|",
								   L"| s|",
								   L"| u|",
								   L"/||\\"},
								
								  {L"\\||/",
								   L"|  |",
								   L"|  |",
								   L"|__|",
								   L"\\ */",
								   L" \\/"}};
								  
	return tmpV;
}

vector<vector<wstring>> Spaceship::graphicLines = 
	Spaceship::initializeVectorGraphics();	  

//Color scheme	
int Spaceship::color = WHITE_BLACK;

// References
// https://ubuntuforums.org/showthread.php?t=836043
// https://stackoverflow.com/questions/17663186/initializing-a-two-dimensional-stdvector
// https://stackoverflow.com/questions/1115854/how-to-resolve-error-bad-index-fatal-index-file-corrupt-when-using-git
// https://stackoverflow.com/questions/56120664/converting-typeid-to-namespace-for-static-member-access-c?noredirect=1#comment98874550_56120664