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

void Obstacle::createObstacle(World *world, string graphicLines[],
							  int lengthGL) {
	//Find random starting position that does not
	//encroach on existing world obstacles or miniCubes
	set<pair<int,int>>::iterator it1, it2;
	bool enchroaches;
	do {
		posX = rand() % world->getBottomRow();
		posY = (rand() % (COLS - 10)) + 10;		//No obstacles start in first 10 columns
		for(int i = 0; i < lengthGL; i++) 
			for(int j = 0; j < graphicLines[i].length(); j++) {
				it1 = world->getObsCoords().
					find(make_pair(i + posX, j + posY));
				it2 = world->getMiniCubes().
					find(make_pair(i + posX, j + posY));
				if(it1 != world->getObsCoords().end() || 
				   it2 != world->getMiniCubes().end())
					enchroaches = true;
				else enchroaches = false;
			}
	} while(enchroaches == true);
}