/**********************************************************
** Program name: cubeRunner.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/23/2019
** Description: Main for Capstone Project "Cube Runner.
**********************************************************/

//See constants.hpp for constant definitions
#include "../HEADER/constants.hpp"

//See using.hpp for "using" declarations
#include "../HEADER/using.hpp"

//Function prototypes
#include "../HEADER/highlight.hpp"
#include "../HEADER/hostPrompt.hpp"
#include "../HEADER/userPrompt.hpp"
#include "../HEADER/initColors.hpp"
#include "../HEADER/transitionAnimation.hpp"
#include "../HEADER/loadGraphic.hpp"
#include "../HEADER/loadInstructions.hpp"
#include "../HEADER/paintCubeGraphic.hpp"
#include "../HEADER/paintGraphic.hpp"
#include "../HEADER/printMenu.hpp"
#include "../HEADER/validateWinSize.hpp"
#include "../HEADER/highScore.hpp"

//Game class
#include "../HEADER/Game.hpp"

//reading text file
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/*************************** GLOBALS *********************************/

WINDOW *scrn; 	//Will point to curses window object

vector<string> cmdoutlines;  			//Screen output vector
vector<wstring> cmdoutlinesGraphics;	//Reserved for graphics only (multi-threaded)

//Menus
const char* menu1[] = {"1 Player",
					   "2 Player",
					   "High Scores",
					   "Instructions",
					   "Exit"};

const char* menu2[] = {"Easy",
					   "Normal",
					   "Hard",
					   "Back"};

const char* menu3[] = {"Hostname or IP Address: ",
						"Port Number: ",
						"",
						"   * Press Enter to Continue at Each Prompt. Press End or Esc To Go Back. *"};

const char* menu4[] = {"Username: ",
						"",
				    "",
						"    *Press End or Esc To Go Back*"};

//Convert c_strings arrays to string vector
vector<string> loadMenuVec(const char** menu, int length) {
	vector<string> retVec;
	for(int i = 0; i < length; i++)
		retVec.push_back(menu[i]);
	return retVec;
}

//Store menu c_str arrays as vectors of strings
const vector<string> menu1Items = loadMenuVec(menu1, MENU1_LENGTH);
const vector<string> menu2Items = loadMenuVec(menu2, MENU2_LENGTH);
const vector<string> menu3Items = loadMenuVec(menu3, MENU3_LENGTH);
const vector<string> menu4Items = loadMenuVec(menu4, MENU4_LENGTH);

//Network prompt
const string networkPrompt = "Please enter a Hostname or IP Address "
							 "and port number to connect to a Cube Runner "
							 "multiplayer server";
//username prompt
const string usernamePrompt = "Please enter a username to begin the game";

/*************************** END GLOBALS *****************************/

