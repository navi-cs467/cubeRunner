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

//Note: the 'toggled' parameter is no longer used but has been
//left in place in case it may be needed later. All function calls
//still pass an argument for toggled.
void paintGraphic(WINDOW *subscrn, char* fileName, 
				  int seedColor, bool toggled, int offset,
				  int obsEndCol, int obsColor, int backgroundColor) {
	
	//Couldn't get Unicode characters to read from file properly, 
	//so hard-coded this graphic...
	if(strcmp(fileName, "GRAPHICS/sharkChase.txt") == 0) {
		cmdoutlinesGraphics[0] = L"            ,`\\";
		cmdoutlinesGraphics[1] = L",\\      ____)  \\________________";
		cmdoutlinesGraphics[2] = L"\\ `────´                       /   ___";
		cmdoutlinesGraphics[3] = L"{ ,──._         « « «  <o  ___/   /\\__\\";
		cmdoutlinesGraphics[4] = L" `     `─.__    ´ ´ ´     /´´´    \\/__/";
		cmdoutlinesGraphics[5] = L"            \\/---|  )--.  \\.";
		cmdoutlinesGraphics[6] = L"                 ;,´    `-´";
		cmdoutlinesGraphics[7] = L"";
		cmdoutlinesGraphics[8] = L"";
		cmdoutlinesGraphics[9] = L"";
	}
		
	else loadGraphic(fileName);
	
	//Clear contents of sub-window
	if(backgroundColor == -1) wattron(subscrn, COLOR_PAIR(BLACK_BLACK));
	else wattron(subscrn, COLOR_PAIR(backgroundColor));
	for (int y = 0; y < MM_GRAPHIC_HEIGHT; y++)
		mvwhline(subscrn, y, 0, ' ', MM_GRAPHIC_WIDTH);
	werase(subscrn);
	wrefresh(subscrn);
	
	//Paint the rows to the screen
    int color, row;
	wattron(subscrn, A_BOLD);
    for (row = 0, color = seedColor; 
			row < MM_GRAPHIC_HEIGHT; row++, color++) {  
       if(color == 7) color = 1;		//Cycle to first index when necessary
	   for (int col = offset, j = 0; col < cmdoutlinesGraphics[row].
								substr(0, MM_GRAPHIC_WIDTH - 1).length(); 
			col++, j++) {
		   //Change color
		   if(cmdoutlinesGraphics[row][col] == '=')
			   wattron(subscrn, COLOR_PAIR(RED_BLACK));
		   else if(obsEndCol == -1 || col > obsEndCol)
			   wattron(subscrn, COLOR_PAIR(color)); 
		   else wattron(subscrn, COLOR_PAIR(obsColor));
		   //mvwaddstr(subscrn, row, 0,
		   //	cmdoutlinesGraphics[row].
		   //	substr(offset > 0 ? offset - 1 : 0,
		   //		   MM_GRAPHIC_WIDTH - offset - 1).c_str());
		  //mvwaddwch(subscrn, row, j, cmdoutlinesGraphics[row][col]);
		  wmove(subscrn, row, j);
		  wprintw(subscrn, "%lc", cmdoutlinesGraphics[row][col]);
	   }
    }
	wrefresh(subscrn);  
	//usleep(100 * 1000);	
}