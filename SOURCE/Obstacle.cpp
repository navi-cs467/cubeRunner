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
#include "../HEADER/Water.hpp"
#include "../HEADER/Land.hpp"

Obstacle::Obstacle(int type, int posX, int posY, int gt, int gts, 
				   int colorOrColorSeed, int hits, int gm) :
			posX(posX), posY(posY), gt(gt), gts(gts) {
	longestGS = 0;
	for(int i = 0; i < gts; i++) {
		if(type == 1) {
			if(Seaweed::_getGraphicLines()[gt][i].size() > longestGS)
				longestGS = Seaweed::_getGraphicLines()[gt][i].size();
		}
		else if(type == 2) {
			if(Coral::_getGraphicLines()[gt][i].size() > longestGS)
				longestGS = Coral::_getGraphicLines()[gt][i].size();
		}
		else if(type == 3) {
			if(Shark::_getGraphicLines()[gt][i].size() > longestGS)
				longestGS = Shark::_getGraphicLines()[gt][i].size();
			//Initialize maxHits based on gameMode
			if(gm == EASY) maxHits = 3;
			else if(gm == NORMAL) maxHits = 4;
			else maxHits = 5;
			
			//Initialize hits ands mvsSinceLastHit
			this->hits = hits;
		}
		else if(type == 4) {
			if(Octopus::_getGraphicLines()[gt][i].size() > longestGS)
				longestGS = Octopus::_getGraphicLines()[gt][i].size();
			//Initialize maxHits based on gameMode
			if(gm == EASY) maxHits = 2;
			else if(gm == NORMAL) maxHits = 3;
			else maxHits = 4;
			
			//Initialize hits and mvsSinceLastHit
			this->hits = hits;
		}
		else if(type == 5) {
			if(Tree::_getGraphicLines()[gt][i].size() > longestGS)
				longestGS = Tree::_getGraphicLines()[gt][i].size();
		}
	}
}

