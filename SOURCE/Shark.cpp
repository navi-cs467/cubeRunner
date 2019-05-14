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
	//curDirection = left;
	//sameDirectionMoveCount = 0;
	createObstacle(world, offScreen, specificGraphic);
	
	//Update size of graphic type array 
	//and length of longest graphic wstring
	gts = Shark::getGraphicLines()[gt].size();
	longestGS = 0;
	for(int i = 0; i < gts; i++)
		if(Shark::getGraphicLines()[gt][i].size() > longestGS)
			longestGS = Shark::getGraphicLines()[gt][i].size();
	
	//Update World's obsCoords with new graphic position
	world->updateObsCoords(this);
}

vector<vector<wstring>> Shark::getGraphicLines() {
	return Shark::_getGraphicLines();
}

vector<vector<wstring>> Shark::initializeVectorGraphics () {
	vector<vector<wstring>> tmpV {{L"            ,`\\",
								   L",\\      ____)  \\________________",
								   L"\\ `────´                       /",
								   L"{ ,──._         « « «  <o  ___/",
								   L" `     `─.__    ´ ´ ´     /´´´",
								   L"            \\/---|  )--.  \\.",
								   L"                 ;,´    `-´"},
								
								 { L"                 /´,",
								   L"________________/  (____      /,",
								   L"\\                       `────´ /",
								   L" \\___  o>  » » »         .──, }",
								   L"  ```\\     ` ` `    __.─´",
								   L"    ./  .--(  |---\\/",
								   L"     `-´    `,;"}};
								  
	return tmpV;
}

vector<vector<wstring>> Shark::graphicLines = 
	Shark::initializeVectorGraphics();	  

//Color scheme	
int Shark::color = WHITE_BLUE;

// References
// https://ubuntuforums.org/showthread.php?t=836043
// https://stackoverflow.com/questions/17663186/initializing-a-two-dimensional-stdvector
// https://stackoverflow.com/questions/1115854/how-to-resolve-error-bad-index-fatal-index-file-corrupt-when-using-git
// https://stackoverflow.com/questions/56120664/converting-typeid-to-namespace-for-static-member-access-c?noredirect=1#comment98874550_56120664