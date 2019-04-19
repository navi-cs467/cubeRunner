// psax.c; illustration of curses library

// runs the shell command 'ps ax' and dislays the last lines of its output,
// as many as the window will fit; allows the user to move up and down
// within the window, with the option to kill whichever process is
// currently highlighted

// usage:  psax

// user commands:

//    'u':  move highlight up a line
//    'd':  move highlight down a line
//    'k':  kill process in currently highlighted line
//    'r':  re-run 'ps ax' for update
//    'q':  quit

// possible extensions:  allowing scrolling, so that the user could go
// through all the 'ps ax' output, not just the last lines; allow
// wraparound for long lines; ask user to confirm before killing a
// process

#define MAXROW 1000
#define MAXCOL 500

#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "initColor.h"
using std::string;
using std::fstream;
using std::ostringstream;
using std::vector;
using std::ios;
using std::cout;
using std::endl;


#define MIN_WIN_HEIGHT 32
#define MIN_WIN_WIDTH 135
#define INTRO_HEIGHT 28
#define INTRO_WIDTH 120
#define MM_CUBE_GRAPHIC_HEIGHT 9			//MM stands for Main Menu
#define MM_CUBE_GRAPHIC_WIDTH 40
#define MM_HEIGHT 25
#define MM_WIDTH 25
#define MAIN_BORDER_ANIMATION_ROW_WIDTH 2
#define MAIN_BORDER_ANIMATION_COL_WIDTH 6
#define MAX_MENU_ITEMS 4
#define MENU1_LENGTH 4
#define MENU2_LENGTH 4

// #define RED     "\033[31m"      /* Red */
//#define RESET   "\033[0m" */

WINDOW *scrn; 	//Will point to curses window object

vector<string> cmdoutlines;  			//Screen output vector
vector<string> cmdoutlinesGraphics;		//Reserved for graphics only (multi-threaded)

const char* menu1[] = {"1 Player",
					   "2 Player",
					   "High Scores",
					   "Exit"};
				  
const char* menu2[] = {"Easy",
					   "Normal",
					   "Hard",
					   "Back"};
					   
vector<string> loadMenuVec(const char** menu, int length) {
	vector<string> retVec;
	for(int i = 0; i < length; i++) 
		retVec.push_back(menu[i]);
	return retVec;
}
	
const vector<string> menu1Items = loadMenuVec(menu1, MENU1_LENGTH);
const vector<string> menu2Items = loadMenuVec(menu2, MENU2_LENGTH);

int ncmdlines,  //Number of rows in cmdoutlines
    nwinlines,  //Number of rows our "ps ax" output occupies in the
                //xterm (or equiv.) window
    winrow,  	//Current row position in screen
    cmdstartrow,  //Index of first row in cmdoutlines to be displayed
    cmdlastrow;   //Index of last row in cmdoutlines to be displayed

//Rewrites the line at winrow in a white background
void highlight(WINDOW *subscrn, int cursorPos, int color)
{  
   int highlightColor;
   if(color == RED_BLACK) highlightColor = RED_WHITE;
   if(color == YELLOW_BLACK) highlightColor = YELLOW_WHITE;
   if(color == GREEN_BLACK) highlightColor = GREEN_WHITE;
   if(color == CYAN_BLACK) highlightColor = CYAN_WHITE;
   if(color == BLUE_BLACK) highlightColor = BLUE_WHITE;
   if(color == MAGENTA_BLACK) highlightColor = MAGENTA_WHITE;
   
   //Highlight line
   wattron(subscrn, COLOR_PAIR(WHITE_RED));
   mvwaddstr(subscrn, cursorPos, 
		(MM_WIDTH - cmdoutlines[cursorPos - 1].length())/2, 
		cmdoutlines[cursorPos - 1].c_str());
   wrefresh(subscrn);  // make the change appear on the screen
}

//Rewrites the line at winrow in a black background
void unhighlight(WINDOW *subscrn, int cursorPos, int color)
{  
   int highlightColor;
   if(color == RED_WHITE) highlightColor = RED_BLACK;
   if(color == YELLOW_WHITE) highlightColor = YELLOW_BLACK;
   if(color == GREEN_WHITE) highlightColor = GREEN_BLACK;
   if(color == CYAN_WHITE) highlightColor = CYAN_BLACK;
   if(color == BLUE_WHITE) highlightColor = BLUE_BLACK;
   if(color == MAGENTA_WHITE) highlightColor = MAGENTA_BLACK;
   
   //Highlight line
   wattron(subscrn, COLOR_PAIR(RED_WHITE));
   mvwaddstr(subscrn, cursorPos, 
		(MM_WIDTH - cmdoutlines[cursorPos - 1].length())/2, 
		cmdoutlines[cursorPos - 1].c_str());
   wrefresh(subscrn);  // make the change appear on the screen
}


