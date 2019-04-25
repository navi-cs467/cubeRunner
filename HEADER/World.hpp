/********************************************************************
** Program name: World.hpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Header file for World class. The World class is 
** 				abstract and provides non-derived class specific
**				member declarations and definitions for the dynamic
**				screen layout during gameplay.
********************************************************************/

class World {
	protected:
		list<Obstacle*> obstacles;
		set<pair<int, int>> miniCubes, obsCoords;
		int gameMode, numPlayers;
		
	public:
		World(int gameMode, int numPlayers) :
			gameMode(gameMode), numPlayers(numPlayers) {}
		void updateObsCoords(Obstacle *ob);
		virtual void renderWorld() = 0;
		virtual void renderWorld() = 0;
		virtual void scroll() = 0;
		friend list<Obstacle*>& Obstacle::getWorldObs();
};
		
		
// Ref
// http://www.cplusplus.com/reference/utility/pair/pair/
// http://www.cplusplus.com/reference/set/set/find/