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

void World::updateObsCoords(Obstacle *ob) {
	if(typeid(*ob) == typeid(Seaweed1))
		for(int i = 0; i < Seaweed1::getLengthGL(); i++)
			for(int j = 0; j < Seaweed1::getGraphicLines()[i].length(); j++)
				obsCoords.insert(make_pair(ob->getPosX() + i,
										   ob->getPosY() + j));
	/* else if(typeid(*ob) == typeid(Coral)
		for(int i = 0; i < Coral::graphicLines.length(); i++)
			for(j = 0; j < Coral::graphicLines[i].length(); j++)
				obsCoords.insert(make_pair(ob->getXCoord + i,
										   ob->getYCoord + j));
	else if(typeid(*ob) == typeid(Shark)
		for(int i = 0; i < Shark::graphicLines.length(); i++)
			for(j = 0; j < Shark::graphicLines[i].length(); j++)
				obsCoords.insert(make_pair(ob->getXCoord + i,
										   ob->getYCoord + j));
	else if(typeid(*ob) == typeid(Octopus)
		for(int i = 0; i < Octopus::graphicLines.length(); i++)
			for(j = 0; j < Octopus::graphicLines[i].length(); j++)
				obsCoords.insert(make_pair(ob->getXCoord + i,
										   ob->getYCoord + j)); */
}

void World::initMiniCubes(int numMiniCubes) {
	//Find random starting position that does not
	//encroach on existing world obstacles
	set<pair<int,int>>::iterator it;
	bool enchroaches;
	int posX, posY;
	for(int i = 0; i < numMiniCubes; i++) {
		do {
			posX = rand() % bottomRow;
			posY = (rand() % (COLS - 10)) + 10;		//No miniCubes start in first 10 columns
			it = obsCoords.find(make_pair(posX, posY));
			if(it != obsCoords.end())
				enchroaches = true;
			else enchroaches = false;
		} while(enchroaches == true);
		miniCubes.insert(make_pair(posX, posY));
	}
}
	