//Loads intro graphic in cmdoutlinesGraphics
void loadGraphic(const char* fileName)
{  
   //Clear cmdoutlines
   cmdoutlinesGraphics.clear();
   
   fstream f; string line;
   f.open(fileName, ios::in);  //Open intro graphic file
   for (int row = 0; row < INTRO_HEIGHT; row++) {
      if (getline(f, line))	{	  //If not end of file, add line to cmdoutlines
		  //Don't want stored line to exceed width of screen, which the
		  //Curses library provides to us in the variable COLS, so truncate
		  //to at most COLS characters
		  if(line.size() > COLS) line = line.substr(0, COLS);
		  cmdoutlinesGraphics.push_back(line);
	  }
	  else break;
   }
   f.close();  //Close file
}

//Displays last part of command output (as much as fits in screen)
void intro()
{ 
	clear();  //Curses clear-screen call
	
	// paint screen black
	attron(COLOR_PAIR(BLACK_BLACK));
    for (int y = 0; y < LINES; y++) {
        mvhline(y, 0, ' ', COLS);
    }
	refresh();
   
    //Turn on bold attribute
    attron(A_BOLD);
	
	int startingCol = (COLS - INTRO_WIDTH)/2, 
		startingRow = (LINES - INTRO_HEIGHT)/2;
	WINDOW *subscrn = newwin(INTRO_HEIGHT, INTRO_WIDTH, 
							 startingRow, startingCol);
   
    //Load intro graphic into cmdoutlinesGraphics from file
    loadGraphic("cubeRunner.txt");
   
    //Paint the rows to the screen
    int color, row;
    for (row = 0, color = rand() % 6 + 1; 
			row < INTRO_HEIGHT - 2; row++, color++) {  
       if(color == 7) color = 1;		//Cycle to first index when necessary
	   //Change color
	   wattron(subscrn, COLOR_PAIR(color)); 
	   mvwaddstr(subscrn, row, 0,
		 cmdoutlinesGraphics[row].c_str());  //Curses call to move to the
									 //specified position and
									 //paint a string there
		usleep(1 * 1000);	//Sleep for 100 milliseconds for animation effect
		wrefresh(subscrn);  //Now make the changes actually appear on the screen,
							//using this call to the curses library
    }
	
	wattron(subscrn, COLOR_PAIR(BLACK_BLACK));		//Ensures these two lines are painted 
	mvwhline(subscrn, row, 0, ' ', INTRO_WIDTH);	//correctly before threading below
	mvwhline(subscrn, row + 1, 0, ' ', INTRO_WIDTH);	
		
	usleep(200 * 1000);	//Sleep for 200 milliseconds
	
	//Print blinking continue prompt
	//attron(A_BLINK);	//Doesn't work, (at least for some terminals)
	bool inputReceived = false;
	omp_set_num_threads(2); double time = omp_get_wtime(); bool visible = false;
	#pragma omp parallel sections shared(subscrn, inputReceived, \
	winrow, row, COLS, cmdoutlinesGraphics)
	{
		#pragma omp section
		{
			while(!inputReceived) {
				if(time + 0.6 < omp_get_wtime() && visible == false){
					time = omp_get_wtime();
					wattron(subscrn, COLOR_PAIR(RED_BLACK));
					mvwaddstr(subscrn, row + 1, 0, 
						cmdoutlinesGraphics[row + 1].c_str());
					wrefresh(subscrn);
					visible = true;
				}
				else if(time + 1.75 < omp_get_wtime() && visible == true){
					time = omp_get_wtime();
					wattron(subscrn, COLOR_PAIR(BLACK_BLACK));
					mvwhline(subscrn, row + 1, 0, ' ', INTRO_WIDTH);
					wrefresh(subscrn);
					visible = false;
				}
			}
		}
		
		#pragma omp section
		{
			if(!inputReceived) getch();
			inputReceived = true;
		}
	}
	
	//Paint in alternating color perimeters converging into middle of screen
	for(int i = LINES, j = COLS, color = rand() % 6 + 9, z = 0; 
		i >= LINES/2 && j >= COLS/2; 
		i -= MAIN_BORDER_ANIMATION_ROW_WIDTH, j -= MAIN_BORDER_ANIMATION_COL_WIDTH, color++, z++){
		if(color == 15) color = 9;	//Reset color when necessary
		attron(COLOR_PAIR(color));
		for(int k = LINES - i, l = 0; l < MAIN_BORDER_ANIMATION_ROW_WIDTH; l++) 
			mvhline(k + l, COLS - j, ' ', j - (z * MAIN_BORDER_ANIMATION_COL_WIDTH));
		for(int k = i - 1, l = 0; l < MAIN_BORDER_ANIMATION_ROW_WIDTH; l++) 
			mvhline(k - l, COLS - j, ' ', j - (z * MAIN_BORDER_ANIMATION_COL_WIDTH));
		for(int k = COLS - j, l = 0; l < MAIN_BORDER_ANIMATION_COL_WIDTH; l++) 
			mvvline(LINES - i, k + l, ' ', i - (z * MAIN_BORDER_ANIMATION_ROW_WIDTH));
		for(int l = 0; l < MAIN_BORDER_ANIMATION_COL_WIDTH; l++) 
			mvvline(LINES - i, j - l, ' ', i - (z * MAIN_BORDER_ANIMATION_ROW_WIDTH));
		
		usleep(1 * 1000);	//Sleep for 100 milliseconds for animation effect
		refresh();
	}
	
	
    
	//attron(COLOR_PAIR(0));	//Reset to default
    //attroff(A_BOLD);
	
	delwin(subscrn);
}

