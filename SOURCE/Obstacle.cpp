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

void Obstacle::move(World* world) {
	
	Direction mvDir; //int testMvDir;
	//Randomly assign a new "seed" direction for the
	//following for loop if the last move direction has 
	//been "exhausted" per the move counter.
	if(mvCounter == 0) {
		mvDir = static_cast<Direction>(rand() % NUM_DIRECTIONS);
		//Randomly assign a new value for
		//the move counter between MAX_MOVE_COUNTER
		//and MIN_MOVE_COUNTER
		mvCounter = rand() % (MAX_MOVE_COUNTER -
							  MIN_MOVE_COUNTER + 1) +
							  MIN_MOVE_COUNTER;
	}
	else mvDir = lastMV;
	
	//This loop starts with mvDir, then cycles through
	//all possible move directions to find one that will
	//not cause an overlap with an existing object. If
	//no direction is available (i.e. the object is 
	//surrounded by other objects on all sides and in all
	//directions) the object will not move. If the object
	//moves into a space occupied by a miniCube, the miniCube
	//is removed.
	bool moveCleared;
	set<pair<int,int>>::iterator itObs, itCubes;
	for(int i = 0; i < NUM_DIRECTIONS && !moveCleared; i++) {
		moveCleared = true;
		if(mvDir == left) { 
			for(int j = 0; j < gts; j++) {
				itObs = world->getObsCoords().
					find(make_pair(posX + j, posY - 1));
				if(itObs != world->getObsCoords().end()) {
					++mvDir;
					if(mvDir == none) mvDir = left;
					moveCleared = false;
					continue;
				}
			}
			if(moveCleared) {
				for(int j = 0; j < gts; j++) {
					itObs = world->getObsCoords().
						find(make_pair(posX + j, posY + longestGS - 1);
					world->getObsCoords().erase(itObs);
				}
				posY--;
				for(int j = 0; j < gts; j++) {
					world->getObsCoords().
						insert(make_pair(posX + j, posY));
				}
				if(lastMV == left) mvCounter--;
				else {
					lastMV = left;
					mvCounter = rand() % (MAX_MOVE_COUNTER -
							  MIN_MOVE_COUNTER + 1) +
							  MIN_MOVE_COUNTER;
				}
				for(int j = 0; j < gts; j++) {
					itCubes = world->getMiniCubes().
						find(make_pair(posX + j, posY - 1));
					if(itCubes != world->getMiniCubes().end()) {
						world->getMiniCubes().erase(itCubes);
					}
				}
				break;
			}
		}
		else if(mvDir == left_down) {
			//if(typeof(world*) != typeof(Space) &&
			//	 posX + gts == world->getBottomRow() + 1) continue;
			if(posX + gts == world->getBottomRow() + 1) continue;
			for(int j = 1; j <= gts; j++) {
				itObs = world->getObsCoords().
					find(make_pair(posX + j, posY - 1));
				if(itObs != world->getObsCoords().end()) {
					++mvDir;
					if(mvDir == none) mvDir = left;
					moveCleared = false;
					continue;
				}
			}
			for(int j = 0; j < longestGS - 1; j++) {
				itObs = world->getObsCoords().
					find(make_pair(posX + gts, posY + j));
				if(itObs != world->getObsCoords().end()) {
					++mvDir;
					if(mvDir == none) mvDir = left;
					moveCleared = false;
					continue;
				}
			}
			if(moveCleared) {
				for(int j = 0; j < gts; j++) {
					itObs = world->getObsCoords().
						find(make_pair(posX + j, posY + longestGS - 1);
					world->getObsCoords().erase(itObs);
				}
				for(int j = 0; j < longestGS; j++) {
					itObs = world->getObsCoords().
						find(make_pair(posX, posY + j);
					world->getObsCoords().erase(itObs);
				}
				posX++; posY--;
				for(int j = 0; j < gts; j++) {
					itObs = world->getObsCoords().
						insert(posX + j, posY);
				}
				for(int j = 0; j < longestGS; j++) {
					itObs = world->getObsCoords().
						insert(posX + gts - 1, posY + j);
				}
				if(lastMV == left_down) mvCounter--;
				else {
					lastMV = left_down;
					mvCounter = rand() % (MAX_MOVE_COUNTER -
							  MIN_MOVE_COUNTER + 1) +
							  MIN_MOVE_COUNTER;
				}
				for(int j = 1; j <= gts; j++) {
					itCubes = world->getMiniCubes().
						find(make_pair(posX + j, posY - 1));
					if(itCubes != world->getMiniCubes().end()) {
						world->getMiniCubes().erase(itCubes);
					}
				}
				for(int j = 0; j < longestGS - 1; j++) {
					itCubes = world->getMiniCubes().
						find(make_pair(posX + gts, posY + j));
					if(itCubes != world->getMiniCubes().end()) {
						world->getMiniCubes().erase(itCubes);
					}
				}
				break;
			}
		}
		else if(mvDir == down) {
			//if(typeof(world*) != typeof(Space) &&
			//	 posX + gts == world->getBottomRow() + 1) continue;
			if(posX + gts == world->getBottomRow() + 1) continue;
			for(int j = 0; j < longestGS; j++) {
				itObs = world->getObsCoords().
					find(make_pair(posX + gts, posY + j));
				if(itObs != world->getObsCoords().end()) {
					++mvDir;
					if(mvDir == none) mvDir = left;
					moveCleared = false;
					continue;
				}
			}
			if(moveCleared) {
				for(int j = 0; j < longestGS; j++) {
					itObs = world->getObsCoords().
						find(make_pair(posX, posY + j);
					world->getObsCoords().erase(itObs);
				}
				posX++;
				for(int j = 0; j < longestGS; j++) {
					itObs = world->getObsCoords().
						insert(posX + gts - 1, posY + j);
				}
				if(lastMV == down) mvCounter--;
				else {
					lastMV = down;
					mvCounter = rand() % (MAX_MOVE_COUNTER -
							  MIN_MOVE_COUNTER + 1) +
							  MIN_MOVE_COUNTER;
				}
				for(int j = 0; j < longestGS; j++) {
					itCubes = world->getMiniCubes().
						find(make_pair(posX + gts, posY + j)); 
					if(itCubes != world->getMiniCubes().end()) {
						world->getMiniCubes().erase(itCubes);
					}
				}
				break;
			}
		}
		else if(mvDir == right_down) {
			//if(typeof(world*) != typeof(Space) &&
			//	 posX + gts == world->getBottomRow() + 1) continue;
			if(posX + gts == world->getBottomRow() + 1) continue;
			for(int j = 1; j <= gts; j++) {
				itObs = world->getObsCoords().
					find(make_pair(posX + j, posY + longestGS));
				if(itObs != world->getObsCoords().end()) {
					++mvDir;
					if(mvDir == none) mvDir = left;
					moveCleared = false;
					continue;
				}
			}
			for(int j = 1; j <= longestGS; j++) {
				itObs = world->getObsCoords().
					find(make_pair(posX + gts, posY + j));
				if(itObs != world->getObsCoords().end()) {
					++mvDir;
					if(mvDir == none) mvDir = left;
					moveCleared = false;
					continue;
				}
			}
			if(moveCleared) {
				/* for(int j = 0; j < gts; j++) {
					itObs = world->getObsCoords().
						find(make_pair(posX + j, posY + longestGS - 1);
					world->getObsCoords().erase(itObs);
				}
				for(int j = 0; j < longestGS; j++) {
					itObs = world->getObsCoords().
						find(make_pair(posX, posY + j);
					world->getObsCoords().erase(itObs);
				}
				posX++; posY++;
				for(int j = 0; j < gts; j++) {
					itObs = world->getObsCoords().
						insert(posX + j, posY);
				}
				for(int j = 0; j < longestGS; j++) {
					itObs = world->getObsCoords().
						insert(posX + gts - 1, posY + j);
				} */
				if(lastMV == right_down) mvCounter--;
				else {
					lastMV = right_down;
					mvCounter = rand() % (MAX_MOVE_COUNTER -
							  MIN_MOVE_COUNTER + 1) +
							  MIN_MOVE_COUNTER;
				}
				for(int j = 1; j <= gts; j++) {
					itCubes = world->getMiniCubes().
						find(make_pair(posX + j, posY + longestGS)); 
					if(itCubes != world->getMiniCubes().end()) {
						world->getMiniCubes().erase(itCubes);
					}
				}
				for(int j = 1; j <= longestGS; j++) {
					itCubes = world->getMiniCubes().
						find(make_pair(posX + gts, posY + j));
					if(itCubes != world->getMiniCubes().end()) {
						world->getMiniCubes().erase(itCubes);
					}
				}
				break;
			}
		}
		else if(mvDir == right) {
			for(int j = 0; j < gts; j++) {
				itObs = world->getObsCoords().
					find(make_pair(posX + j, posY + longestGS));
				if(itObs != world->getObsCoords().end()) {
					++mvDir;
					if(mvDir == none) mvDir = left;
					moveCleared = false;
					continue;
				}
			}
			if(moveCleared) {
				posY++;
				if(lastMV == right) mvCounter--;
				else {
					lastMV = right;
					mvCounter = rand() % (MAX_MOVE_COUNTER -
							  MIN_MOVE_COUNTER + 1) +
							  MIN_MOVE_COUNTER;
				}
				for(int j = 0; j < gts; j++) {
					itCubes = world->getMiniCubes().
						find(make_pair(posX + j, posY + longestGS)); 
					if(itCubes != world->getMiniCubes().end()) {
						world->getMiniCubes().erase(itCubes);
					}
				}
				break;
			}
		}
		else if(mvDir == right_up) {
			for(int j = -1; j < gts - 1; j++) {
				itObs = world->getObsCoords().
					find(make_pair(posX + j, posY + longestGS));
				if(itObs != world->getObsCoords().end()) {
					++mvDir;
					if(mvDir == none) mvDir = left;
					moveCleared = false;
					continue;
				}
			}
			for(int j = 1; j < longestGS; j++) {
				itObs = world->getObsCoords().
					find(make_pair(posX - 1, posY + j));
				if(itObs != world->getObsCoords().end()) {
					++mvDir;
					if(mvDir == none) mvDir = left;
					moveCleared = false;
					continue;
				}
			}
			if(moveCleared) {
				posX--; posY++;
				if(lastMV == right_up) mvCounter--;
				else {
					lastMV = right_up;
					mvCounter = rand() % (MAX_MOVE_COUNTER -
							  MIN_MOVE_COUNTER + 1) +
							  MIN_MOVE_COUNTER;
				}
				for(int j = -1; j < gts - 1; j++) {
					itCubes = world->getMiniCubes().
						find(make_pair(posX + j, posY + longestGS)); 
					if(itCubes != world->getMiniCubes().end()) {
						world->getMiniCubes().erase(itCubes);
					}
				}
				for(int j = 1; j < longestGS; j++) {
					itCubes = world->getMiniCubes().
						find(make_pair(posX - 1, posY + j));
					if(itCubes != world->getMiniCubes().end()) {
						world->getMiniCubes().erase(itCubes);
					}
				}
				break;
			}
		}
		else if(mvDir == up) {
			for(int j = 0; j < longestGS; j++) {
				itObs = world->getObsCoords().
					find(make_pair(posX - 1, posY + j));
				if(itObs != world->getObsCoords().end()) {
					++mvDir;
					if(mvDir == none) mvDir = left;
					moveCleared = false;
					continue;
				}
			}
			if(moveCleared) {
				posX--;
				if(lastMV == up) mvCounter--;
				else {
					lastMV = up;
					mvCounter = rand() % (MAX_MOVE_COUNTER -
							  MIN_MOVE_COUNTER + 1) +
							  MIN_MOVE_COUNTER;
				}
				for(int j = 0; j < longestGS; j++) {
					itCubes = world->getMiniCubes().
						find(make_pair(posX - 1, posY + j));
					if(itCubes != world->getMiniCubes().end()) {
						world->getMiniCubes().erase(itCubes);
					}
				}
				break;
			}
		}
		else if(mvDir == left_up) {
			for(int j = -1; j < gts - 1; j++) {
				itObs = world->getObsCoords().
					find(make_pair(posX + j, posY - 1));
				if(itObs != world->getObsCoords().end()) {
					++mvDir;
					if(mvDir == none) mvDir = left;
					moveCleared = false;
					continue;
				}
			}
			for(int j = 0; j < longestGS - 1; j++) {
				itObs = world->getObsCoords().
					find(make_pair(posX - 1, posY + j));
				if(itObs != world->getObsCoords().end()) {
					++mvDir;
					if(mvDir == none) mvDir = left;
					moveCleared = false;
					continue;
				}
			}
			if(moveCleared) {
				posX--; posY--;
				if(lastMV == left_up) mvCounter--;
				else {
					lastMV = left_up;
					mvCounter = rand() % (MAX_MOVE_COUNTER -
							  MIN_MOVE_COUNTER + 1) +
							  MIN_MOVE_COUNTER;
				}
				for(int j = -1; j < gts - 1; j++) {
					itCubes = world->getMiniCubes().
						find(make_pair(posX + j, posY - 1));
					if(itCubes != world->getMiniCubes().end()) {
						world->getMiniCubes().erase(itCubes);
					}
				}
				for(int j = 0; j < longestGS - 1; j++) {
					itCubes = world->getMiniCubes().
						find(make_pair(posX - 1, posY + j));
					if(itCubes != world->getMiniCubes().end()) {
						world->getMiniCubes().erase(itCubes);
					}
				}
				break;
			}
		}
	}
}
	