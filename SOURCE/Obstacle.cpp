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
							  vector<vector<wstring>> graphicLines,
							  Direction offScreenDirection,
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
		//if(typeid(*world) != Space)
		//Can't have partial graphic beneath "ground" for Water or Land
		if(world->getBottomRow() - posX < graphicLines[gt].size())
			posX = world->getBottomRow() - (graphicLines[gt].size() - 1);
		
		if(offScreenDirection == none)
			posY = (rand() % (COLS - 10)) + 10;		//No obstacles start in first 10 columns
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

void Obstacle::setGT(int newGraphic) {
	//Update graphic type
	gt = newGraphic;
	
	//Update graphic type size and length of longest graphic string
	longestGS = 0;
	if(typeid(*this) == typeid(Seaweed)) {
		gts = Seaweed::getGraphicLines()[gt].size();
		for(int i = 0; i < gts; i++)
			if(Seaweed::getGraphicLines()[gt][i].size() > longestGS)
				longestGS = Seaweed::getGraphicLines()[gt][i].size();
	}
	else if(typeid(*this) == typeid(Coral)) {
		gts = Coral::getGraphicLines()[gt].size();
		for(int i = 0; i < gts; i++)
			if(Coral::getGraphicLines()[gt][i].size() > longestGS)
				longestGS = Coral::getGraphicLines()[gt][i].size();
	}
	else if(typeid(*this) == typeid(Shark)) {
		gts = Shark::getGraphicLines()[gt].size();
		for(int i = 0; i < gts; i++)
			if(Shark::getGraphicLines()[gt][i].size() > longestGS)
				longestGS = Shark::getGraphicLines()[gt][i].size();
	}
	else if(typeid(*this) == typeid(Octopus)) {
		gts = Octopus::getGraphicLines()[gt].size();
		for(int i = 0; i < gts; i++)
			if(Octopus::getGraphicLines()[gt][i].size() > longestGS)
				longestGS = Octopus::getGraphicLines()[gt][i].size();
	}
}

