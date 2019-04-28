/********************************************************************
** Program name: Seaweed1.hpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Header file for Seaweed1 class, a derived Obstacle.
********************************************************************/

class Seaweed1 : public Obstacle {
	private:
		static string graphicLines[];
	
	private:
		Seaweed1();
		static string[]& getGraphicLines() {return 
		
	public:
};

string Seaweed1::graphicLines = {string("(   )"),
								 string(" ) ("),
								 string(" ( )"),
								 string(" ) ("),
								 string("(   )")};