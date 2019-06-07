/**************************************************************
** Program name: noServerAvailable.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 5/31/2019
** Description: Source file for noServerAvailable function.
****************************************************************/

#include "../HEADER/noServerAvailable.hpp"

//Message informing the player that the other player has terminated early
//let player confirm
void noServerAvailable(char *host, char *port,
					   int *userInput, int *confirmedGameOver) {

	clear();  //Curses clear-screen call

	//Explicitly set userInput
	*userInput = 0;

	//Paint screen black
	attron(COLOR_PAIR(BLACK_BLACK));
	attron(A_BOLD);
	for (int y = 0; y < LINES; y++) {
		mvhline(y, 0, ' ', COLS);
	}
	refresh();
	
	ostringstream ostr1, ostr2;
	ostr1 << "No cubeRunner server available on " << host << ":" << port;
	ostr2 << "Please re-enter host information and try again.";

	//Create sub-window, place in center of main window
	int startingCol = 0, startingRow = 0, 
	width = (ostr1.str().length() > ostr2.str().length() ? 
			 ostr1.str().length() : ostr2.str().length()) + 4;
	startingRow = (LINES - 8)/2;
	startingCol = (COLS / 2) - (width / 2) + 4;
	WINDOW *subscrn = newwin(8, width, startingRow, startingCol);

	//Paint window black
	wattron(subscrn, COLOR_PAIR(BLACK_BLACK));
	wattron(subscrn, A_BOLD);
	for (int y = 0; y < LINES; y++) {
		mvwhline(subscrn, y, 0, ' ', COLS);
	}
	refresh();

	//Change attribute to white foreground (for sub-window)
	wattron(subscrn, COLOR_PAIR(WHITE_BLACK));

	//Put a border around sub-window and output resize message
	box(subscrn, '|', '_');
	wborder(subscrn, '|', '|', '-', '-', '*', '*', '*', '*');
	
	mvwprintw(subscrn, 1, (width - 4) / 2 - (ostr1.str().length() / 2) + 2, 
						   ostr1.str().c_str());
	mvwprintw(subscrn, 3, (width - 4) / 2 - (ostr2.str().length() / 2) + 2, 
						   ostr2.str().c_str());
	mvwprintw(subscrn, 6, (width - 4) / 2 - (25 / 2) + 2,
						   "Press Any Key to Continue");
	refresh();
	wrefresh(subscrn);

	*confirmedGameOver = 1;

	delwin(subscrn);
}
