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
	
	int longest = 0;
	if(typeid(*ob) == typeid(Seaweed)) {
		//find length of longest string in graphicLines array
		for(int i = 0; i < Seaweed::getGraphicLines()[ob->getGT()].size(); i++)
			if(Seaweed::getGraphicLines()[ob->getGT()][i].size() > longest)
				longest = Seaweed::getGraphicLines()[ob->getGT()][i].size();
		
		//Update obsCoords	
		for(int i = 0; i < Seaweed::getGraphicLines()[ob->getGT()].size(); i++)
			for(int j = 0; j < longest; j++)
				obsCoords.insert(make_pair(ob->getPosX() + i,
										   ob->getPosY() + j));
	}
	else if(typeid(*ob) == typeid(Coral)) {
		//find length of longest string in graphicLines array
		for(int i = 0; i < Coral::getGraphicLines()[ob->getGT()].size(); i++)
			if(Coral::getGraphicLines()[ob->getGT()][i].size() > longest)
				longest = Coral::getGraphicLines()[ob->getGT()][i].size();
	
		//Update obsCoords
		for(int i = 0; i < Coral::getGraphicLines()[ob->getGT()].size(); i++)
			for(int j = 0; j < longest; j++)
				obsCoords.insert(make_pair(ob->getPosX() + i,
										   ob->getPosY() + j));
	}
	else if(typeid(*ob) == typeid(Shark)) {
		//find length of longest string in graphicLines array
		for(int i = 0; i < Shark::getGraphicLines()[ob->getGT()].size(); i++)
			if(Shark::getGraphicLines()[ob->getGT()][i].size() > longest)
				longest = Shark::getGraphicLines()[ob->getGT()][i].size();
	
		//Update obsCoords
		for(int i = 0; i < Shark::getGraphicLines()[ob->getGT()].size(); i++)
			for(int j = 0; j < longest; j++)
				obsCoords.insert(make_pair(ob->getPosX() + i,
										   ob->getPosY() + j));
	}
	else if(typeid(*ob) == typeid(Octopus)) {
		//find length of longest string in graphicLines array
		for(int i = 0; i < Octopus::getGraphicLines()[ob->getGT()].size(); i++)
			if(Octopus::getGraphicLines()[ob->getGT()][i].size() > longest)
				longest = Octopus::getGraphicLines()[ob->getGT()][i].size();
	
		//Update obsCoords
		for(int i = 0; i < Octopus::getGraphicLines()[ob->getGT()].size(); i++)
			for(int j = 0; j < longest; j++)
				obsCoords.insert(make_pair(ob->getPosX() + i,
										   ob->getPosY() + j));
	}
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
	