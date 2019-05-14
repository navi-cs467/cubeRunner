/***************************************************
** Program name: printMenu.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/23/2019
** Description: Source file for printMenu function.
****************************************************/

#include "../HEADER/printMenu.hpp"

//Prints a menu (using ncurses library) and returns (pointer to)
//subwindow object containing printed window. Menu items are 
//designated in the menuItems parameter. Starting row and column 
//is given  by default according to the size of the graphic above 
//(MM_GRAPHIC_WIDTH x MM_GRAPHIC_HEIGHT), but these values can
//be overridden by specifying the default parameters altStartingCol
//and altStartingRow.
WINDOW *printMenu(vector<string> menuItems, int seedColor, 
					int lineColors[], int menuLength, int menuWidth, 
					int altStartingCol, int altStartingRow) { 
	
	int startingCol, startingRow;
	//Setup subscreen for menu
	if(!altStartingCol) startingCol = (COLS - MM_GRAPHIC_WIDTH)/2 + 
						(MM_GRAPHIC_WIDTH - MM_WIDTH)/2 + 1;
	else startingCol = altStartingCol;						
	if(!altStartingRow) startingRow = ((LINES - MM_GRAPHIC_HEIGHT)/4) + 
							MM_GRAPHIC_HEIGHT + 5 + 1;	//By default starts 5 rows below cube graphic, + 1 due to outer border
	else startingRow = altStartingRow;
	WINDOW *subscrnMenu = newwin(menuLength + 2, menuWidth, startingRow, startingCol);			
	wattron(subscrnMenu, COLOR_PAIR(WHITE_BLACK));							
	if(seedColor != -1) {
		box(subscrnMenu, '|', '_'); 
		wborder(subscrnMenu, '|', '|', '-', '-', '*', '*', '*', '*');
	}
	wrefresh(subscrnMenu);
	
	//Makes sure nothing erroneous is printed next time cmdoutlinesGraphics
	//is used.
	for(vector<string>::iterator it = cmdoutlines.begin();
		it < cmdoutlines.end(); it++)
			*it = "";
	cmdoutlines.clear();
	
	//Print and store menu line items and line colors...
	int i = 1;
	wattron(subscrnMenu, A_BOLD);
	for(vector<string>::iterator it = menuItems.begin();
			it != menuItems.end(); it++, i++) {
		if(seedColor == BLUE_BLACK) seedColor++;	//No blue, too hard to see against black
		if(lineColors && seedColor != -1) lineColors[i - 1] = seedColor;
		if(seedColor != -1) wattron(subscrnMenu, COLOR_PAIR(seedColor));
		else wattron(subscrnMenu, COLOR_PAIR(WHITE_BLACK));
		if(seedColor != -1) 
			mvwprintw(subscrnMenu, i, (menuWidth - it->length())/2, it->c_str()); 	//Centered for menu 1 and 2
			//mvwprintw(subscrnMenu, i, 6, it->c_str()); 	//Slightly off-centered when not highlighted
		else
			mvwprintw(subscrnMenu, i, 0, it->c_str());	//left - justified for menu 3
		cmdoutlines.push_back(*it);		//Stores current menu items in cmdoutlines
		wrefresh(subscrnMenu);
		if(seedColor != -1) seedColor++;
		if(seedColor == 7) seedColor = 1;
	}

	return subscrnMenu;
	
}