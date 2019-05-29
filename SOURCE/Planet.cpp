/********************************************************************
** Program name: Planet.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Source file for Planet class, a derived Obstacle.
********************************************************************/

#include "../HEADER/Planet.hpp"

Planet::Planet(World *world, Direction offScreen,
			 int specificGraphic) : Obstacle() {
	
	isStationary = true;
	createObstacle(world, offScreen, specificGraphic);
	
	//Update size of graphic type array 
	//and length of longest graphic string
	gts = Planet::getGraphicLines()[gt].size();
	longestGS = 0;
	for(int i = 0; i < gts; i++)
		if(Planet::getGraphicLines()[gt][i].size() > longestGS)
			longestGS = Planet::getGraphicLines()[gt][i].size();
		
	//Update World's obsCoords and this Obstacle's
	//nonWSObsCoords with new graphic position
	world->updateObsCoords(this);
}

vector<vector<wstring>> Planet::initializeVectorGraphics () {
	vector<vector<wstring>> tmpV {{L"┌.-¸_ ▄▄▄▄",
								   L" ╚`:██░░░░██",
								   L"   █:´░░░░░░█",
								   L"   █░░░░`:░░█._",
								   L"    ██░░░░██:`‗",
								   L"      ▀▀▀▀"},
								
								  {L"      ▄▄▄▄ _¸-.┌",
								   L"    ██░░░░██:`╚",
								   L"   █░░░░░░:´█",
								   L" _.█░`:░░░░░█",
								   L" ‗`:██░░░░██",
								   L"      ▀▀▀▀"}};
								  
	return tmpV;
}

vector<vector<wstring>> Planet::getGraphicLines() {
	return Planet::_getGraphicLines();
}

vector<vector<wstring>> Planet::graphicLines = 
	Planet::initializeVectorGraphics();	  

//Color scheme	
int Planet::colorSeed = rand() % 6 + 1;

// References
// https://ubuntuforums.org/showthread.php?t=836043
// https://stackoverflow.com/questions/17663186/initializing-a-two-dimensional-stdvector
// https://stackoverflow.com/questions/1115854/how-to-resolve-error-bad-index-fatal-index-file-corrupt-when-using-git