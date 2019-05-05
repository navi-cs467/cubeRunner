/********************************************************************
** Program name: Obstacle.hpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Header file for Obstacle class. The Obstacle class 
**				is abstract and provides non-derived class specific
**				member declarations for the game obstacles.
********************************************************************/

#ifndef Obstacle_hpp
#define Obstacle_hpp

#include "using.hpp"
#include "Direction.hpp"

//Forward declaration needed to workaround a
//cyclic dependency. (https://stackoverflow.com/questions/
//2133250/does-not-name-a-type-error)
class World;

class Obstacle {
	protected:
		int posX;
		int posY;
		bool isStationary;
		void createObstacle(World *world, 
							vector<vector<string>> graphicLines, 
							Direction offScreenDirection = none,
							int specificGraphic = -1);
		int gt;	//Short for "graphic type"

	public:
		int getPosX() {return posX;}
		int getPosY() {return posY;}
		void setPosX(int posX) {this->posX = posX;}
		void setPosY(int posY) {this->posY = posY;}
		bool getIsStationary() {return isStationary;}
		int getGT() {return gt;}
		void setGT(int newGraphic) {gt = newGraphic;}
		virtual Direction getDirection() {return none;}
		
		//Renders class abstract, since there is no
		//candidate for a pure virtual function that
		//is used by derived classes. (Also ensures
		//memory is managed properly when deleting
		//derived class objects via base class pointers.)
		virtual ~Obstacle() {}
};

#endif /* Obstacle_hpp */

// References
// https://stackoverflow.com/questions/461203/when-to-use-virtual-destructors
// https://www.geeksforgeeks.org/virtual-destructor/