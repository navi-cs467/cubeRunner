/********************************************************************
** Program name: Comet.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Source file for Comet class, a derived Obstacle.
********************************************************************/

#include "../HEADER/Comet.hpp"

Comet::Comet(World *world, Direction offScreen, 
			 int specificGraphic) : Obstacle() {
	
	isStationary = false;
	//curDirection = left;
	//sameDirectionMoveCount = 0;
	color = rand() % 5 + 2;
	
	//Set mvDir
	lastMV = static_cast<Direction>(rand() % NUM_DIRECTIONS);
	if(lastMV == right || lastMV == right_up || lastMV == right_down)
		gt = 1;
	else
		gt = 0;
	
	createObstacle(world, offScreen, gt);
	
	//Update size of graphic type array 
	//and length of longest graphic string
	gts = Comet::getGraphicLines()[gt].size();
	longestGS = 0;
	for(int i = 0; i < gts; i++)
		if(Comet::getGraphicLines()[gt][i].size() > longestGS)
			longestGS = Comet::getGraphicLines()[gt][i].size();
		
	//Update World's obsCoords and this Obstacle's
	//nonWSObsCoords with new graphic position
	world->updateObsCoords(this);
	
	//Initialize maxHits based on gameMode
	if(world->getGameMode() == EASY) maxHits = 1;
	else if(world->getGameMode() == NORMAL) maxHits = 2;
	else maxHits = 3;
	
	//Initialize hits and mvsSinceLastHit
	hits = 0;
	mvsSinceLastHit = 0;
}

vector<vector<wstring>> Comet::initializeVectorGraphics () {
	vector<vector<wstring>> tmpV {{L"       _ .,-\"´:.",
								   L" .--.´\"_'.:'´",
								   L"(    )´.'",
								   L" `--´-´"},
								
								  {L".:`\"-,. _",
								   L" `':.'_\"` .--. ",
								   L"      '.´(    )",
								   L"        `-`--´ "}};
								  
	return tmpV;
}

vector<vector<wstring>> Comet::getGraphicLines() {
	return Comet::_getGraphicLines();
}

vector<vector<wstring>> Comet::graphicLines = 
	Comet::initializeVectorGraphics();
	
//Comet doesn't change direction unless it hits something
void Comet::move(World* world) {
	
	Direction mvDir; //int testMvDir;

	mvDir = lastMV;
	
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
				setGT(0);
				//Insert new obsCoords
				for(int j = 0; j < gts; j++) { 
					world->getObsCoords().
						insert(make_pair(posX + j, posY));
				}
				//Update Obstacle's nonWSObsCoords	
				for(int j = 0; j < gts; j++) {
					for(int k = 0; k < this->getGraphicLines()[gt][j].length(); k++) {
						if(this->getGraphicLines()[gt][j][k] != ' ')
								newNonWSObsCoords.insert(make_pair(posX + j, posY + k));
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
				setGT(0);
				for(int j = 0; j < gts; j++) {
					world->getObsCoords().
						insert(make_pair(posX + j, posY));
					
				}
				for(int j = 0; j < longestGS; j++) {
					world->getObsCoords().
						insert(make_pair(posX + gts - 1, posY + j));
				}
				//Update Obstacle's nonWSObsCoords	
				for(int j = 0; j < gts; j++) {
					for(int k = 0; k < this->getGraphicLines()[gt][j].length(); k++) {
						if(this->getGraphicLines()[gt][j][k] != ' ')
								newNonWSObsCoords.insert(make_pair(posX + j, posY + k));
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
				for(int j = 0; j < gts; j++) {
					for(int k = 0; k < this->getGraphicLines()[gt][j].length(); k++) {
						if(this->getGraphicLines()[gt][j][k] != ' ')
								newNonWSObsCoords.insert(make_pair(posX + j, posY + k));
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
				setGT(1);
				for(int j = 0; j < gts; j++) {
					world->getObsCoords().
						insert(make_pair(posX + j, posY + longestGS - 1));
				}
				for(int j = 0; j < longestGS; j++) {
					world->getObsCoords().
						insert(make_pair(posX + gts - 1, posY + j));
				}
				//Update Obstacle's nonWSObsCoords	
				for(int j = 0; j < gts; j++) {
					for(int k = 0; k < this->getGraphicLines()[gt][j].length(); k++) {
						if(this->getGraphicLines()[gt][j][k] != ' ')
								newNonWSObsCoords.insert(make_pair(posX + j, posY + k));
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
				setGT(1);
				for(int j = 0; j < gts; j++) {
					world->getObsCoords().
						insert(make_pair(posX + j, posY + longestGS - 1));
				}
				//Update Obstacle's nonWSObsCoords	
				for(int j = 0; j < gts; j++) {
					for(int k = 0; k < this->getGraphicLines()[gt][j].length(); k++) {
						if(this->getGraphicLines()[gt][j][k] != ' ')
								newNonWSObsCoords.insert(make_pair(posX + j, posY + k));
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
				setGT(1);
				for(int j = 0; j < gts; j++) { 
					world->getObsCoords().
						insert(make_pair(posX + j, posY + longestGS - 1));
				}
				for(int j = 0; j < longestGS; j++) {
					world->getObsCoords().
						insert(make_pair(posX, posY + j));
				}
				//Update Obstacle's nonWSObsCoords	
				for(int j = 0; j < gts; j++) {
					for(int k = 0; k < this->getGraphicLines()[gt][j].length(); k++) {
						if(this->getGraphicLines()[gt][j][k] != ' ')
								newNonWSObsCoords.insert(make_pair(posX + j, posY + k));
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
				for(int j = 0; j < gts; j++) {
					for(int k = 0; k < this->getGraphicLines()[gt][j].length(); k++) {
						if(this->getGraphicLines()[gt][j][k] != ' ')
								newNonWSObsCoords.insert(make_pair(posX + j, posY + k));
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
				setGT(0);
				for(int j = 0; j < gts; j++) { 
					world->getObsCoords().
						insert(make_pair(posX + j, posY));
				}
				for(int j = 0; j < longestGS; j++) {
					world->getObsCoords().
						insert(make_pair(posX, posY + j));
				}
				//Update Obstacle's nonWSObsCoords	
				for(int j = 0; j < gts; j++) {
					for(int k = 0; k < this->getGraphicLines()[gt][j].length(); k++) {
						if(this->getGraphicLines()[gt][j][k] != ' ')
								newNonWSObsCoords.insert(make_pair(posX + j, posY + k));
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
		if(world->getGameMode() == EASY && 
		   mvsSinceLastHit == OBS_REGEN_RATE_EASY)
			hits--;
		else if(world->getGameMode() == NORMAL && 
				mvsSinceLastHit == OBS_REGEN_RATE_NORMAL)
			hits--;
		else if(world->getGameMode() == HARD && 
				mvsSinceLastHit == OBS_REGEN_RATE_HARD)
			hits--;
	}
}


// References
// https://ubuntuforums.org/showthread.php?t=836043
// https://stackoverflow.com/questions/17663186/initializing-a-two-dimensional-stdvector
// https://stackoverflow.com/questions/1115854/how-to-resolve-error-bad-index-fatal-index-file-corrupt-when-using-git