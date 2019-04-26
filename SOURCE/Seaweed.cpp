/********************************************************************
** Program name: Seaweed.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Source file for Seaweed class, a derived Obstacle.
********************************************************************/

Seaweed::Seaweed(World* world) : Obstacle(world), isStationary(true) {
	
	//Find random starting position that does not
	//encroach on existing world obstacles or miniCubes
	set<pair<int,int>>::iterator it1, it2;
	bool enchroaches = false;
	do {
		posX = rand() % LINES - 4;
		posY = rand() % COLS + 10;		//No obstacles start in first 10 columns
		for(int i = 0; i < graphicLines.length(); i++) 
			for(int j = 0; j < graphicLines[i].length(); j++)
				it1 = world->getWorldObsCoords().find(make_pair(i, j));
				it2 = world->getMiniCubes().find(make_pair(i, j));
				if(it1 != world->getWorldObsCoords().end() || 
				   it2 != world->getMiniCubes().end())
					enchroaches = true;
	} while(enchroaches == true);