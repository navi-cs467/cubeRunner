/*****************************************************
** Program name: hostPrompt.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/23/2019
** Description: Source file for hostPrompt function.
*****************************************************/

#include "../HEADER/hostPrompt.hpp"

//Prompts the user for a hostname and port number for multi-player
//gameplay, via a ncurses sub-window object that is returned. Prints
//an error window/message and returns to previous menu if a network
//connection can not be established with a Cube Runner server.
WINDOW *hostPrompt(int startingColMenu3, int startingRowMenu3,
						WINDOW **subscrnGraphic, int *currMenu,
						bool *escaped, char *host, char *port) {
	//Replace Game Menu header with hostname/port prompt
	attron(COLOR_PAIR(WHITE_BLACK));
	mvaddstr(startingRowMenu3 - 2, startingColMenu3, networkPrompt.c_str());
	refresh();
	WINDOW *subscrnMenu = printMenu(menu3Items, -1, NULL,
								MENU3_LENGTH, COLS, startingColMenu3,
								startingRowMenu3);
	*subscrnGraphic = paintCubeGraphic(*subscrnGraphic, "GRAPHICS/menuCubeRight1_2.txt");
	*currMenu = 3;
	int x = startingRowMenu3 + 1, y = startingColMenu3 + menu3Items[0].length();
	move(x, y);
	curs_set(1);
	//echo();
	refresh();


	// char portStr[6];
	int i = 0, j = 0, ch;

	//Get hostname or IP address
	do{
		ch = getch();
		if((ch == KEY_BACKSPACE || ch == 127) && i > 0) {
			i--;
			mvaddstr(x, --y, "  ");
			move(x, y);
			refresh();
		}
		else if(i < 255 && 
				!(ch == KEY_BACKSPACE || ch == 127 || ch == '\n')) {
			host[i++] = ch;
			char tmpStr[2]; tmpStr[0] = ch; tmpStr[1] = '\0';
			mvaddstr(x, y++, tmpStr);
			refresh();
		}
	}
	while(ch != '\n' && ch != 27 && ch != KEY_END);
	//If escape key is entered, go back to menu 2
	if(ch == 27 || ch == KEY_END) {
		*escaped = true;
		curs_set(0);
		noecho();
		refresh();
		//*currMenu = 2;
		werase(subscrnMenu); wrefresh(subscrnMenu); delwin(subscrnMenu);
		return NULL;
	}
	host[i] = '\0';
	
	//Move cursor down
	x = startingRowMenu3 + 2; y = startingColMenu3 + menu3Items[1].length();
	move(x, y);
	refresh();
	
	//Get port number
	i = 0;
	do{
		ch = getch();
		if((ch == KEY_BACKSPACE || ch == 127) && i > 0) {
			i--;
			mvaddstr(x, --y, "  ");
			move(x, y);
			refresh();
		}
		else if(i < 5 && 
				!(ch == KEY_BACKSPACE || ch == 127 || ch == '\n')) {
			port[i++] = ch;
			char tmpStr[2]; tmpStr[0] = ch; tmpStr[1] = '\0';
			mvaddstr(x, y++, tmpStr);
			refresh();
		}
	}
	while(ch != '\n' && ch != 27 && ch != KEY_END);
	//If escape key is entered, go back to menu 2
	if(ch == 27 || ch == KEY_END) {
		*escaped = true;
		curs_set(0);
		noecho();
		refresh();
		//*currMenu = 2;
		werase(subscrnMenu); wrefresh(subscrnMenu); delwin(subscrnMenu);
		return NULL;
	}
	port[i] = '\0';

	curs_set(0);
	
	return subscrnMenu;
}
