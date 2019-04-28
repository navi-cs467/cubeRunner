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

void World::updateObsCoords(Obstacle *ob) {
	if(typeid(*ob) == typeid(Seaweed)
		for(int i = 0; i < Seaweed::graphicLines.length(); i++)
			for(j = 0; j < Seaweed::graphicLines[i].length(); j++)
				obsCoords.insert(make_pair(ob->getXCoord + i,
										   ob->getYCoord + j));
	else if(typeid(*ob) == typeid(Coral)
		for(int i = 0; i < Seaweed::graphicLines.length(); i++)
			for(j = 0; j < Seaweed::graphicLines[i].length(); j++)
				obsCoords.insert(make_pair(ob->getXCoord + i,
										   ob->getYCoord + j));
	if(typeid(*ob) == typeid(Shark)
		for(int i = 0; i < Seaweed::graphicLines.length(); i++)
			for(j = 0; j < Seaweed::graphicLines[i].length(); j++)
				obsCoords.insert(make_pair(ob->getXCoord + i,
										   ob->getYCoord + j));
	if(typeid(*ob) == typeid(Octopus)
		for(int i = 0; i < Seaweed::graphicLines.length(); i++)
			for(j = 0; j < Seaweed::graphicLines[i].length(); j++)
				obsCoords.insert(make_pair(ob->getXCoord + i,
										   ob->getYCoord + j));
}
	