void Obstacle::move(World*) {
	
	Direction randDir = rand() % NUM_DIRECTIONS;
	bool moveCleared;
	set<pair<int,int>>::iterator itObs, itCubes;
	for(int i = 0; i < NUM_DIRECTIONS && !moveCleared; i++) {
		moveCleared = true;
		if(randDir == left) {
			for(int j = 0; j < gts; j++) {
				itObs = world->getObsCoords().
					find(make_pair(posX + j, posY - 1));
				if(itObs != world->getObsCoords.end()) {
					randDir++;
					moveCleared = false;
					continue;
				}
			}
			if(moveCleared) {
				for(int j = 0; j < gts; j++) {
					itCubes = world->getMiniCubes().
						find(make_pair(posX + j, posY - 1));
					if(itCubes != world->miniCubes.end()) {
						miniCubes.erase(itCubes);
					}
				}
			}
		}
		if(randDir == left_down) {
			for(int j = 1; j <= gts; j++) {
				itObs = world->getObsCoords().
					find(make_pair(posX + j, posY - 1));
				if(itObs != world->getObsCoords.end()) {
					randDir++;
					moveCleared = false;
					continue;
				}
			}
			for(int j = 0; j < longestGS - 1; j++) {
				itObs = world->getObsCoords().
					find(make_pair(posX + gts, posY + j));
				if(itObs != world->getObsCoords.end()) {
					randDir++;
					moveCleared = false;
					continue;
				}
			}
			if(moveCleared) {
				for(int j = 1; j <= gts; j++) {
					itCubes = world->getMiniCubes().
						find(make_pair(posX + j, posY - 1));
					if(itCubes != world->miniCubes.end()) {
						miniCubes.erase(itCubes);
					}
				}
				for(int j = 0; j < longestGS - 1; j++) {
					itCubes = world->getMiniCubes().
						find(make_pair(posX + gts, posY + j));
					if(itCubes != world->miniCubes.end()) {
						miniCubes.erase(itCubes);
					}
				}
			}
		}
		if(randDir == down) {
			for(int j = 0; j < longestGS; j++) {
				itObs = world->getObsCoords().
					find(make_pair(posX + gts, posY + j));
				if(itObs != world->getObsCoords.end()) {
					randDir++;
					moveCleared = false;
					continue;
				}
			}
			if(moveCleared) {
				for(int j = 0; j < longestGS; j++) {
					itCubes = world->getMiniCubes().
						find(make_pair(posX + gts, posY + j)) != 
					if(itCubes != world->miniCubes.end()) {
						miniCubes.erase(itCubes);
					}
				}
			}
		}
		if(randDir == right_down) {
			for(int j = 1; j <= gts; j++) {
				itObs = world->getObsCoords().
					find(make_pair(posX + j, posY + longestGS));
				if(itObs != world->getObsCoords.end()) {
					randDir++;
					moveCleared = false;
					continue;
				}
			}
			for(int j = 1; j <= longestGS; j++) {
				itObs = world->getObsCoords().
					find(make_pair(posX + gts, posY + j));
				if(itObs != world->getObsCoords.end()) {
					randDir++;
					moveCleared = false;
					continue;
				}
			}
			if(moveCleared) {
				for(int j = 1; j <= gts; j++) {
					itCubes = world->getMiniCubes().
						find(make_pair(posX + j, posY + longestGS)) != 
					if(itCubes != world->miniCubes.end()) {
						miniCubes.erase(itCubes);
					}
				}
				for(int j = 1; j <= longestGS; j++) {
					itCubes = world->getMiniCubes().
						find(make_pair(posX + gts, posY + j)) != 
					if(itCubes != world->miniCubes.end()) {
						miniCubes.erase(itCubes);
					}
				}
			}
		}
		if(randDir == right) {
			for(int j = 0; j < gts; j++) {
				itObs = world->getObsCoords().
					find(make_pair(posX + j, posY + longestGS));
				if(itObs != world->getObsCoords.end()) {
					randDir++;
					moveCleared = false;
					continue;
				}
			}
			if(moveCleared) {
				for(int j = 0; j < gts; j++) {
					itCubes = world->getMiniCubes().
						find(make_pair(posX + j, posY + longestGS)) != 
					if(itCubes != world->miniCubes.end()) {
						miniCubes.erase(itCubes);
					}
				}
			}
		}
		if(randDir == right_up) {
			for(int j = -1; j < gts - 1; j++) {
				itObs = world->getObsCoords().
					find(make_pair(posX + j, posY + longestGS));
				if(itObs != world->getObsCoords.end()) {
					randDir++;
					moveCleared = false;
					continue;
				}
			}
			for(int j = 1; j < longestGS; j++) {
				itObs = world->getObsCoords().
					find(make_pair(posX - 1, posY + j));
				if(itObs != world->getObsCoords.end()) {
					randDir++;
					moveCleared = false;
					continue;
				}
			}
			if(moveCleared) {
				for(int j = -1; j < gts - 1; j++) {
					itCubes = world->getMiniCubes().
						find(make_pair(posX + j, posY + longestGS)) != 
					if(itCubes != world->miniCubes.end()) {
						miniCubes.erase(itCubes);
					}
				}
				for(int j = 1; j < longestGS; j++) {
					itCubes = world->getMiniCubes().
						find(make_pair(posX - 1, posY + j)) != 
					if(itCubes != world->miniCubes.end()) {
						miniCubes.erase(itCubes);
					}
				}
			}
		}
		if(randDir == up) {
			for(int j = 0; j < longestGS; j++) {
				itObs = world->getObsCoords().
					find(make_pair(posX - 1, posY + j));
				if(itObs != world->getObsCoords.end()) {
					randDir++;
					moveCleared = false;
					continue;
				}
			}
			if(moveCleared) {
				for(int j = 0; j < longestGS; j++) {
					itCubes = world->getMiniCubes().
						find(make_pair(posX - 1, posY + j)) != 
					if(itCubes != world->miniCubes.end()) {
						miniCubes.erase(itCubes);
					}
				}
			}
		}
		if(randDir == left_up) {
			for(int j = -1; j < gts - 1; j++) {
				itObs = world->getObsCoords().
					find(make_pair(posX + j, posY - 1));
				if(itObs != world->getObsCoords.end()) {
					randDir++;
					moveCleared = false;
					continue;
				}
			}
			for(int j = 0; j < longestGS - 1; j++) {
				itObs = world->getObsCoords().
					find(make_pair(posX - 1, posY + j));
				if(itObs != world->getObsCoords.end()) {
					randDir++;
					moveCleared = false;
					continue;
				}
			}
			if(moveCleared) {
				for(int j = -1; j < gts - 1; j++) {
					itCubes = world->getMiniCubes().
						find(make_pair(posX + j, posY - 1)) != 
					if(itCubes != world->miniCubes.end()) {
						miniCubes.erase(itCubes);
					}
				}
				for(int j = 0; j < longestGS - 1; j++) {
					itCubes = world->getMiniCubes().
						find(make_pair(posX - 1, posY + j)) != 
					if(itCubes != world->miniCubes.end()) {
						miniCubes.erase(itCubes);
					}
				}
			}
		}
	}
}
	