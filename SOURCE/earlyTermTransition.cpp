/**************************************************************
** Program name: earlyTermTransition.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 5/31/2019
** Description: Source file for earlyTermTransition function.
****************************************************************/

#include "../HEADER/earlyTermTransition.hpp"

//Message informing the player that the other player has terminated early
//let player confirm
void earlyTermTransition(int playerNum, int *userInput, int *confirmedGameOver, bool *hasTerminated) {

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

        ostringstream ostr1;
	ostringstream ostr2;
	if(playerNum == 1)
	{
		ostr1 << "Player 2 has terminated early.";
		ostr2 << "Game score will not be saved.";
	}
	else
	{
		ostr1 << "Player 1 has terminated early.";
		ostr2 << "Game score will not be saved.";
	}
	
	mvwprintw(subscrn, 1, 7, ostr1.str().c_str());
	mvwprintw(subscrn, 2, 7, ostr2.str().c_str());
	mvwprintw(subscrn, 6, 9, "Press Any Key to Continue");
	refresh();
	wrefresh(subscrn);

	if(confirmedGameOver != NULL) {
		*confirmedGameOver = 1;
	}

	//Set hasTerminated true if applicable (multiplayer only)
	if(hasTerminated) *hasTerminated = true;

	delwin(subscrn);
}