int main(void)
{
	setlocale(LC_ALL, "");		//Trying to print unicode square '\u25A0'...

	//Window setup, next 3 lines are curses library calls, a standard
    //initializing sequence for curses programs
    scrn = initscr();
	keypad(stdscr, TRUE);	//Enables use of arrow keys (among other things)
    noecho();  //Don't echo keystrokes
    cbreak();  //Keyboard input valid immediately, not after hit Enter

	//Exit if terminal does not support color
	/* if (has_colors() == FALSE) {
    endwin();
    printf("Your terminal does not support color\n");
    exit(1);
	} */

	initColors();	//Initialize color indexes

	//Make sure users window is at least
	//MIN_WIN_WIDTH x MIN_WIN_HEIGHT (see constants.hpp)
	//(Instructs user to resize window then exits with
	//key press if minimum window dimensions are not met.)
	validateWinSize();

	srand(time(NULL));	//Seed random number generator with system time

    //Run transitionAnimation animation
    transitionAnimation("GRAPHICS/cubeRunner.txt", 120, 28, BLACK_BLACK, 1, RED_BLACK);


	//Variables needed for menu and game
	int cursorPos = 1, currMenu = 1, gameMode = -1;
	bool gameOn = false, escaped = false, isTwoPlayer = false;
	char host[256]; //memset(host, 0, sizeof(char) * 256);
	char port[6]; //memset(port, 0, sizeof(char) * 6);
	char username[MSG_SIZE];
	//WINDOW *subscrnMenuBorder;

	while(1) {


		//Set number of omp threads for menu
		omp_set_num_threads(3);

		//Lock needed so only one thread attempts to modify
		//subscrnGraphic or menu border at a time
		omp_lock_t subscrnGraphicLock;

		//Initialize lock
		omp_init_lock(&subscrnGraphicLock);

		clear();  // curses clear-screen call

		//Paint screen black
		attron(COLOR_PAIR(BLACK_BLACK));
		for (int y = 0; y < LINES; y++) {
			mvhline(y, 0, ' ', COLS);
		}
		refresh();

		//Paint initial cube graphic
		WINDOW* subscrnGraphic = paintCubeGraphic(NULL, "GRAPHICS/menuCubeRight1_1.txt");

		//Setup subscreen for menu outer border
		int startingCol = (COLS - MM_GRAPHIC_WIDTH)/2 +
						  (MM_GRAPHIC_WIDTH - MM_WIDTH) / 2,
			startingRow = ((LINES - MM_GRAPHIC_HEIGHT)/4) +
						   MM_GRAPHIC_HEIGHT + 5;		//Menu starts 5 lines below the graphic


		WINDOW *subscrnMenuBorder = newwin(MENU1_LENGTH + 4, MM_WIDTH + 2, startingRow, startingCol);

		wattron(subscrnMenuBorder, COLOR_PAIR(WHITE_BLACK));
		box(subscrnMenuBorder, '|', '_');
		wborder(subscrnMenuBorder, '|', '|', '-', '-', '*', '*', '*', '*');
		wrefresh(subscrnMenuBorder);

		//Print Game Menu header
		attron(COLOR_PAIR(WHITE_BLACK));
		mvaddstr(startingRow - 1, startingCol + (MM_WIDTH - 7)/2, "Game Menu");
		refresh();

		//Print menu 1 with random starting line color
		int startingLineColor = rand() % 6 + 1, lineColors[MAX_MENU_ITEMS];
		WINDOW *subscrnMenu1 = printMenu(menu1Items, startingLineColor, lineColors,
											MENU1_LENGTH, MM_WIDTH); // change menu2Items to menu1Items

		//Declare menu 2, 3, and 4 (for future use)
		WINDOW *subscrnMenu2, *subscrnMenu3, *subscrnMenu4;;

		//Initialize menu 3 starting position variables
		int startingColMenu3 = startingCol +
			(MM_WIDTH - networkPrompt.length() - 2)/2;
		int startingRowMenu3 = startingRow + 1;

		//menu 4 starting position variables
		int startingColMenu4 = startingCol + (MM_WIDTH - usernamePrompt.length() - 2)/2;
		int startingRowMenu4 = startingRow + 1;

		//Start highlighting at line 1
		highlight(subscrnMenu1, 1, lineColors[0], startingLineColor,
					menu1Items, MENU1_LENGTH, MM_WIDTH);

		//Variables needed for menu and game     /////Move to top of while statement
		/*int cursorPos = 1, currMenu = 1, gameMode = -1;
		bool gameOn = false, escaped = false, isTwoPlayer = false;
		char host[256]; //memset(host, 0, sizeof(char) * 256);
		char port[6]; //memset(port, 0, sizeof(char) * 6);*/

		//Setup multi-threaded block, with three threads as described below...
		#pragma omp parallel sections shared(cursorPos, currMenu, \
											 isTwoPlayer, gameMode, \
											 gameOn, escaped, \
											 subscrnGraphic)
		{
			//This section (thread) handles the cube animation for menu 1,
			//which alternates between various graphics approximately
			//once every second.
			#pragma omp section
			{
				int graphicIterator = cursorPos, seedColor = rand() % 6 + 1,
					prevCurPos = cursorPos;
				bool toggled = true;
				double time = omp_get_wtime();
				while(!gameOn) {
					usleep(100 * 1000);
					if(time + 1 < omp_get_wtime()) {
						time = omp_get_wtime();
						graphicIterator++;
						if(graphicIterator == 4) graphicIterator = 0;
						seedColor++;
						if(seedColor == 7) seedColor = 1;
					}
					if(prevCurPos != cursorPos) {
						toggled = true;
						prevCurPos = cursorPos;
						//Clear and reprint (to address menu "garble" issue, but causes
						//screen "flicker")
						/* clear();
						if(currMenu == 1) {
							//Outer Border
							delwin(subscrnMenuBorder);
							subscrnMenuBorder = newwin(MENU1_LENGTH + 4, MM_WIDTH + 2, startingRow, startingCol);
							wattron(subscrnMenuBorder, COLOR_PAIR(WHITE_BLACK));
							box(subscrnMenuBorder, '|', '_');
							wborder(subscrnMenuBorder, '|', '|', '-', '-', '*', '*', '*', '*');
							wrefresh(subscrnMenuBorder);
							//Menu
							delwin(subscrnMenu1);
							subscrnMenu1 = printMenu(menu1Items, startingLineColor, lineColors,
												MENU1_LENGTH, MM_WIDTH);
							highlight(subscrnMenu1, cursorPos, lineColors[cursorPos-1],
								startingLineColor, menu1Items, MENU1_LENGTH, MM_WIDTH);
						}
						else {
							//Outer Border
							delwin(subscrnMenuBorder);
							subscrnMenuBorder = newwin(MENU2_LENGTH + 4, MM_WIDTH + 2, startingRow, startingCol);
							wattron(subscrnMenuBorder, COLOR_PAIR(WHITE_BLACK));
							box(subscrnMenuBorder, '|', '_');
							wborder(subscrnMenuBorder, '|', '|', '-', '-', '*', '*', '*', '*');
							wrefresh(subscrnMenuBorder);
							//Menu
							delwin(subscrnMenu2);
							subscrnMenu2 = printMenu(menu2Items, startingLineColor, lineColors,
												MENU2_LENGTH, MM_WIDTH);
							highlight(subscrnMenu2, cursorPos, lineColors[cursorPos-1],
								startingLineColor, menu2Items, MENU2_LENGTH, MM_WIDTH);
						} */
					}
					else toggled = false;
					omp_set_lock(&subscrnGraphicLock);
					if(currMenu == 1 && cursorPos == 1) {
						if(graphicIterator == 0) paintGraphic(subscrnGraphic,
							"GRAPHICS/menuCubeLeft1_1.txt", seedColor, toggled);
						else if(graphicIterator == 1) paintGraphic(subscrnGraphic,
							"GRAPHICS/menuCubeRight1_1.txt", seedColor, toggled);
						else if(graphicIterator == 2) paintGraphic(subscrnGraphic,
							"GRAPHICS/menuCubeLeft2_1.txt", seedColor, toggled);
						else paintGraphic(subscrnGraphic,
							"GRAPHICS/menuCubeRight2_1.txt", seedColor, toggled);
					}
					else if(currMenu == 1 && cursorPos == 2) {
						if(graphicIterator == 0) paintGraphic(subscrnGraphic,
							"GRAPHICS/menuCubeLeft1_2.txt", seedColor, toggled);
						else if(graphicIterator == 1) paintGraphic(subscrnGraphic,
							"GRAPHICS/menuCubeRight1_2.txt", seedColor, toggled);
						else if(graphicIterator == 2) paintGraphic(subscrnGraphic,
							"GRAPHICS/menuCubeLeft2_2.txt", seedColor, toggled);
						else paintGraphic(subscrnGraphic,
							"GRAPHICS/menuCubeRight2_2.txt", seedColor, toggled);
					}
					else if(currMenu == 1 && cursorPos == 3) {
						paintGraphic(subscrnGraphic, "GRAPHICS/highScore.txt", seedColor, toggled);
					}
					else if(currMenu == 1 && cursorPos == 4) {
						if(toggled == true)
						paintGraphic(subscrnGraphic, "GRAPHICS/instructionsPic.txt", seedColor, toggled);
					}
					else if(currMenu == 1 && cursorPos == 5)
						if(toggled == true)
						paintGraphic(subscrnGraphic,
							"GRAPHICS/menuCubeLeft1_1.txt", seedColor, toggled);
					omp_unset_lock(&subscrnGraphicLock);
				}
			}

			//This section (thread) handles the cube animation for menu 2,
			//which moves one of two graphics (depending on player count)
			//across the screen from left-to-right, the speed of which
			//depends on the mode highlighted (faster = more difficult).
			#pragma omp section
			{
				double time1 = omp_get_wtime();
				int colOffset = 0;
				while(!gameOn) {
					usleep(25 * 1000);
					if(currMenu == 1) colOffset = 0;
					if(currMenu == 2){
						if(cursorPos == 1 && time1 + 0.25 < omp_get_wtime()) {
							time1 = omp_get_wtime();
							omp_set_lock(&subscrnGraphicLock);
							if(isTwoPlayer == false)
								subscrnGraphic =
									paintCubeGraphic(subscrnGraphic,
										"GRAPHICS/menuCubeRight1_1.txt", colOffset);
							else
								subscrnGraphic =
									paintCubeGraphic(subscrnGraphic,
										"GRAPHICS/menuCubeRight1_2.txt", colOffset);
							wrefresh(subscrnGraphic);
							omp_unset_lock(&subscrnGraphicLock);
							colOffset++;
						}
						else if(cursorPos == 2 && time1 + 0.1 < omp_get_wtime()) {
							time1 = omp_get_wtime();
							omp_set_lock(&subscrnGraphicLock);
							if(isTwoPlayer == false)
								subscrnGraphic =
									paintCubeGraphic(subscrnGraphic,
										"GRAPHICS/menuCubeRight1_1.txt", colOffset);
							else
								subscrnGraphic =
									paintCubeGraphic(subscrnGraphic,
										"GRAPHICS/menuCubeRight1_2.txt", colOffset);
							wrefresh(subscrnGraphic);
							omp_unset_lock(&subscrnGraphicLock);
							colOffset++;
						}
						else if(cursorPos == 3 && time1 + 0.05 < omp_get_wtime()) {
							time1 = omp_get_wtime();
							omp_set_lock(&subscrnGraphicLock);
							if(isTwoPlayer == false)
								subscrnGraphic =
									paintCubeGraphic(subscrnGraphic,
										"GRAPHICS/menuCubeRight1_1.txt", colOffset);
							else
								subscrnGraphic =
									paintCubeGraphic(subscrnGraphic,
										"GRAPHICS/menuCubeRight1_2.txt", colOffset);
							wrefresh(subscrnGraphic);
							omp_unset_lock(&subscrnGraphicLock);
							colOffset++;
						}
						if(colOffset == COLS - (COLS - MM_GRAPHIC_WIDTH)/2)
							colOffset = -((COLS - MM_GRAPHIC_WIDTH)/2) - MM_GRAPHIC_WIDTH;
					}
				}
			}

			//This section (thread) handles user input, including network
			//specifics (i.e. hostname and port number of Cube Runner server)
			//for multi-player functionality.
			#pragma omp section
			{
				while(!gameOn) {
					usleep(1000);
					int c = getch();
					omp_set_lock(&subscrnGraphicLock);
					if(c == KEY_DOWN || c == 'k') {
						if(cursorPos != EXIT && currMenu == 1) {
							cursorPos++;
							highlight(subscrnMenu1, cursorPos, lineColors[cursorPos-1],
								startingLineColor, menu1Items, MENU1_LENGTH, MM_WIDTH);
						}
						else if(cursorPos != BACK && currMenu == 2) {   //changed EXIT to BACK
							cursorPos++;
							highlight(subscrnMenu2, cursorPos, lineColors[cursorPos-1],
								startingLineColor, menu2Items, MENU2_LENGTH, MM_WIDTH); //changed MENU1_LENGTH to MENU2_LENGTH
						}
						else if(cursorPos == EXIT && currMenu == 1) {
							cursorPos = ONE_PLAYER;
							highlight(subscrnMenu1, cursorPos, lineColors[cursorPos-1],
								startingLineColor, menu1Items, MENU1_LENGTH, MM_WIDTH);
						}
						else if(cursorPos == BACK && currMenu == 2) {
							cursorPos = EASY;
							highlight(subscrnMenu2, cursorPos, lineColors[cursorPos-1],
								startingLineColor, menu2Items, MENU2_LENGTH, MM_WIDTH);
						}
						if(currMenu == 1 && cursorPos == TWO_PLAYER) isTwoPlayer = true;
						else if(currMenu == 1 && cursorPos == ONE_PLAYER) isTwoPlayer = false;

					}
					else if(c == KEY_UP || c == 'i') {
						if(cursorPos != ONE_PLAYER && currMenu == 1) {
							cursorPos--;
							highlight(subscrnMenu1, cursorPos, lineColors[cursorPos-1],
								startingLineColor, menu1Items, MENU1_LENGTH, MM_WIDTH);
						}
						else if(cursorPos != EASY && currMenu == 2) {
							cursorPos--;
							highlight(subscrnMenu2, cursorPos, lineColors[cursorPos-1],
								startingLineColor, menu2Items, MENU2_LENGTH, MM_WIDTH); //changed MENU1_LENGTH to MENU2_LENGTH
						}
						else if(cursorPos == ONE_PLAYER && currMenu == 1) {
							cursorPos = EXIT;
							highlight(subscrnMenu1, cursorPos, lineColors[cursorPos-1],
								startingLineColor, menu1Items, MENU1_LENGTH, MM_WIDTH);
						}
						else if(cursorPos == EASY && currMenu == 2) {
							cursorPos = BACK;
							highlight(subscrnMenu2, cursorPos, lineColors[cursorPos-1],
								startingLineColor, menu2Items, MENU2_LENGTH, MM_WIDTH);
						}
						if(currMenu == 1 && cursorPos == ONE_PLAYER) isTwoPlayer = false;
						else if(currMenu == 1 && cursorPos == TWO_PLAYER) isTwoPlayer = true;
					}
					else if(c == KEY_ENTER || c == 10 || c == 13) {
						if(currMenu == 1 && (cursorPos == ONE_PLAYER || cursorPos == TWO_PLAYER)) {
							delwin(subscrnMenu1);

							//Update Outer Border
							wclear(subscrnMenuBorder); wrefresh(subscrnMenuBorder); //Clear outer menu border
							subscrnMenuBorder = newwin(MENU2_LENGTH + 4, MM_WIDTH + 2, startingRow, startingCol);
							box(subscrnMenuBorder, '|', '_');
							wborder(subscrnMenuBorder, '|', '|', '-', '-', '*', '*', '*', '*');
							wrefresh(subscrnMenuBorder);

							//Something strange causing bottom line to go uncleared on some occasions...
							mvhline(startingRow + 8, 0, ' ', COLS); refresh();

							subscrnMenu2 = printMenu(menu2Items,
								startingLineColor, NULL, MENU2_LENGTH, MM_WIDTH);
							cursorPos = NORMAL;

							highlight(subscrnMenu2, cursorPos, lineColors[cursorPos-1],
								startingLineColor, menu2Items, MENU2_LENGTH, MM_WIDTH); //change MENU1_LENGTH to MENU2_LENGTH
							currMenu = 2;


						}
						else if(currMenu == 1 && cursorPos == HIGH_SCORE) {
							clear();

							displayScores(&subscrnGraphic);

							/////////////////REDRAW THE MENU/////////////////////
							clear();

							//Print Game Menu header
							attron(COLOR_PAIR(WHITE_BLACK));
							mvaddstr(startingRow - 1, startingCol + (MM_WIDTH - 7)/2, "Game Menu");
							refresh();

							//Update Outer Border
							wclear(subscrnMenuBorder); wrefresh(subscrnMenuBorder); //Clear outer menu border
							subscrnMenuBorder = newwin(MENU1_LENGTH + 4, MM_WIDTH + 2, startingRow, startingCol);  //**************
							box(subscrnMenuBorder, '|', '_');
							wborder(subscrnMenuBorder, '|', '|', '-', '-', '*', '*', '*', '*');
							wrefresh(subscrnMenuBorder);

							subscrnMenu1 = printMenu(menu1Items,
								startingLineColor, NULL, MENU1_LENGTH, MM_WIDTH);
							highlight(subscrnMenu1, HIGH_SCORE, lineColors[cursorPos-1],
								startingLineColor, menu1Items, MENU1_LENGTH, MM_WIDTH);

							paintGraphic(subscrnGraphic, "GRAPHICS/highScore.txt", 1, true);
						}
						else if(currMenu == 1 && cursorPos == INSTRUCTIONS) {
							/*fstream inFile;
							string inLine;
							inFile.open("../GRAPHICS/instructionsText.txt", ios::in);


							clear();
							move(0,0);
							if(inFile.is_open()){
								while(getline(inFile, inLine)){
									printw("FILE IS OPEN");
									//printw("%s", inLine);
									refresh();
								}
								inFile.close();
							}
							else{
								printw("FILE NOT OPEN");
							}*/
							clear();
							loadInstructions();
							//getch();

							/////////////////REDRAW THE MENU/////////////////////
							clear();



							//Print Game Menu header
							attron(COLOR_PAIR(WHITE_BLACK));
							mvaddstr(startingRow - 1, startingCol + (MM_WIDTH - 7)/2, "Game Menu");
							refresh();

							//Update Outer Border
							wclear(subscrnMenuBorder); wrefresh(subscrnMenuBorder); //Clear outer menu border
							subscrnMenuBorder = newwin(MENU1_LENGTH + 4, MM_WIDTH + 2, startingRow, startingCol);  //**************
							box(subscrnMenuBorder, '|', '_');
							wborder(subscrnMenuBorder, '|', '|', '-', '-', '*', '*', '*', '*');
							wrefresh(subscrnMenuBorder);

							subscrnMenu1 = printMenu(menu1Items,
								startingLineColor, NULL, MENU1_LENGTH, MM_WIDTH);
							highlight(subscrnMenu1, INSTRUCTIONS, lineColors[cursorPos-1],
								startingLineColor, menu1Items, MENU1_LENGTH, MM_WIDTH);
							paintGraphic(subscrnGraphic, "GRAPHICS/instructionsPic.txt", 1, true);

						}
						else if(currMenu == 1 && cursorPos == EXIT) {
							delwin(subscrnMenu1);
							// restore original settings and leave
							endwin();
							exit(0);
						}

						//Easy game...
						else if(currMenu == 2 && cursorPos == EASY && isTwoPlayer == false) {
							delwin(subscrnMenu2);
							wclear(subscrnMenuBorder); wrefresh(subscrnMenuBorder);
							subscrnMenu4 =
								userPrompt(startingColMenu4, startingRowMenu4,
									&subscrnGraphic, &currMenu, &escaped, username, isTwoPlayer);

							//return back to menu
							if(escaped) {
								//Clear and delete username prompt
								wclear(subscrnMenu4); wrefresh(subscrnMenu4); delwin(subscrnMenu4);

								//Reinstate outer menu border
								box(subscrnMenuBorder, '|', '_');
								wborder(subscrnMenuBorder, '|', '|', '-', '-', '*', '*', '*', '*');
								wrefresh(subscrnMenuBorder);
								highlight(subscrnMenu2, cursorPos, lineColors[cursorPos-1],
									startingLineColor, menu2Items, MENU2_LENGTH, MM_WIDTH);

								//Replace Game Menu header
								attron(COLOR_PAIR(BLACK_BLACK));
								mvhline(startingRow - 1, 0, ' ', COLS);
								attron(COLOR_PAIR(WHITE_BLACK));
								mvaddstr(startingRow - 1, startingCol + (MM_WIDTH - 7)/2, "Game Menu");
								refresh();

								//Restore menu variable
								currMenu = 2;
							}

							else {
								gameOn = true;
								gameMode = EASY;
							}

							escaped = false;
						}
						//Go to network prompt if multi-player mode is selected
						else if(currMenu == 2 && cursorPos == EASY && isTwoPlayer == true) {
							delwin(subscrnMenu2);
							wclear(subscrnMenuBorder); wrefresh(subscrnMenuBorder); //Clear outer menu border

							subscrnMenu3 =
								hostPrompt(startingColMenu3, startingRowMenu3,
									&subscrnGraphic, &currMenu, &escaped, host, port);

							//Return from network prompt if user escapes the menu
							if(escaped) {
								//Clear and delete host prompt menu
								wclear(subscrnMenu3); wrefresh(subscrnMenu3); delwin(subscrnMenu3);

								//Reinstate outer menu border
								box(subscrnMenuBorder, '|', '_');
								wborder(subscrnMenuBorder, '|', '|', '-', '-', '*', '*', '*', '*');
								wrefresh(subscrnMenuBorder);
								highlight(subscrnMenu2, cursorPos, lineColors[cursorPos-1],
									startingLineColor, menu2Items, MENU2_LENGTH, MM_WIDTH);

								//Replace Game Menu header
								attron(COLOR_PAIR(BLACK_BLACK));
								mvhline(startingRow - 1, 0, ' ', COLS);
								attron(COLOR_PAIR(WHITE_BLACK));
								mvaddstr(startingRow - 1, startingCol + (MM_WIDTH - 7)/2, "Game Menu");
								refresh();

								//Restore menu variable
								currMenu = 2;
							}

							else {
								//Clear and delete host prompt menu
								wclear(subscrnMenu3); wrefresh(subscrnMenu3); delwin(subscrnMenu3);
								clear();  // curses clear-screen call

								//Paint screen black
								attron(COLOR_PAIR(BLACK_BLACK));
								for (int y = 0; y < LINES; y++) {
									mvhline(y, 0, ' ', COLS);
								}
								refresh();

								subscrnMenu4 =
									userPrompt(startingColMenu4, startingRowMenu4,
										&subscrnGraphic, &currMenu, &escaped, username, isTwoPlayer);

								if(escaped)
								{
									wclear(subscrnMenu4); wrefresh(subscrnMenu4); delwin(subscrnMenu4);

									//Reinstate outer menu border
									box(subscrnMenuBorder, '|', '_');
									wborder(subscrnMenuBorder, '|', '|', '-', '-', '*', '*', '*', '*');
									wrefresh(subscrnMenuBorder);
									highlight(subscrnMenu2, cursorPos, lineColors[cursorPos-1],
										startingLineColor, menu2Items, MENU2_LENGTH, MM_WIDTH);

									//Replace Game Menu header
									attron(COLOR_PAIR(BLACK_BLACK));
									mvhline(startingRow - 1, 0, ' ', COLS);
									attron(COLOR_PAIR(WHITE_BLACK));
									mvaddstr(startingRow - 1, startingCol + (MM_WIDTH - 7)/2, "Game Menu");
									refresh();

									//Restore menu variable
									currMenu = 2;
								}

								else
								{
									gameMode = EASY;
									gameOn = true;
								}

								escaped = false;

							}

							escaped = false;
						}

						//Normal game...
						else if(currMenu == 2 && cursorPos == NORMAL && isTwoPlayer == false) {
							delwin(subscrnMenu2);
							wclear(subscrnMenuBorder); wrefresh(subscrnMenuBorder);

							subscrnMenu4 =
								userPrompt(startingColMenu4, startingRowMenu4,
									&subscrnGraphic, &currMenu, &escaped, username, isTwoPlayer);

							//return back to menu
							if(escaped) {
								//Clear and delete username prompt
								wclear(subscrnMenu4); wrefresh(subscrnMenu4); delwin(subscrnMenu4);

								//Reinstate outer menu border
								box(subscrnMenuBorder, '|', '_');
								wborder(subscrnMenuBorder, '|', '|', '-', '-', '*', '*', '*', '*');
								wrefresh(subscrnMenuBorder);
								highlight(subscrnMenu2, cursorPos, lineColors[cursorPos-1],
									startingLineColor, menu2Items, MENU2_LENGTH, MM_WIDTH);

								//Replace Game Menu header
								attron(COLOR_PAIR(BLACK_BLACK));
								mvhline(startingRow - 1, 0, ' ', COLS);
								attron(COLOR_PAIR(WHITE_BLACK));
								mvaddstr(startingRow - 1, startingCol + (MM_WIDTH - 7)/2, "Game Menu");
								refresh();

								//Restore menu variable
								currMenu = 2;
							}

							else {
								gameOn = true;
								gameMode = NORMAL;
							}

							escaped = false;
						}
						//Go to network prompt if multi-player mode is selected
						else if(currMenu == 2 && cursorPos == NORMAL && isTwoPlayer == true) {
							delwin(subscrnMenu2);
							wclear(subscrnMenuBorder); wrefresh(subscrnMenuBorder); //Clear outer menu border

							subscrnMenu3 =
								hostPrompt(startingColMenu3, startingRowMenu3,
									&subscrnGraphic, &currMenu, &escaped, host, port);

							//Return from network prompt if user escapes the menu
							if(escaped) {
								//Clear and delete host prompt menu
								wclear(subscrnMenu3); wrefresh(subscrnMenu3); delwin(subscrnMenu3);

								//Reinstate outer menu border
								box(subscrnMenuBorder, '|', '_');
								wborder(subscrnMenuBorder, '|', '|', '-', '-', '*', '*', '*', '*');
								wrefresh(subscrnMenuBorder);
								highlight(subscrnMenu2, cursorPos, lineColors[cursorPos-1],
									startingLineColor, menu2Items, MENU2_LENGTH, MM_WIDTH);

								//Replace Game Menu header
								attron(COLOR_PAIR(BLACK_BLACK));
								mvhline(startingRow - 1, 0, ' ', COLS);
								attron(COLOR_PAIR(WHITE_BLACK));
								mvaddstr(startingRow - 1, startingCol + (MM_WIDTH - 7)/2, "Game Menu");
								refresh();

								//Restore menu variable
								currMenu = 2;
							}
							else {
								//Clear and delete host prompt menu
								wclear(subscrnMenu3); wrefresh(subscrnMenu3); delwin(subscrnMenu3);
								clear();  // curses clear-screen call

								//Paint screen black
								attron(COLOR_PAIR(BLACK_BLACK));
								for (int y = 0; y < LINES; y++) {
									mvhline(y, 0, ' ', COLS);
								}
								refresh();

								subscrnMenu4 =
									userPrompt(startingColMenu4, startingRowMenu4,
										&subscrnGraphic, &currMenu, &escaped, username, isTwoPlayer);

								if(escaped)
								{
									wclear(subscrnMenu4); wrefresh(subscrnMenu4); delwin(subscrnMenu4);

									//Reinstate outer menu border
									box(subscrnMenuBorder, '|', '_');
									wborder(subscrnMenuBorder, '|', '|', '-', '-', '*', '*', '*', '*');
									wrefresh(subscrnMenuBorder);
									highlight(subscrnMenu2, cursorPos, lineColors[cursorPos-1],
										startingLineColor, menu2Items, MENU2_LENGTH, MM_WIDTH);

									//Replace Game Menu header
									attron(COLOR_PAIR(BLACK_BLACK));
									mvhline(startingRow - 1, 0, ' ', COLS);
									attron(COLOR_PAIR(WHITE_BLACK));
									mvaddstr(startingRow - 1, startingCol + (MM_WIDTH - 7)/2, "Game Menu");
									refresh();

									//Restore menu variable
									currMenu = 2;
								}

								else
								{
									gameMode = NORMAL;
									gameOn = true;
								}

								escaped = false;

							}
							escaped = false;
						}

						//Hard game...
						else if(currMenu == 2 && cursorPos == HARD && isTwoPlayer == false) {
							delwin(subscrnMenu2);
							wclear(subscrnMenuBorder); wrefresh(subscrnMenuBorder);

							subscrnMenu4 =
								userPrompt(startingColMenu4, startingRowMenu4,
									&subscrnGraphic, &currMenu, &escaped, username, isTwoPlayer);

							//return back to menu
							if(escaped) {
								//Clear and delete username prompt
								wclear(subscrnMenu4); wrefresh(subscrnMenu4); delwin(subscrnMenu4);

								//Reinstate outer menu border
								box(subscrnMenuBorder, '|', '_');
								wborder(subscrnMenuBorder, '|', '|', '-', '-', '*', '*', '*', '*');
								wrefresh(subscrnMenuBorder);
								highlight(subscrnMenu2, cursorPos, lineColors[cursorPos-1],
									startingLineColor, menu2Items, MENU2_LENGTH, MM_WIDTH);

								//Replace Game Menu header
								attron(COLOR_PAIR(BLACK_BLACK));
								mvhline(startingRow - 1, 0, ' ', COLS);
								attron(COLOR_PAIR(WHITE_BLACK));
								mvaddstr(startingRow - 1, startingCol + (MM_WIDTH - 7)/2, "Game Menu");
								refresh();

								//Restore menu variable
								currMenu = 2;
							}

							else {
								gameOn = true;
								gameMode = HARD;
							}

							escaped = false;
						}
						//Go to network prompt if multi-player mode is selected
						else if(currMenu == 2 && cursorPos == HARD && isTwoPlayer == true) {
							delwin(subscrnMenu2);
							wclear(subscrnMenuBorder); wrefresh(subscrnMenuBorder); //Clear outer menu border

							subscrnMenu3 =
								hostPrompt(startingColMenu3, startingRowMenu3,
									&subscrnGraphic, &currMenu, &escaped, host, port);

							//Return from network prompt if user escapes the menu
							if(escaped) {
								//Clear and delete host prompt menu
								wclear(subscrnMenu3); wrefresh(subscrnMenu3); delwin(subscrnMenu3);

								//Reinstate outer menu border
								box(subscrnMenuBorder, '|', '_');
								wborder(subscrnMenuBorder, '|', '|', '-', '-', '*', '*', '*', '*');
								wrefresh(subscrnMenuBorder);
								highlight(subscrnMenu2, cursorPos, lineColors[cursorPos-1],
									startingLineColor, menu2Items, MENU2_LENGTH, MM_WIDTH);

								//Replace Game Menu header
								attron(COLOR_PAIR(BLACK_BLACK));
								mvhline(startingRow - 1, 0, ' ', COLS);
								attron(COLOR_PAIR(WHITE_BLACK));
								mvaddstr(startingRow - 1, startingCol + (MM_WIDTH - 7)/2, "Game Menu");
								refresh();

								//Restore menu variable
								currMenu = 2;
							}
							else {
								//Clear and delete host prompt menu
								wclear(subscrnMenu3); wrefresh(subscrnMenu3); delwin(subscrnMenu3);
								clear();  // curses clear-screen call

								//Paint screen black
								attron(COLOR_PAIR(BLACK_BLACK));
								for (int y = 0; y < LINES; y++) {
									mvhline(y, 0, ' ', COLS);
								}
								refresh();

								subscrnMenu4 =
									userPrompt(startingColMenu4, startingRowMenu4,
										&subscrnGraphic, &currMenu, &escaped, username, isTwoPlayer);

								if(escaped)
								{
									wclear(subscrnMenu4); wrefresh(subscrnMenu4); delwin(subscrnMenu4);

									//Reinstate outer menu border
									box(subscrnMenuBorder, '|', '_');
									wborder(subscrnMenuBorder, '|', '|', '-', '-', '*', '*', '*', '*');
									wrefresh(subscrnMenuBorder);
									highlight(subscrnMenu2, cursorPos, lineColors[cursorPos-1],
										startingLineColor, menu2Items, MENU2_LENGTH, MM_WIDTH);

									//Replace Game Menu header
									attron(COLOR_PAIR(BLACK_BLACK));
									mvhline(startingRow - 1, 0, ' ', COLS);
									attron(COLOR_PAIR(WHITE_BLACK));
									mvaddstr(startingRow - 1, startingCol + (MM_WIDTH - 7)/2, "Game Menu");
									refresh();

									//Restore menu variable
									currMenu = 2;
								}

								else
								{
									gameMode = HARD;
									gameOn = true;
								}

								escaped = false;

							}
							escaped = false;
						}

						//Exit    // Back???
						else if(currMenu == 2 && (cursorPos == BACK || c == KEY_END || c == 27)) {
							delwin(subscrnMenu2);

							//Update Outer Border
							wclear(subscrnMenuBorder); wrefresh(subscrnMenuBorder); //Clear outer menu border
							subscrnMenuBorder = newwin(MENU1_LENGTH + 4, MM_WIDTH + 2, startingRow, startingCol);  //**************
							box(subscrnMenuBorder, '|', '_');
							wborder(subscrnMenuBorder, '|', '|', '-', '-', '*', '*', '*', '*');
							wrefresh(subscrnMenuBorder);
							highlight(subscrnMenu1, cursorPos, lineColors[cursorPos-1],
							startingLineColor, menu1Items, MENU1_LENGTH, MM_WIDTH); //change subscrnMenu2 to 1, menu2Items to 1


							subscrnMenu1 = printMenu(menu1Items, startingLineColor, NULL,
														MENU1_LENGTH, MM_WIDTH);
							subscrnGraphic = paintCubeGraphic(subscrnGraphic,
												"GRAPHICS/menuCubeRight1_1.txt");
							if(isTwoPlayer == false) cursorPos = ONE_PLAYER;
							else cursorPos = TWO_PLAYER;
							highlight(subscrnMenu1, cursorPos, lineColors[cursorPos-1],
								startingLineColor, menu1Items, MENU1_LENGTH, MM_WIDTH);
							currMenu = 1;

						}
					}
					//Go back to first menu if user presses "END" or "ESC"
					else if(currMenu == 2 && (c == KEY_END || c == 27)) {
							delwin(subscrnMenu2);

							//Update Outer Border
							wclear(subscrnMenuBorder); wrefresh(subscrnMenuBorder); //Clear outer menu border
							subscrnMenuBorder = newwin(MENU1_LENGTH + 4, MM_WIDTH + 2, startingRow, startingCol);  //**************
							box(subscrnMenuBorder, '|', '_');
							wborder(subscrnMenuBorder, '|', '|', '-', '-', '*', '*', '*', '*');
							wrefresh(subscrnMenuBorder);
							highlight(subscrnMenu2, cursorPos, lineColors[cursorPos-1],
							startingLineColor, menu2Items, MENU1_LENGTH, MM_WIDTH);


							subscrnMenu1 = printMenu(menu1Items, startingLineColor, NULL,
														MENU1_LENGTH, MM_WIDTH);
							subscrnGraphic = paintCubeGraphic(subscrnGraphic,
												"GRAPHICS/menuCubeRight1_1.txt");
							if(isTwoPlayer == false) cursorPos = ONE_PLAYER;
							else cursorPos = TWO_PLAYER;
							highlight(subscrnMenu1, cursorPos, lineColors[cursorPos-1],
								startingLineColor, menu1Items, MENU1_LENGTH, MM_WIDTH);
							currMenu = 1;
					}
					omp_unset_lock(&subscrnGraphicLock);
				}
			}
		}

		Game game = Game(gameMode, isTwoPlayer);
		//Initial transition animation
		transitionAnimation("GRAPHICS/Water.txt", 120, 16, BLUE_BLUE, 30, WHITE_BLUE);
		struct GameData gameinfo;

		if(isTwoPlayer == false) {
			gameinfo = game.playGame(NULL, NULL, username);

      if (gameinfo.earlyTerm == false)
			{
				addScoreSingle(gameinfo.finalScore, gameinfo.firstName,
					gameinfo.hours, gameinfo.minutes, gameinfo.seconds, gameMode);
			}
		}
		else {
			gameinfo = game.playGame(host, port, username);

			//only save high scores from first player to prevent duplicates
			if (gameinfo.playerNum == 1 && gameinfo.earlyTerm == false)
			{
				addScoreMulti(gameinfo.finalScore, gameinfo.firstName,
					gameinfo.secondName, gameinfo.hours, gameinfo.minutes,
					gameinfo.seconds, gameinfo.gameMode);
			}
		}
		//Reinitialize Variables for Next Game Loop
		cursorPos = 1; currMenu = 1; gameMode = -1;
		gameOn = false; escaped = false; isTwoPlayer = false;
	}
	return 0;
	endwin();
}

