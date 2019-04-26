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
		vector<WINDOW*> subscrns;
		int gameMode, numPlayers;
		
	public:
		World(int gameMode, int numPlayers) :
			gameMode(gameMode), numPlayers(numPlayers) {}
		void updateObsCoords(Obstacle *ob);
		virtual void renderWorld() = 0;
		virtual void renderWorld() = 0;
		virtual void scroll() = 0;
		
		//The following two functions are used by the Obstacle
		//constructor to ensure a new obstacle is not placed on
		//top of an existing obstacle or minicube.
		set<pair<int, int>>& getWorldObsCoords() {return obsCoords;}
		set<pair<int, int>>& getWorldMiniCubes() {return miniCubes;};
};
		
		
// Ref
// http://www.cplusplus.com/reference/utility/pair/pair/
// http://www.cplusplus.com/reference/set/set/find/