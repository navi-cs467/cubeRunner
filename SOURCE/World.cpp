/********************************************************************
** Program name: World.hpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Source file for World class. The World class is 
** 				abstract and provides non-derived class specific
**				member definitions for the dynamic screen layout 
**				during gameplay.
********************************************************************/

#include "../HEADER/World.hpp"
#include "../HEADER/Cube.hpp"

void World::updateObsCoords(Obstacle *ob) {
	
	for(int i = 0; i < ob->getGTS(); i++) {
		for(int j = 0; j < ob->getLongestGS(); j++) {
			obsCoords.insert(make_pair(ob->getPosX() + i,
									   ob->getPosY() + j));
			if(j < ob->getGraphicLines()[ob->getGT()][i].length() &&
			   (ob->getGraphicLines()[ob->getGT()][i][j]) != ' ')
					ob->getNonWSObsCoords().insert(make_pair(ob->getPosX() + i,
									   ob->getPosY() + j));
		}
	}
}

void World::initMiniCubes(int numMiniCubes, 
						  Direction offScreenDirection) {
	//Find random starting position that does not
	//encroach on existing world obstacles
	set<pair<int,int>>::iterator it;
	bool enchroaches;
	int posX, posY;
	
	for(int i = 0; i < numMiniCubes; i++) {
		do {
			posX = rand() % bottomRow;
			
			if(offScreenDirection == none)
				posY = (rand() % (COLS - 10)) + 10;		//No miniCubes start in first 10 columns
			else
				posY = rand() % COLS;	
			
			if(offScreenDirection != none) {
				if(offScreenDirection == right)
					posY = posY + COLS;
				else if(offScreenDirection == left)
					posY = -posY;
				else if(offScreenDirection == up)
					posX = -posX;
				else
					posX = posX + LINES;
			}
			it = obsCoords.find(make_pair(posX, posY));
			if(it != obsCoords.end())
				enchroaches = true;
			else enchroaches = false;
		} while(enchroaches == true);
		miniCubes.insert(make_pair(posX, posY));
	}
}

void World::moveObs() {
	for(list<Obstacle*>::iterator it = obstacles.begin();
		it != obstacles.end(); it++) 
			if(!(*it)->getIsStationary())
				(*it)->move(this);
}

void World::resetPlayer(Cube *cube) {
	//Remove Obstacles in first 10 columns so player can be
	//reset in "safety zone"
	for(list<Obstacle*>::iterator it = obstacles.begin();
		it != obstacles.end(); it++) {
		if((*it)->getPosY() <= RESET_WIDTH) {
			//Remove coords from obsCoords and nonWSObsCoords
			for(int i = 0; i < (*it)->getGTS(); i++)
				for(int j = 0; j < (*it)->getLongestGS(); j++) {
					if(getObsCoords().find
							(make_pair((*it)->getPosX() + i,
										   (*it)->getPosY() + j)) !=
											getObsCoords().end())
						getObsCoords().erase
							(getObsCoords().
								find(make_pair((*it)->getPosX() + i,
											   (*it)->getPosY() + j)));
					if((*it)->getNonWSObsCoords().find
							(make_pair((*it)->getPosX() + i,
										   (*it)->getPosY() + j)) !=
											(*it)->getNonWSObsCoords().end())
						(*it)->getNonWSObsCoords().erase
							((*it)->getNonWSObsCoords().
								find(make_pair((*it)->getPosX() + i,
											   (*it)->getPosY() + j)));
				}
			//Free memory
			delete *it;

			//Remove pointer
			obstacles.erase(it--);
		}
	}
	cube->cubeReset(this);
}

// References
// http://www.cplusplus.com/reference/string/string/length/