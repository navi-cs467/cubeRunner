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

//Forward declaration needed to workaround a
//cyclic dependency. (https://stackoverflow.com/questions/
//2133250/does-not-name-a-type-error)
class World;

class Obstacle {
	private:
		int posX;
		int posY;
		bool isStationary;
		
	public:
		Obstacle(World* world);
		int getPosX() {return posX;}
		int getPosY() {return posY;}
		void setPosX(int posX) {this->posX = posX;}
		void setPosY(int posY) {this->posY = posY;}
		bool getIsStationary() {return isStationary;}
		
		//Renders class abstract, since there is no
		//candidate for a pure virtual function that
		//is used by derived classes
		virtual ~Obstacle() = 0;
};

#endif /* Obstacle_hpp */