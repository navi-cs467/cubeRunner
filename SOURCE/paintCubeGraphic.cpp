/**********************************************************
** Program name: paintCubeGraphic.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/23/2019
** Description: Source file for paintCubeGraphic function.
**********************************************************/

#include "../HEADER/paintCubeGraphic.hpp"

//Helper function to print and animate the cube graphic for
//game menu. (Aides in movement across the screen via the 
//offset parameter.) Returns (pointer to) ncurses subwindow 
//object with painted cube graphic.
WINDOW* paintCubeGraphic(WINDOW *subscrnGraphic, 
							const char* fileName, int offset) {
	//Clear subscrnGraphic, if not NULL
	if(subscrnGraphic){
		werase(subscrnGraphic);
		wrefresh(subscrnGraphic);
		delwin(subscrnGraphic);
	}
	
	//Setup subscreen for cube graphic
	//Graphic starts one-quarter of the way down the screen
	int startingColCG = ((COLS - MM_GRAPHIC_WIDTH)/2) + offset,
		startingRowCG = (LINES - MM_GRAPHIC_HEIGHT)/4;		
	subscrnGraphic = newwin(MM_GRAPHIC_HEIGHT, MM_GRAPHIC_WIDTH, 
		startingRowCG, startingColCG);
	
	//Paint cube graphic
	paintGraphic(subscrnGraphic, fileName, rand()%6+1, false);
	
	return subscrnGraphic;
}