void Obstacle::createObstacle(World *world, 
							  Direction offScreenDirection,
							  int specificGraphic) {
	
	//Number of potential graphics for this Obstacle
	int numGraphics = getGraphicLines().size();
	
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
		if(world->getBottomRow() - posX < getGraphicLines()[gt].size())
			posX = world->getBottomRow() - (getGraphicLines()[gt].size() - 1);
		
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
		
		for(int i = 0; i < getGraphicLines()[gt].size(); i++) {
			for(int j = 0; j < getGraphicLines()[gt][i].length(); j++) {
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
	
	//Update graphic type size
	gts = getGraphicLines()[gt].size();
	
	//Update length of longest graphic string
	longestGS = 0;
	for(int i = 0; i < gts; i++)
		if(getGraphicLines()[gt][i].size() > longestGS)
			longestGS = getGraphicLines()[gt][i].size();
	
	/* //Update (invert) Obstacle's holes
	set<pair<int, int>> tmpHoles;
	//if(lastMV != up || lastMV != down) {
	if(1) {
		for(int i = 0; i < gts; i++) {
			for(int j = 0; j < this->getGraphicLines()[gt][i].length(); j++) {
				if(holes.find(make_pair(posX + i, posY + j)) != holes.end()) {
					tmpHoles.insert
						(make_pair(posX + i, 
							posY + this->getGraphicLines()[gt][i].length() - j - 1));
				}
			}
		}
		swap(holes, tmpHoles);
	}
	else {
		for(int i = 0; i < gts; i++) {
			for(int j = 0; j < this->getGraphicLines()[gt][i].length(); j++) {
				if(holes.find(make_pair(posX + i, posY + j)) != holes.end()) {
					tmpHoles.insert
						(make_pair(posX + this->getGraphicLines()[gt][i].length() - i - 1, 
							posY + j));
				}
			}
		}
		swap(holes, tmpHoles);
	} */
}

void Obstacle::move(World* world) {
	
	//Do not move the Obstacle if it is offscreen left or right
	//for Water and Land Worlds
	if(((typeid(*world) == typeid(Water)) ||
		typeid(*world) == typeid(Land)) &&
		   (posY > COLS || posY < 0 - longestGS)) return;
	
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
	bool moveCleared = true;
	set<pair<int,int>>::iterator itObs, itCubes;
	set<pair<int,int>> newNonWSObsCoords, newHoles;
	for(int i = 0; i < NUM_DIRECTIONS; i++, moveCleared = true) {
		if(mvDir == left) { 
			for(int j = 0; j < gts; j++) {
				itObs = world->getObsCoords().
					find(make_pair(posX + j, posY - 1));
				if(itObs != world->getObsCoords().end()) {
					//++mvDir;
					mvDir = left_down;
					if(mvDir == none) mvDir = left;
					moveCleared = false;
					break;
				}
			}
			if(moveCleared) {
				//Erase affected obsCoords
				for(int j = 0; j < gts; j++) {
					itObs = world->getObsCoords().
						find(make_pair(posX + j, posY + longestGS - 1));
					if(itObs != world->getObsCoords().end())
						world->getObsCoords().erase(itObs);
				}
				//Update Obstacle position and graphic
				posY--;
				setGT(1);
				//Insert new obsCoords
				for(int j = 0; j < gts; j++) { 
					world->getObsCoords().
						insert(make_pair(posX + j, posY));
				}
				//Update Obstacle's nonWSObsCoords	
				for(int i = 0; i < gts; i++) {
					for(int j = 0; j < this->getGraphicLines()[gt][i].length(); j++) {
						if(this->getGraphicLines()[gt][i][j] != ' ')
								newNonWSObsCoords.insert(make_pair(posX + i, posY + j));
					}
				}
				swap(nonWSObsCoords, newNonWSObsCoords);
				/* //Update Obstacle's holes
				for(set<pair<int, int>>::iterator holesIt = holes.begin(); 
					holesIt != holes.end(); holesIt++) {
						newHoles.insert
							(make_pair(holesIt->first, holesIt->second - 1));
				} */
				swap(holes, newHoles);
				//Update mvCounter				
				if(lastMV == left) mvCounter--;
				else {
					lastMV = left;
					mvCounter = rand() % (MAX_MOVE_COUNTER -
							  MIN_MOVE_COUNTER + 1) +
							  MIN_MOVE_COUNTER;
				}
				//"Consume" miniCubes now in obsCoords
				for(int j = 0; j < gts; j++) {
					itCubes = world->getMiniCubes().
						find(make_pair(posX + j, posY));
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
			if(posX + gts == world->getBottomRow() + 1) {
				mvDir = down;
				continue;
			}
			for(int j = 1; j <= gts; j++) {
				itObs = world->getObsCoords().
					find(make_pair(posX + j, posY - 1));
				if(itObs != world->getObsCoords().end()) {
					//++mvDir;
					mvDir = down;
					if(mvDir == none) mvDir = left;
					moveCleared = false;
					break;
				}
			}
			for(int j = 0; j < longestGS - 1; j++) {
				itObs = world->getObsCoords().
					find(make_pair(posX + gts, posY + j));
				if(itObs != world->getObsCoords().end()) {
					//++mvDir;
					mvDir = down;
					if(mvDir == none) mvDir = left;
					moveCleared = false;
					break;
				}
			}
			if(moveCleared) {
				//Erase affected obsCoords
				for(int j = 0; j < gts; j++) {
					itObs = world->getObsCoords().
						find(make_pair(posX + j, posY + longestGS - 1));
					if(itObs != world->getObsCoords().end())
						world->getObsCoords().erase(itObs);
				}
				for(int j = 0; j < longestGS - 1; j++) {
					itObs = world->getObsCoords().
						find(make_pair(posX, posY + j));
					if(itObs != world->getObsCoords().end())
						world->getObsCoords().erase(itObs);
				}
				//Update Obstacle's position and graphic
				posX++; posY--;
				setGT(1);
				for(int j = 0; j < gts; j++) {
					world->getObsCoords().
						insert(make_pair(posX + j, posY));
					
				}
				for(int j = 0; j < longestGS; j++) {
					world->getObsCoords().
						insert(make_pair(posX + gts - 1, posY + j));
				}
				//Update Obstacle's nonWSObsCoords	
				for(int i = 0; i < gts; i++) {
					for(int j = 0; j < this->getGraphicLines()[gt][i].length(); j++) {
						if(this->getGraphicLines()[gt][i][j] != ' ')
								newNonWSObsCoords.insert(make_pair(posX + i, posY + j));
					}
				}
				swap(nonWSObsCoords, newNonWSObsCoords);
				/* //Update Obstacle's holes
				for(set<pair<int, int>>::iterator holesIt = holes.begin(); 
					holesIt != holes.end(); holesIt++) {
						newHoles.insert
							(make_pair(holesIt->first + 1, holesIt->second - 1));
				}
				swap(holes, newHoles); */
				//Update move counter and/or direction
				if(lastMV == left_down) mvCounter--;
				else {
					lastMV = left_down;
					mvCounter = rand() % (MAX_MOVE_COUNTER -
							  MIN_MOVE_COUNTER + 1) +
							  MIN_MOVE_COUNTER;
				}
				//Consume miniCubes in Obstacles path
				for(int j = 0; j < gts; j++) {
					itCubes = world->getMiniCubes().
						find(make_pair(posX + j, posY - 1));
					if(itCubes != world->getMiniCubes().end()) {
						world->getMiniCubes().erase(itCubes);
					}
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
		else if(mvDir == down) {
			//if(typeof(world*) != typeof(Space) &&
			//	 posX + gts == world->getBottomRow() + 1) continue;
			if(posX + gts == world->getBottomRow() + 1) {
				mvDir = right_down;
				continue;
			}
			for(int j = 0; j < longestGS; j++) {
				itObs = world->getObsCoords().
					find(make_pair(posX + gts, posY + j));
				if(itObs != world->getObsCoords().end()) {
					//++mvDir;
					mvDir = right_down;
					if(mvDir == none) mvDir = left;
					moveCleared = false;
					break;
				}
			}
			if(moveCleared) {
				//Erase affected obsCoords
				for(int j = 0; j < longestGS; j++) {
					itObs = world->getObsCoords().
						find(make_pair(posX, posY + j));
					if(itObs != world->getObsCoords().end())
						world->getObsCoords().erase(itObs);
				}
				posX++;
				for(int j = 0; j < longestGS; j++) {
					world->getObsCoords().
						insert(make_pair(posX + gts - 1, posY + j));
				}
				//Update Obstacle's nonWSObsCoords	
				for(int i = 0; i < gts; i++) {
					for(int j = 0; j < this->getGraphicLines()[gt][i].length(); j++) {
						if(this->getGraphicLines()[gt][i][j] != ' ')
								newNonWSObsCoords.insert(make_pair(posX + i, posY + j));
					}
				}
				swap(nonWSObsCoords, newNonWSObsCoords);
				/* //Update Obstacle's holes
				for(set<pair<int, int>>::iterator holesIt = holes.begin(); 
					holesIt != holes.end(); holesIt++) {
						newHoles.insert
							(make_pair(holesIt->first + 1, holesIt->second));
				}
				swap(holes, newHoles); */
				//Update move counter and/or direction
				if(lastMV == down) mvCounter--;
				else {
					lastMV = down;
					mvCounter = rand() % (MAX_MOVE_COUNTER -
							  MIN_MOVE_COUNTER + 1) +
							  MIN_MOVE_COUNTER;
				}
				//Consume miniCubes in Obstacles path
				for(int j = 0; j < longestGS; j++) {
					itCubes = world->getMiniCubes().
						find(make_pair(posX + gts - 1, posY + j)); 
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
			if(posX + gts == world->getBottomRow() + 1) {
				mvDir = right;
				continue;
			}
			for(int j = 1; j <= gts; j++) {
				itObs = world->getObsCoords().
					find(make_pair(posX + j, posY + longestGS));
				if(itObs != world->getObsCoords().end()) {
					//++mvDir;
					mvDir = right;
					if(mvDir == none) mvDir = left;
					moveCleared = false;
					break;
				}
			}
			for(int j = 1; j <= longestGS; j++) {
				itObs = world->getObsCoords().
					find(make_pair(posX + gts, posY + j));
				if(itObs != world->getObsCoords().end()) {
					//++mvDir;
					mvDir = right;
					if(mvDir == none) mvDir = left;
					moveCleared = false;
					continue;
				}
			}
			if(moveCleared) {
				//Erase affected obsCoords
				for(int j = 0; j < gts; j++) {
					itObs = world->getObsCoords().
						find(make_pair(posX + j, posY));
					if(itObs != world->getObsCoords().end())
						world->getObsCoords().erase(itObs);
				}
				for(int j = 1; j < longestGS; j++) {
					itObs = world->getObsCoords().
						find(make_pair(posX, posY + j));
					if(itObs != world->getObsCoords().end())
						world->getObsCoords().erase(itObs);
				}
				//Update Obstacle's position and graphic
				posX++; posY++;
				setGT(0);
				for(int j = 0; j < gts; j++) {
					world->getObsCoords().
						insert(make_pair(posX + j, posY + longestGS - 1));
				}
				for(int j = 0; j < longestGS; j++) {
					world->getObsCoords().
						insert(make_pair(posX + gts - 1, posY + j));
				}
				//Update Obstacle's nonWSObsCoords	
				for(int i = 0; i < gts; i++) {
					for(int j = 0; j < this->getGraphicLines()[gt][i].length(); j++) {
						if(this->getGraphicLines()[gt][i][j] != ' ')
								newNonWSObsCoords.insert(make_pair(posX + i, posY + j));
					}
				}
				swap(nonWSObsCoords, newNonWSObsCoords);
				/* //Update Obstacle's holes
				for(set<pair<int, int>>::iterator holesIt = holes.begin(); 
					holesIt != holes.end(); holesIt++) {
						newHoles.insert
							(make_pair(holesIt->first + 1, holesIt->second + 1));
				}
				swap(holes, newHoles); */
				//Update move counter and/or direction
				if(lastMV == right_down) mvCounter--;
				else {
					lastMV = right_down;
					mvCounter = rand() % (MAX_MOVE_COUNTER -
							  MIN_MOVE_COUNTER + 1) +
							  MIN_MOVE_COUNTER;
				}
				//Consume miniCubes in Obstacles path
				for(int j = 0; j < gts; j++) {
					itCubes = world->getMiniCubes().
						find(make_pair(posX + j, posY + longestGS)); 
					if(itCubes != world->getMiniCubes().end()) {
						world->getMiniCubes().erase(itCubes);
					}
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
		else if(mvDir == right) {
			for(int j = 0; j < gts; j++) {
				itObs = world->getObsCoords().
					find(make_pair(posX + j, posY + longestGS));
				if(itObs != world->getObsCoords().end()) {
					//++mvDir;
					mvDir = right_up;
					if(mvDir == none) mvDir = left;
					moveCleared = false;
					break;
				}
			}
			if(moveCleared) {
				//Erase affected obsCoords
				for(int j = 0; j < gts; j++) {
					itObs = world->getObsCoords().
						find(make_pair(posX + j, posY));
					if(itObs != world->getObsCoords().end())
						world->getObsCoords().erase(itObs);
				}
				//Update position and graphic
				posY++;
				setGT(0);
				for(int j = 0; j < gts; j++) {
					world->getObsCoords().
						insert(make_pair(posX + j, posY + longestGS - 1));
				}
				//Update Obstacle's nonWSObsCoords	
				for(int i = 0; i < gts; i++) {
					for(int j = 0; j < this->getGraphicLines()[gt][i].length(); j++) {
						if(this->getGraphicLines()[gt][i][j] != ' ')
								newNonWSObsCoords.insert(make_pair(posX + i, posY + j));
					}
				}
				swap(nonWSObsCoords, newNonWSObsCoords);
				/* //Update Obstacle's holes
				for(set<pair<int, int>>::iterator holesIt = holes.begin(); 
					holesIt != holes.end(); holesIt++) {
						newHoles.insert
							(make_pair(holesIt->first, holesIt->second + 1));
				}
				swap(holes, newHoles); */
				//Update move counter and/or direction
				if(lastMV == right) mvCounter--;
				else {
					lastMV = right;
					mvCounter = rand() % (MAX_MOVE_COUNTER -
							  MIN_MOVE_COUNTER + 1) +
							  MIN_MOVE_COUNTER;
				}
				//Consume miniCubes in Obstacles path
				for(int j = 0; j < gts; j++) {
					itCubes = world->getMiniCubes().
						find(make_pair(posX + j, posY + longestGS - 1)); 
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
					//++mvDir;
					mvDir = up;
					if(mvDir == none) mvDir = left;
					moveCleared = false;
					break;
				}
			}
			for(int j = 1; j < longestGS; j++) {
				itObs = world->getObsCoords().
					find(make_pair(posX - 1, posY + j));
				if(itObs != world->getObsCoords().end()) {
					//++mvDir;
					mvDir = up;
					if(mvDir == none) mvDir = left;
					moveCleared = false;
					break;
				}
			}
			if(moveCleared) {
				//Erase affected obsCoords
				for(int j = 0; j < gts; j++) {
					itObs = world->getObsCoords().
						find(make_pair(posX + j, posY));
					if(itObs != world->getObsCoords().end())
						world->getObsCoords().erase(itObs);
				}
				for(int j = 1; j < longestGS; j++) {
					itObs = world->getObsCoords().
						find(make_pair(posX + gts - 1, posY + j));
					if(itObs != world->getObsCoords().end())
						world->getObsCoords().erase(itObs);
				}
				//Update Obstacle's position and graphic
				posX--; posY++;
				setGT(0);
				for(int j = 0; j < gts; j++) { 
					world->getObsCoords().
						insert(make_pair(posX + j, posY + longestGS - 1));
				}
				for(int j = 0; j < longestGS; j++) {
					world->getObsCoords().
						insert(make_pair(posX, posY + j));
				}
				//Update Obstacle's nonWSObsCoords	
				for(int i = 0; i < gts; i++) {
					for(int j = 0; j < this->getGraphicLines()[gt][i].length(); j++) {
						if(this->getGraphicLines()[gt][i][j] != ' ')
								newNonWSObsCoords.insert(make_pair(posX + i, posY + j));
					}
				}
				swap(nonWSObsCoords, newNonWSObsCoords);
				/* //Update Obstacle's holes
				for(set<pair<int, int>>::iterator holesIt = holes.begin(); 
					holesIt != holes.end(); holesIt++) {
						newHoles.insert
							(make_pair(holesIt->first - 1, holesIt->second + 1));
				}
				swap(holes, newHoles); */
				//Update move counter and/or direction
				if(lastMV == right_up) mvCounter--;
				else {
					lastMV = right_up;
					mvCounter = rand() % (MAX_MOVE_COUNTER -
							  MIN_MOVE_COUNTER + 1) +
							  MIN_MOVE_COUNTER;
				}
				//Consume miniCubes in Obstacles path
				for(int j = -1; j < gts - 1; j++) {
					itCubes = world->getMiniCubes().
						find(make_pair(posX + j, posY + longestGS - 1)); 
					if(itCubes != world->getMiniCubes().end()) {
						world->getMiniCubes().erase(itCubes);
					}
				}
				for(int j = 1; j < longestGS; j++) {
					itCubes = world->getMiniCubes().
						find(make_pair(posX, posY + j));
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
					//++mvDir;
					mvDir = left_up;
					if(mvDir == none) mvDir = left;
					moveCleared = false;
					break;
				}
			}
			if(moveCleared) {
				//Erase affected obsCoords
				for(int j = 0; j < longestGS; j++) {
					itObs = world->getObsCoords().
						find(make_pair(posX + gts - 1, posY + j));
					if(itObs != world->getObsCoords().end())
						world->getObsCoords().erase(itObs);
				}
				posX--;
				for(int j = 0; j < longestGS; j++) {
					world->getObsCoords().
						insert(make_pair(posX, posY + j));
				}
				//Update Obstacle's nonWSObsCoords	
				for(int i = 0; i < gts; i++) {
					for(int j = 0; j < this->getGraphicLines()[gt][i].length(); j++) {
						if(this->getGraphicLines()[gt][i][j] != ' ')
								newNonWSObsCoords.insert(make_pair(posX + i, posY + j));
					}
				}
				swap(nonWSObsCoords, newNonWSObsCoords);
				/* //Update Obstacle's holes
				for(set<pair<int, int>>::iterator holesIt = holes.begin(); 
					holesIt != holes.end(); holesIt++) {
						newHoles.insert
							(make_pair(holesIt->first - 1, holesIt->second));
				}
				swap(holes, newHoles); */
				//Update move counter and/or direction
				if(lastMV == up) mvCounter--;
				else {
					lastMV = up;
					mvCounter = rand() % (MAX_MOVE_COUNTER -
							  MIN_MOVE_COUNTER + 1) +
							  MIN_MOVE_COUNTER;
				}
				//Consume miniCubes in Obstacles path
				for(int j = 0; j < longestGS; j++) {
					itCubes = world->getMiniCubes().
						find(make_pair(posX, posY + j));
					if(itCubes != world->getMiniCubes().end()) {
						world->getMiniCubes().erase(itCubes);
					}
				}
				break;
			}
		}
		else {	//left_up
			for(int j = -1; j < gts - 1; j++) {
				itObs = world->getObsCoords().
					find(make_pair(posX + j, posY - 1));
				if(itObs != world->getObsCoords().end()) {
					//++mvDir;
					mvDir = left;
					if(mvDir == none) mvDir = left;
					moveCleared = false;
					break;
				}
			}
			for(int j = 0; j < longestGS - 1; j++) {
				itObs = world->getObsCoords().
					find(make_pair(posX - 1, posY + j));
				if(itObs != world->getObsCoords().end()) {
					//++mvDir;
					mvDir = left;
					if(mvDir == none) mvDir = left;
					moveCleared = false;
					break;
				}
			}
			if(moveCleared) {
				//Erase affected obsCoords
				for(int j = 0; j < gts; j++) {
					itObs = world->getObsCoords().
						find(make_pair(posX + j, posY + longestGS - 1));
					if(itObs != world->getObsCoords().end())
						world->getObsCoords().erase(itObs);
				}
				for(int j = 0; j < longestGS - 1; j++) {
					itObs = world->getObsCoords().
						find(make_pair(posX + gts - 1, posY + j));
					if(itObs != world->getObsCoords().end())
						world->getObsCoords().erase(itObs);
				}
				//Update Obstacle's position and graphic
				posX--; posY--;
				setGT(1);
				for(int j = 0; j < gts; j++) { 
					world->getObsCoords().
						insert(make_pair(posX + j, posY));
				}
				for(int j = 0; j < longestGS; j++) {
					world->getObsCoords().
						insert(make_pair(posX, posY + j));
				}
				//Update Obstacle's nonWSObsCoords	
				for(int i = 0; i < gts; i++) {
					for(int j = 0; j < this->getGraphicLines()[gt][i].length(); j++) {
						if(this->getGraphicLines()[gt][i][j] != ' ')
								newNonWSObsCoords.insert(make_pair(posX + i, posY + j));
					}
				}
				swap(nonWSObsCoords, newNonWSObsCoords);
				/* //Update Obstacle's holes
				for(set<pair<int, int>>::iterator holesIt = holes.begin(); 
					holesIt != holes.end(); holesIt++) {
						newHoles.insert
							(make_pair(holesIt->first - 1, holesIt->second - 1));
				}
				swap(holes, newHoles); */
				//Update move counter and/or direction
				if(lastMV == left_up) mvCounter--;
				else {
					lastMV = left_up;
					mvCounter = rand() % (MAX_MOVE_COUNTER -
							  MIN_MOVE_COUNTER + 1) +
							  MIN_MOVE_COUNTER;
				}
				//Consume miniCubes in Obstacles path
				for(int j = 0; j < gts; j++) {
					itCubes = world->getMiniCubes().
						find(make_pair(posX + j, posY));
					if(itCubes != world->getMiniCubes().end()) {
						world->getMiniCubes().erase(itCubes);
					}
				}
				for(int j = 0; j < longestGS; j++) {
					itCubes = world->getMiniCubes().
						find(make_pair(posX, posY + j));
					if(itCubes != world->getMiniCubes().end()) {
						world->getMiniCubes().erase(itCubes);
					}
				}
				break;
			}
		}
	}
	if(moveCleared && hits > 0) {
		mvsSinceLastHit++;
		if(world->getGameMode() == EASY && mvsSinceLastHit == 60)
			hits--;
		else if(world->getGameMode() == NORMAL && mvsSinceLastHit == 40)
			hits--;
		else if(world->getGameMode() == HARD && mvsSinceLastHit == 20)
			hits--;
	}
}
	