/******************************************************
** Program name: paintGraphic.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/23/2019
** Description: Source file for paintGraphic function.
*******************************************************/

#include "../HEADER/paintGraphic.hpp"

//Paints graphic contained in file named fileName. Graphic should be
//MM_GRAPHIC_WIDTH x MM_GRAPHIC_HEIGHT for proper positioning on the
//screen. (Set MM_GRAPHIC_WIDTH & MM_GRAPHIC_HEIGHT in constants.hpp.)
void paintGraphic(WINDOW *subscrn, const char* fileName, 
					int seedColor, bool toggled, int offset) {
	
	loadGraphic(fileName);
	
	if(toggled) {
		//Clear contents of sub-window
		//wattron(subscrn, COLOR_PAIR(BLACK_BLACK));
		//for (int y = 0; y < MM_GRAPHIC_HEIGHT; y++)
		//	mvwhline(subscrn, y, 0, ' ', MM_GRAPHIC_WIDTH);
		werase(subscrn);
		wrefresh(subscrn);
	}
	
	//Paint the rows to the screen
    int color, row;
    for (row = 0, color = seedColor; 
			row < MM_GRAPHIC_HEIGHT; row++, color++) {  
       if(color == 7) color = 1;		//Cycle to first index when necessary
	   //Change color
	   wattron(subscrn, COLOR_PAIR(color)); 
	   mvwaddstr(subscrn, row, 0,
		 cmdoutlinesGraphics[row].
			substr(offset > 0 ? offset - 1 : 0,
				   MM_GRAPHIC_WIDTH - offset - 1).c_str());							 				
		//usleep(100 * 1000);	
	   wrefresh(subscrn);  
							
    }
}