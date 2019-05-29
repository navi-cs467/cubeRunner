/********************************************************************
** Program name: Asteroid.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Source file for Asteroid class, a derived Obstacle.
********************************************************************/

#include "../HEADER/Asteroid.hpp"
#include "../HEADER/World.hpp"
#include "../HEADER/Water.hpp"
#include "../HEADER/Land.hpp"

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
				int randomAsteroidChar = rand() % 6;
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

//Asteroid doesn't change direction unless it hits something,
//and never changes graphic type based on direction
void Asteroid::move(World* world) {
	
	//Do not move the Obstacle if it is offscreen left or right
	//for Water and Land Worlds
	if(((typeid(*world) == typeid(Water)) ||
		typeid(*world) == typeid(Land)) &&
		   (posY > COLS || posY < 0 - longestGS)) return;
	
	Direction mvDir; //int testMvDir;
	//Randomly assign a new "seed" direction for the
	//following for loop if the last move direction has 
	//been "exhausted" per the move counter. 
	//(Asteroid only changes direction if it hits something)
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
			if(typeid(*world) != typeid(Space) &&
				 posX + gts == world->getBottomRow() + 1) {
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
			if(typeid(*world) != typeid(Space) &&
				 posX + gts == world->getBottomRow() + 1) {
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
			if(typeid(*world) != typeid(Space) &&
				 posX + gts == world->getBottomRow() + 1) {
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

// References
// https://ubuntuforums.org/showthread.php?t=836043
// https://stackoverflow.com/questions/17663186/initializing-a-two-dimensional-stdvector
// https://stackoverflow.com/questions/1115854/how-to-resolve-error-bad-index-fatal-index-file-corrupt-when-using-git