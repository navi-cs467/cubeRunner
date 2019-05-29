/*****************************************************
** Program name: userPrompt.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 5/28/2019
** Description: Source file for userPrompt function.
*****************************************************/

include "../HEADER/userPrompt.hpp"

//Prompts the user for a username
//ia a ncurses sub-window object that is returned. Prints
//an error window/message and returns to previous menu if a network
//connection can not be established with a Cube Runner server.
WINDOW *userPrompt(int startingColMenu4, int startingRowMenu4,
						WINDOW **subscrnGraphic, int *currMenu,
						bool *escaped, char *username, bool twoPlayer) {
	//Replace Game Menu header with username prompt message
	attron(COLOR_PAIR(WHITE_BLACK));
	mvaddstr(startingRowMenu4 - 2, startingColMenu4, usernamePrompt.c_str());
	refresh();
	WINDOW *subscrnMenu = printMenu(menu4Items, -1, NULL,
								MENU4_LENGTH, COLS, startingColMenu4,
								startingRowMenu4);

  if (twoPlayer == false)
  {
    *subscrnGraphic = paintCubeGraphic(*subscrnGraphic, "GRAPHICS/menuCubeRight1_1.txt");
  }

  if (twoPlayer == true)
  {
    *subscrnGraphic = paintCubeGraphic(*subscrnGraphic, "GRAPHICS/menuCubeRight1_2.txt");
  }

	*currMenu = 4;
	int x = startingRowMenu4 + 1, y = startingColMenu4 + menu4Items[0].length();
	move(x, y);
	curs_set(1);
	//echo();
	refresh();


	// char portStr[6];
	int i = 0, j = 0, ch;

	//Get username
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
			username[i++] = ch;
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
	username[i] = '\0';

	curs_set(0);

	return subscrnMenu;
}
