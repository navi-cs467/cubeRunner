/*********************************************************
** Program name: validateWinSize.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/23/2019
** Description: Source file for validateWinSize function.
*********************************************************/

#include "../HEADER/validateWinSize.hpp"

//Validates terminal window is at least 
//MIN_WIN_WIDTH x MIN_WIN_HEIGHT. User is prompted to
//resize window and restart the game if window size does
//not meet these criteria. (See MIN_WIN_HEIGHT & 
//MIN_WIN_WIDTH in validateWinSize.hpp.)
void validateWinSize() {
	
	//Hide the cursor
	curs_set(0);
	
	clear();  //Curses clear-screen call
	
	//Min window size is 135x30
    //Enforce this minimum, asking the user to resize the window
    //and restart the game if necessary.
    if(LINES < MIN_WIN_HEIGHT || COLS < MIN_WIN_WIDTH) {
		// Paint screen red
		attron(COLOR_PAIR(RED_RED));
		attron(A_BOLD);
		for (int y = 0; y < LINES; y++) {
			mvhline(y, 0, ' ', COLS);
		}
		
		//Create sub-window, place in center of main window
		int startingCol = 0, startingRow = 0;
		if(LINES - 8 > 0) startingRow = (LINES - 8)/2;
		if(COLS - 44 > 0) startingCol = (COLS - 44)/2;
		WINDOW *subscrn = newwin(8, 44, startingRow, startingCol);
		
		//Paint window red
		wattron(subscrn, COLOR_PAIR(RED_RED));
		wattron(subscrn, A_BOLD);
		for (int y = 0; y < LINES; y++) {
			mvwhline(subscrn, y, 0, ' ', COLS);
		}
		refresh();
		
		//Change attribute to white foreground (for sub-window)
		wattron(subscrn, COLOR_PAIR(WHITE_RED));
		
		//Put a border around sub-window and output resize message 
		box(subscrn, '|', '_'); 
		wborder(subscrn, '|', '|', '-', '-', '*', '*', '*', '*');
		//Convert MIN_WIN_WIDTH and MIN_WIN_HEIGHT to c-string representation
		ostringstream ostr;
		ostr << "Minimum window size is " << MIN_WIN_WIDTH << "x" << MIN_WIN_HEIGHT << ".";
		mvwprintw(subscrn, 1, 7, ostr.str().c_str());
		//Convert COLS and LINES to c-string representation
		ostr.str(""); ostr.clear();
		ostr << "(Current window size is " << COLS << "x" << LINES << ".)";
		mvwprintw(subscrn, 2, 6, ostr.str().c_str());
		mvwprintw(subscrn, 4, 3, "Please resize your window and relaunch.");
		mvwprintw(subscrn, 6, 11, "Press any key to exit.");
		refresh();
		wrefresh(subscrn);
		getch();
		
		//Cleanup  and exit
		delwin(scrn);
		endwin();
		exit(1);
    }
}