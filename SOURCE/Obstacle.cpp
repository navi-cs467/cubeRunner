/********************************************************************
** Program name: Obstacle.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Source file for Obstacle class. The Obstacle class 
**				is abstract and provides non-derived class specific
**				member definitions/implementations for the game 
**				obstacles.
********************************************************************/

#include "../HEADER/Obstacle.hpp"
#include "../HEADER/World.hpp"

void Obstacle::createObstacle(World *world, 
							  vector<vector<string>> graphicLines,
							  int specificGraphic) {
	
	//Number of potential graphics for this Obstacle
	int numGraphics = graphicLines.size();
	
	//If multiple graphics are available for this Obstacle,
	//and a specific graphic is not indicated via the specificGraphic
	//parameter (i.e. specificGraphic == -1), randomly choose one of 
	//the available graphics
	if(numGraphics > 1 && specificGraphic == -1)
		gt = (rand() % numGraphics);
	else if(specificGraphic != -1) gt = specificGraphic;
	else gt = 0;
	
	//Find random starting position that does not
	//encroach on existing world obstacles or miniCubes
	set<pair<int,int>>::iterator it1, it2;
	bool enchroaches;
	do {
		posX = rand() % world->getBottomRow();
		posY = (rand() % (COLS - 10)) + 10;		//No obstacles start in first 10 columns
		for(int i = 0; i < graphicLines[gt].size(); i++) {
			for(int j = 0; j < graphicLines[gt][i].length(); j++) {
				it1 = world->getObsCoords().
					find(make_pair(i + posX, j + posY));
				it2 = world->getMiniCubes().
					find(make_pair(i + posX, j + posY));
				if(it1 != world->getObsCoords().end() || 
				   it2 != world->getMiniCubes().end()) {
					enchroaches = true;
					break;
				}
				else enchroaches = false;
			}
			if(enchroaches == true) break;
		}
		
	} while(enchroaches == true);
}