/* //Moves up/down one line
void updown(int inc)
{  int tmp = winrow + inc;
   // ignore attempts to go off the edge of the screen
   if (tmp >= 0 && tmp < LINES) {
      // rewrite the current line before moving; since our current font
      // is non-BOLD (actually A_NORMAL), the effect is to unhighlight
      // this line
      mvaddstr(winrow,0,cmdoutlines[cmdstartrow+winrow].c_str());
      // highlight the line we're moving to
      winrow = tmp;
      highlight();
   }
} */

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

void paintCube(WINDOW *subscrn, const char* fileName, int seedColor) {
	
	loadGraphic(fileName);
	
	/* //Clear contents of sub-window
	wattron(subscrn, COLOR_PAIR(BLACK_BLACK));
	for (int y = 0; y < MM_CUBE_GRAPHIC_HEIGHT; y++)
		mvwhline(subscrn, y, 0, ' ', MM_CUBE_GRAPHIC_WIDTH);
	wrefresh(subscrn); */
	
	//Paint the rows to the screen
    int color, row;
    for (row = 0, color = seedColor; 
			row < MM_CUBE_GRAPHIC_HEIGHT; row++, color++) {  
       if(color == 7) color = 1;		//Cycle to first index when necessary
	   //Change color
	   wattron(subscrn, COLOR_PAIR(color)); 
	   mvwaddstr(subscrn, row, 0,
		 cmdoutlinesGraphics[row].c_str());							 				
		//usleep(100 * 1000);	
	   wrefresh(subscrn);  
							
    }	
	
}

WINDOW *printMenu(vector<string> menuItems, int seedColor, int lineColors[]) { 
	
	//Setup subscreen for menu
	int startingCol = (COLS - MM_CUBE_GRAPHIC_WIDTH)/2 + 
						(MM_CUBE_GRAPHIC_WIDTH - MM_WIDTH)/2 + 1, 
		startingRow = ((LINES - MM_CUBE_GRAPHIC_HEIGHT)/4) + MM_CUBE_GRAPHIC_HEIGHT + 5 + 1;	//Starts 5 rows below	
	WINDOW *subscrnMenu = newwin(MENU1_LENGTH + 2, MM_WIDTH, startingRow, startingCol);				//cube graphic, + 1
	wattron(subscrnMenu, COLOR_PAIR(WHITE_BLACK));												//due to outer border
	box(subscrnMenu, '|', '_'); 
	wborder(subscrnMenu, '|', '|', '-', '-', '*', '*', '*', '*');
	wrefresh(subscrnMenu);
	
	//Clear cmdoutlines so menu items can be stored
	//(Stored menu item strings and line colors are needed for the highlight function.)
	cmdoutlines.clear();
	
	//Print and store menu line items and line colors...
	int i = 1;
	for(vector<string>::iterator it = menuItems.begin(); it != menuItems.end(); it++) {
		if(seedColor == BLUE_BLACK) seedColor++;	//No blue, too hard to see against black
		if(lineColors) lineColors[i - 1] = seedColor;
		wattron(subscrnMenu, COLOR_PAIR(seedColor++));
		mvwprintw(subscrnMenu, i++, (MM_WIDTH - it->length())/2, it->c_str());
		cmdoutlines.push_back(*it);		//Stores current menu items in cmdoutlines
		wrefresh(subscrnMenu);
		if(seedColor == 7) seedColor = 1;
	}
	
	//mvaddstr(6, 6, (cmdoutlines[0]+cmdoutlines[1]).c_str());
	
	/* //Line 1
	if(lineColors) lineColors[0] = seedColor;
	wattron(subscrnMenu1, COLOR_PAIR(seedColor++));
	mvwprintw(subscrnMenu1, 1, 5, "1 Player");
	cmdoutlines.push_back("1 Player");
	wrefresh(subscrnMenu1);
	if(seedColor == 7) seedColor = 1;
	
	//Line 2
	if(lineColors) lineColors[1] = seedColor;
	wattron(subscrnMenu1, COLOR_PAIR(seedColor++));
	mvwprintw(subscrn, 2, 5, "2 Player");
	cmdoutlines.push_back("2 Player");
	wrefresh(subscrnMenu1);
	if(seedColor == 7) seedColor = 1;
	
	//Line 3
	if(lineColors) lineColors[2] = seedColor;
	wattron(subscrnMenu1, COLOR_PAIR(seedColor++));
	mvwprintw(subscrn, 3, 5, "High Scores");
	cmdoutlines.push_back("High Scores");
	wrefresh(subscrnMenu1);
	if(seedColor == 7) seedColor = 1;
	
	//Line 4
	if(lineColors) lineColors[3] = seedColor;
	mvwprintw(subscrn, 4, 5, "Exit");
	wattron(subscrnMenu1, COLOR_PAIR(seedColor++));
	cmdoutlines.push_back("Exit");
	wrefresh(subscrnMenu1);
	if(seedColor == 7) seedColor = 1;			//In case more items are added later
	 */
	return subscrnMenu;
	
}

