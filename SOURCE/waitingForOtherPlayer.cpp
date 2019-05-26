/**************************************************************
** Program name: waitingForOtherPlayer.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 5/25/2019
** Description: Source file for waitingForOtherPlayer function.
****************************************************************/

#include "../HEADER/waitingForOtherPlayer.hpp"

//Message informing the player who responds to prompts
//first that the server is waiting for the other player
//to confirm response (multiplayer only).
WINDOW* waitingForOtherPlayer(int playerNum) {
	
	clear();  //Curses clear-screen call
	
	//Paint screen black
	attron(COLOR_PAIR(BLACK_BLACK));
	attron(A_BOLD);
	for (int y = 0; y < LINES; y++) {
		mvhline(y, 0, ' ', COLS);
	}
	refresh();
	
	//Create sub-window, place in center of main window
	int startingCol = 0, startingRow = 0;
	if(LINES - 8 > 0) startingRow = (LINES - 8)/2;
	if(COLS - 44 > 0) startingCol = (COLS - 44)/2;
	WINDOW *subscrn = newwin(8, 44, startingRow, startingCol);
	
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

	ostringstream ostr;
	if(playerNum == 1)
		ostr << "Waiting For Player 2.";
	else
		ostr << "Waiting For Player 1.";
	mvwprintw(subscrn, 1, 7, ostr.str().c_str());
	mvwprintw(subscrn, 6, 11, "Press 'q' to exit.");
	refresh();
	wrefresh(subscrn);
	
	//Return subscreen so it can be deleted once other player has connected
	return subscrn;
}