// References
// http://heather.cs.ucdavis.edu/~matloff/UnixAndC/CLanguage/Curses.pdf
// http://www.cplusplus.com/reference/string/string/getline/
// https://stackoverflow.com/questions/8460666/c-does-getline-have-a-maximum-string-or-character-capacity
// http://www.cplusplus.com/reference/string/string/substr/
// https://stackoverflow.com/questions/347949/how-to-convert-a-stdstring-to-const-char-or-char
// https://www.linuxjournal.com/content/programming-color-ncurses
// https://www.linuxjournal.com/content/about-ncurses-colors-0
// https://www.youtube.com/watch?v=pjT5wq11ZSE
// https://stackoverflow.com/questions/18837836/how-can-i-hide-the-cursor-in-ncurses
// https://invisible-island.net/ncurses/howto/NCURSES-Programming-HOWTO.html
// https://stackoverflow.com/questions/6281368/multi-line-define-directives
// https://stackoverflow.com/questions/21173308/how-to-best-initialize-a-vector-of-strings-in-c
// https://stackoverflow.com/questions/1182665/curses-getting-arrow-keys
// https://stackoverflow.com/questions/11067800/ncurses-key-enter-is-fail
// https://stackoverflow.com/questions/2545720/error-default-argument-given-for-parameter-1
// https://stackoverflow.com/questions/5288036/how-to-clear-ostringstream
// https://www.gnu.org/software/guile-ncurses/manual/html_node/Getting-characters-from-the-keyboard.html
// https://stackoverflow.com/questions/4114095/how-to-revert-a-git-repository-to-a-previous-commit