int main(void)
{  
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
	
	initColor();	//Initialize color indexes (see initColor.h)
	
	validateWinSize();
	
	srand(time(NULL));	//Seed random number generator with system time
    
    //Run intro animation
    intro();
	
	clear();  // curses clear-screen call
	
	//Paint screen black
	attron(COLOR_PAIR(BLACK_BLACK));
    for (int y = 0; y < LINES; y++) {
        mvhline(y, 0, ' ', COLS);
    }
	refresh();
	
	//Setup subscreen for cube graphic
	int startingCol = (COLS - MM_CUBE_GRAPHIC_WIDTH)/2,
		startingRow = (LINES - MM_CUBE_GRAPHIC_HEIGHT)/4;		//Graphic starts one-quarter of the way down the screen
	WINDOW *subscrnGraphic = newwin(MM_CUBE_GRAPHIC_HEIGHT, MM_CUBE_GRAPHIC_WIDTH, 
							 startingRow, startingCol);
	//Random starting color for cube graphic
	wattron(subscrnGraphic, COLOR_PAIR(rand() % 6 + 15));
	
	//Print initial cube graphic
	paintCube(subscrnGraphic, "menuCubeRight1_1.txt", rand()%6+1);
	
	//Setup subscreen for menu outer border
	startingCol = startingCol + (MM_CUBE_GRAPHIC_WIDTH - MM_WIDTH) / 2, 
	startingRow = ((LINES - MM_CUBE_GRAPHIC_HEIGHT)/4) + MM_CUBE_GRAPHIC_HEIGHT + 5;		//Menu starts 5 lines below the graphic
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
	WINDOW *subscrnMenu1 = printMenu(menu1Items, startingLineColor, lineColors);
	
	//Declare menu 2 for future use below
	WINDOW *subscrnMenu2;
	
	//Start highlighting at line 1
	int cursorPos = 1, currMenu = 1, playerCount = 1;
	highlight(subscrnMenu1, cursorPos, lineColors[cursorPos]);
							 
	bool gameOn = false; double time = omp_get_wtime(); 
	omp_set_num_threads(2); 
	#pragma omp parallel sections shared(subscrnMenu1, subscrnMenu2, \
	subscrnGraphic, cursorPos, cmdoutlines, lineColors, currMenu, \
	playerCount, startingLineColor)
	{
		#pragma omp section
		{
			int graphicIterator = 0, seedColor = rand() % 6 + 1;
			while(!gameOn) {
				if(time + 1 < omp_get_wtime()) {
					time = omp_get_wtime();
					graphicIterator++;
					if(graphicIterator == 4) graphicIterator = 0;
					seedColor++;
					if(seedColor == 7) seedColor = 1;
				}
				if(cursorPos == 1 || cursorPos == 4) {
					if(graphicIterator == 0) paintCube(subscrnGraphic, 
						"menuCubeLeft1_1.txt", seedColor);
					else if(graphicIterator == 1) paintCube(subscrnGraphic, 
						"menuCubeRight1_1.txt", seedColor);
					else if(graphicIterator == 2) paintCube(subscrnGraphic, 
						"menuCubeLeft2_1.txt", seedColor);
					else paintCube(subscrnGraphic, 
						"menuCubeRight2_1.text", seedColor);
				}
				else if(cursorPos == 2) {
					if(graphicIterator == 0) paintCube(subscrnGraphic, 
						"menuCubeLeft1_2.txt", seedColor);
					else if(graphicIterator == 1) paintCube(subscrnGraphic, 
						"menuCubeRight1_2.txt", seedColor);
					else if(graphicIterator == 2) paintCube(subscrnGraphic, 
						"menuCubeLeft2_2.txt", seedColor);
					else paintCube(subscrnGraphic, 
						"menuCubeRight2_2.text", seedColor);
				}
				else if(cursorPos == 3 && currMenu == 1) {
					if(graphicIterator == 0) paintCube(subscrnGraphic, 
						"menuCubeLeft1_1.txt", seedColor);
					else if(graphicIterator == 1) paintCube(subscrnGraphic, 
						"menuCubeRight1_1.txt", seedColor);
					else if(graphicIterator == 2) paintCube(subscrnGraphic, 
						"menuCubeLeft2_1.txt", seedColor);
					else paintCube(subscrnGraphic, 
						"menuCubeRight2_1.txt", seedColor);
				}
			}
		}
		
		#pragma omp section
		{
			while(1) {
				char c = getch();
				if(c == KEY_DOWN || c == 'k') { ostringstream ostr; ostr << lineColors[0];
					if(cursorPos != 4 && currMenu == 1) { mvaddstr(6, 6, ostr.str().c_str()); refresh();
						//unhighlight(subscrnMenu1, cursorPos, lineColors[cursorPos-1]);
						cursorPos++;
						highlight(subscrnMenu1, cursorPos, lineColors[cursorPos-1]);
					}
					else if(cursorPos != 4 && currMenu == 2) {
						unhighlight(subscrnMenu2, cursorPos, lineColors[cursorPos-1]);
						cursorPos++;
						highlight(subscrnMenu2, cursorPos, lineColors[cursorPos-1]);
					if(currMenu == 1 && cursorPos == 2) playerCount = 2;
					
				}
				else if(c == KEY_UP || c == 'i') {
					if(cursorPos != 1 && currMenu == 1) {
						unhighlight(subscrnMenu1, cursorPos, lineColors[cursorPos-1]);
						cursorPos--;
						highlight(subscrnMenu1, cursorPos, lineColors[cursorPos-1]);
					}
					else if(cursorPos != 1 && currMenu == 2) {
						unhighlight(subscrnMenu2, cursorPos, lineColors[cursorPos-1]);
						cursorPos--;
						highlight(subscrnMenu2, cursorPos, lineColors[cursorPos-1]);
					}
					if(currMenu == 1 && cursorPos == 1) playerCount = 1;
					else if(currMenu == 1 && cursorPos == 2) playerCount = 2;
				}
				else if(c == KEY_ENTER) {
						if(currMenu == 1 && (cursorPos == 1 || cursorPos == 2)) {
							delwin(subscrnMenu1);
							subscrnMenu2 = printMenu(menu2Items, startingLineColor, NULL);
							cursorPos = 2;
							highlight(subscrnMenu2, cursorPos, lineColors[cursorPos-1]);
							currMenu = 2;
						}
						else if(currMenu == 1 && cursorPos == 3) {
							delwin(subscrnMenu1);
							//showHighScores();
							break;	// Temporary
						}
						else if(currMenu == 1 && cursorPos == 4) {
							delwin(subscrnMenu1);
							// restore original settings and leave
							endwin();
							exit(0);
						}
						else if(currMenu == 2 && cursorPos == 1) {
							delwin(subscrnMenu2);
							//startGame(easy, playerCount);
						}
						else if(currMenu == 2 && cursorPos == 2) {
							delwin(subscrnMenu2);
							//startGame(normal, playerCount);
						}
						else if(currMenu == 2 && cursorPos == 3) {
							delwin(subscrnMenu2);
							//startGame(hard, playerCount);
						}
						else if(currMenu == 2 && cursorPos == 4) {
							delwin(subscrnMenu2);
							subscrnMenu1 = printMenu(menu1Items, startingLineColor, NULL);
							cursorPos = 1;
							highlight(subscrnMenu1, cursorPos, lineColors[cursorPos-1]);
							currMenu = 1;
						}
					}
				}
			}
		}
	}
	
    return 0;
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
