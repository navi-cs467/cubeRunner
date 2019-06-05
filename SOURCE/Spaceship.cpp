/********************************************************************
** Program name: Spaceship.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Source file for Spaceship class, a derived Obstacle.
********************************************************************/

#include "../HEADER/Spaceship.hpp"

Spaceship::Spaceship(World *world, Direction offScreen, 
			 int specificGraphic) : Obstacle() {
	
	isStationary = false;
	//curDirection = left;
	//sameDirectionMoveCount = 0;
	createObstacle(world, offScreen, specificGraphic);
	
	//Update size of graphic type array 
	//and length of longest graphic wstring
	gts = Spaceship::getGraphicLines()[gt].size();
	longestGS = 0;
	for(int i = 0; i < gts; i++)
		if(Spaceship::getGraphicLines()[gt][i].size() > longestGS)
			longestGS = Spaceship::getGraphicLines()[gt][i].size();
	
	//Update World's obsCoords and this Obstacle's
	//nonWSObsCoords with new graphic position
	world->updateObsCoords(this);
	
	//Initialize maxHits based on gameMode
	if(world->getGameMode() == EASY) maxHits = 1;
	else if(world->getGameMode() == NORMAL) maxHits = 1;
	else maxHits = 2;
	
	//Initialize hits ands mvsSinceLastHit
	hits = 0;
	mvsSinceLastHit = 0;
}

vector<vector<wstring>> Spaceship::getGraphicLines() {
	return Spaceship::_getGraphicLines();
}

vector<vector<wstring>> Spaceship::initializeVectorGraphics () {
	vector<vector<wstring>> tmpV {{L" \\_____",
								    L"==_____|>",
								    L" /"},
									
								   {L"  _____/",
								    L"<|_____==",
								    L"       \\"},
		
								  {L" /\\",
								   L"/  \\",
								   L"| u|",
								   L"| s|",
								   L"| a|",
								   L"/||\\"},
								   
								  {L" /\\",
								   L"/  \\",
								   L"| o|",
								   L"| s|",
								   L"| u|",
								   L"/||\\"},
								
								  {L"\\||/",
								   L"|  |",
								   L"|  |",
								   L"|__|",
								   L"\\ */",
								   L" \\/"}};
								  
	return tmpV;
}

vector<vector<wstring>> Spaceship::graphicLines = 
	Spaceship::initializeVectorGraphics();	  

//Color scheme	
int Spaceship::color = WHITE_BLACK;

//Spaceship needs its own move to manage the transition
//between vertically traveling spaceships and horizontally
//traveling spaceships.
void Spaceship::move(World* world) {
	
	Direction mvDir; //int testMvDir;
	//Randomly assign a new "seed" direction for the
	//following for loop if the last move direction has 
	//been "exhausted" per the move counter. 
	//(Asteroid only changes direction if it hits something)
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
					for(int k = 0; k < longestGS; k++) {
					itObs = world->getObsCoords().
						find(make_pair(posX + j, posY + k));
					if(itObs != world->getObsCoords().end())
						world->getObsCoords().erase(itObs);
					}
				}
				//Update Obstacle position and graphic
				posY--;
				setGT(1);
				//Insert new obsCoords
				for(int j = 0; j < gts; j++) {
					for(int k = 0; k < longestGS; k++) {
					world->getObsCoords().
						insert(make_pair(posX + j, posY + k));
					}
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
					for(int k = 0; k < longestGS; k++) {
					itObs = world->getObsCoords().
						find(make_pair(posX + j, posY + k));
					if(itObs != world->getObsCoords().end())
						world->getObsCoords().erase(itObs);
					}
				}
				//Update Obstacle's position and graphic
				posX++; posY--;
				setGT(1);
				//Insert new obsCoords
				for(int j = 0; j < gts; j++) {
					for(int k = 0; k < longestGS; k++) {
					world->getObsCoords().
						insert(make_pair(posX + j, posY + k));
					}
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
				for(int j = 0; j < gts; j++) {
					for(int k = 0; k < longestGS; k++) {
					itObs = world->getObsCoords().
						find(make_pair(posX + j, posY + k));
					if(itObs != world->getObsCoords().end())
						world->getObsCoords().erase(itObs);
					}
				}
				//Update Obstacle's position and (if Spaceship) graphic
				posX++;
				setGT(4);
				//Insert new obsCoords
				for(int j = 0; j < gts; j++) {
					for(int k = 0; k < longestGS; k++) {
					world->getObsCoords().
						insert(make_pair(posX + j, posY + k));
					}
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
					for(int k = 0; k < longestGS; k++) {
					itObs = world->getObsCoords().
						find(make_pair(posX + j, posY + k));
					if(itObs != world->getObsCoords().end())
						world->getObsCoords().erase(itObs);
					}
				}
				//Update Obstacle's position and graphic
				posX++; posY++;
				setGT(0);
				//Insert new obsCoords
				for(int j = 0; j < gts; j++) {
					for(int k = 0; k < longestGS; k++) {
					world->getObsCoords().
						insert(make_pair(posX + j, posY + k));
					}
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
					for(int k = 0; k < longestGS; k++) {
					itObs = world->getObsCoords().
						find(make_pair(posX + j, posY + k));
					if(itObs != world->getObsCoords().end())
						world->getObsCoords().erase(itObs);
					}
				}
				//Update position and graphic
				posY++;
				setGT(0);
				//Insert new obsCoords
				for(int j = 0; j < gts; j++) {
					for(int k = 0; k < longestGS; k++) {
					world->getObsCoords().
						insert(make_pair(posX + j, posY + k));
					}
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
					for(int k = 0; k < longestGS; k++) {
					itObs = world->getObsCoords().
						find(make_pair(posX + j, posY + k));
					if(itObs != world->getObsCoords().end())
						world->getObsCoords().erase(itObs);
					}
				}
				//Update Obstacle's position and graphic
				posX--; posY++;
				setGT(0);
				//Insert new obsCoords
				for(int j = 0; j < gts; j++) {
					for(int k = 0; k < longestGS; k++) {
					world->getObsCoords().
						insert(make_pair(posX + j, posY + k));
					}
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
				for(int j = 0; j < gts; j++) {
					for(int k = 0; k < longestGS; k++) {
					itObs = world->getObsCoords().
						find(make_pair(posX + j, posY + k));
					if(itObs != world->getObsCoords().end())
						world->getObsCoords().erase(itObs);
					}
				}
				//Update Obstacle's position and (if Spaceship) graphic
				posX--;
				//Show OSU or USA "side" of Spaceship graphic?
				int osuOrUSA = rand();
				if(osuOrUSA % 2 == 0) setGT(2);
				else setGT(3);
				//Insert new obsCoords
				for(int j = 0; j < gts; j++) {
					for(int k = 0; k < longestGS; k++) {
					world->getObsCoords().
						insert(make_pair(posX + j, posY + k));
					}
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
					for(int k = 0; k < longestGS; k++) {
					itObs = world->getObsCoords().
						find(make_pair(posX + j, posY + k));
					if(itObs != world->getObsCoords().end())
						world->getObsCoords().erase(itObs);
					}
				}
				//Update Obstacle's position and graphic
				posX--; posY--;
				setGT(1);
				//Insert new obsCoords
				for(int j = 0; j < gts; j++) {
					for(int k = 0; k < longestGS; k++) {
					world->getObsCoords().
						insert(make_pair(posX + j, posY + k));
					}
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
// https://stackoverflow.com/questions/56120664/converting-typeid-to-namespace-for-static-member-access-c?noredirect=1#comment98874550_56120664