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
						 char* fileName, int offset,
						 int obsEndCol, int obsColor, 
						 int backgroundColor) {
	//Clear subscrnGraphic, if not NULL
	if(subscrnGraphic){
		werase(subscrnGraphic);
		wrefresh(subscrnGraphic);
		delwin(subscrnGraphic);
	}
	
	//Setup subscreen for cube graphic
	//Graphic starts one-quarter of the way down the screen
	int startingColCG = ((COLS - MM_GRAPHIC_WIDTH)/2) + offset > 0 ?
						((COLS - MM_GRAPHIC_WIDTH)/2) + offset :
						0,
		startingRowCG = (LINES - MM_GRAPHIC_HEIGHT)/4;		
	subscrnGraphic = newwin(MM_GRAPHIC_HEIGHT, MM_GRAPHIC_WIDTH, 
		startingRowCG, startingColCG);
	
	//Paint cube graphic
	if(offset < -(COLS - MM_GRAPHIC_WIDTH)/2) 
		paintGraphic(subscrnGraphic, fileName, rand()%6+1, false,
				 -(offset + (COLS - MM_GRAPHIC_WIDTH)/2),
				 obsEndCol, obsColor, backgroundColor);
	else
		paintGraphic(subscrnGraphic, fileName, rand()%6+1, false, 0,
		obsEndCol, obsColor, backgroundColor);
	
	return subscrnGraphic;
}