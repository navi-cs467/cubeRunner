/********************************************************************
** Program name: Asteroid.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Source file for Asteroid class, a derived Obstacle.
********************************************************************/

#include "../HEADER/Asteroid.hpp"
#include "../HEADER/World.hpp"

Asteroid::Asteroid(World *world, Direction offScreen,
				 int specificGraphic) : Obstacle() {
	
	isStationary = true;
	
	createObstacle(world, offScreen, specificGraphic);
	
	//Update size of graphic type array 
	//and length of longest graphic string
	gts = Asteroid::getGraphicLines()[gt].size();
	longestGS = 0;
	for(int i = 0; i < gts; i++)
		if(Asteroid::getGraphicLines()[gt][i].size() > longestGS)
			longestGS = Asteroid::getGraphicLines()[gt][i].size();
		
	//Update World's obsCoords and this Obstacle's
	//nonWSObsCoords with new graphic position
	world->updateObsCoords(this);
}

vector<vector<wstring>> Asteroid::initializeVectorGraphics () {
	/* vector<vector<wstring>> tmpV {{L"     /--//",
								   L" _-/-/\\_--/",
								   L"/--__\\\\\\\\-",
								   L"   ----//--",
								   L" // \\\\"},
								
								  {L"--//\\\\",
								   L"--**==/\\/\\",
								   L"-_-_-**\\/**\\",
								   L"--////\\\\\\//*--",
								   L"-\\-\\-\\-\\",
								   L"****\\\\////----"},
								 
								  {L"**----**__--",
								   L"-*-*",
								   L"**\\\\//\\ __--",
								   L"-----\\\\",
								   L"------**",
								   L"****"}}; */
	
	//Generate random Asteroid graphics according to the
	//number of graphic types specified in NUM_ASTEROID_TYPES
	//(see constants.hpp)
	vector<vector<wstring>> tmpV(NUM_ASTEROID_TYPES);
	for(int i = 0; i < NUM_ASTEROID_TYPES; i++) {
		int randSize = rand() % 4 + 3;
		for(int j = 0; j < randSize; j++) {
			int randStrLength = rand() % 8 + 3;
			wstring tmpStr;
			for(int k = 0; k < randStrLength; k++) {
				int randomAsteroidChar = rand() % 7;
				if(randomAsteroidChar == 0)
					tmpStr.push_back(' ');
				else if (randomAsteroidChar == 1)
					tmpStr.push_back('/');
				else if (randomAsteroidChar == 2)
					tmpStr.push_back('\\');
				else if (randomAsteroidChar == 3)
					tmpStr.push_back('-');
				else if (randomAsteroidChar == 4)
					tmpStr.push_back('_');	
				else if (randomAsteroidChar == 5)
					tmpStr.push_back('*');
				else
					tmpStr.push_back('=');	
			}
			tmpV[i].push_back(tmpStr);
		}
	}
			
	return tmpV;
}

vector<vector<wstring>> Asteroid::getGraphicLines() {
	return Asteroid::_getGraphicLines();
}

vector<vector<wstring>> Asteroid::graphicLines = 
	Asteroid::initializeVectorGraphics();	  

//Color scheme	
int Asteroid::color = WHITE_BLACK;

// References
// https://ubuntuforums.org/showthread.php?t=836043
// https://stackoverflow.com/questions/17663186/initializing-a-two-dimensional-stdvector
// https://stackoverflow.com/questions/1115854/how-to-resolve-error-bad-index-fatal-index-file-corrupt-when-using-git