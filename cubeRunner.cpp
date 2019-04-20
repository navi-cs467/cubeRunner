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
using std::istringstream;
using std::vector;
using std::ios;
using std::cout;
using std::cin;
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
#define MENU3_LENGTH 4

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
					   
const char* menu3[] = {"Hostname or IP Address: ",
						"Port Number: ",
						"",
						"   *Press Esc To Go Back*"};
					   
vector<string> loadMenuVec(const char** menu, int length) {
	vector<string> retVec;
	for(int i = 0; i < length; i++) 
		retVec.push_back(menu[i]);
	return retVec;
}
	
const vector<string> menu1Items = loadMenuVec(menu1, MENU1_LENGTH);
const vector<string> menu2Items = loadMenuVec(menu2, MENU2_LENGTH);
const vector<string> menu3Items = loadMenuVec(menu3, MENU3_LENGTH);

string networkPrompt = "Please enter a Hostname or IP Address "
						  "and port number to connect to a Cube Runner "
						  "multiplayer server";

int ncmdlines,  //Number of rows in cmdoutlines
    nwinlines,  //Number of rows our "ps ax" output occupies in the
                //xterm (or equiv.) window
    winrow,  	//Current row position in screen
    cmdstartrow,  //Index of first row in cmdoutlines to be displayed
    cmdlastrow;   //Index of last row in cmdoutlines to be displayed

WINDOW *printMenu(vector<string> menuItems, int seedColor, int lineColors[],
						int menuLength, int menuWidth, 
						int altStartingCol = 0, int altStartingRow = 0);
	
//Rewrites the line at winrow in a white background
void highlight(WINDOW *subscrn, int cursorPos, int color, int startingLineColor,
				const vector<string> &menuItems, int menuLength, int menuWidth)
{  //ostringstream ostr; ostr << color << " " << cursorPos; mvaddstr(8, 8, ostr.str().c_str()); refresh();
   int highlightColor, barColor;
   if(color == RED_BLACK) {
	   highlightColor = BLACK_RED;
	   barColor = RED_RED;
   }
   if(color == YELLOW_BLACK) {
	   highlightColor = BLACK_YELLOW;
	   barColor = YELLOW_YELLOW;
   }
   if(color == GREEN_BLACK) {
	   highlightColor = BLACK_GREEN;
	   barColor = GREEN_GREEN;
   }
   if(color == CYAN_BLACK) {
	   highlightColor = BLACK_CYAN;
	   barColor = CYAN_CYAN;
   }
   if(color == BLUE_BLACK) {
	   highlightColor = BLACK_BLUE;
	   barColor = BLUE_BLUE;
   }
   if(color == MAGENTA_BLACK) {
	   highlightColor = BLACK_MAGENTA;
	   barColor = MAGENTA_MAGENTA;
   }
   
   /* //Highlight line
   wattron(subscrn, COLOR_PAIR(barColor));
   //int y, x; getmaxyx(subscrn, y, x);
   mvwhline(subscrn, cursorPos, 1, ' ', MM_WIDTH - 2);
   wattron(subscrn, COLOR_PAIR(highlightColor));
   mvwaddstr(subscrn, cursorPos, 
		(MM_WIDTH - cmdoutlines[cursorPos - 1].length())/2, 
		cmdoutlines[cursorPos - 1].c_str());
   wrefresh(subscrn);  // make the change appear on the screen */
   
   delwin(subscrn);
   
   subscrn = printMenu(menuItems, startingLineColor, NULL, 
						menuLength, menuWidth);
   
   //Highlight line
   wattron(subscrn, COLOR_PAIR(barColor));
   //int y, x; getmaxyx(subscrn, y, x);
   mvwhline(subscrn, cursorPos, 1, ' ', MM_WIDTH - 2);
   wattron(subscrn, COLOR_PAIR(highlightColor));
   mvwaddstr(subscrn, cursorPos, 
		(MM_WIDTH - cmdoutlines[cursorPos - 1].length())/2, 
		cmdoutlines[cursorPos - 1].c_str());
   wrefresh(subscrn);  // make the change appear on the screen */
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

void paintGraphic(WINDOW *subscrn, const char* fileName, int seedColor, bool toggled) {
	
	loadGraphic(fileName);
	
	if(toggled) {
		//Clear contents of sub-window
		wattron(subscrn, COLOR_PAIR(BLACK_BLACK));
		for (int y = 0; y < MM_CUBE_GRAPHIC_HEIGHT; y++)
			mvwhline(subscrn, y, 0, ' ', MM_CUBE_GRAPHIC_WIDTH);
		wrefresh(subscrn);
	}
	
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

WINDOW *printMenu(vector<string> menuItems, int seedColor, 
					int lineColors[], int menuLength, int menuWidth, 
					int altStartingCol, int altStartingRow) { 
	
	int startingCol, startingRow;
	//Setup subscreen for menu
	if(!altStartingCol) startingCol = (COLS - MM_CUBE_GRAPHIC_WIDTH)/2 + 
						(MM_CUBE_GRAPHIC_WIDTH - MM_WIDTH)/2 + 1;
	else startingCol = altStartingCol;						
	if(!altStartingRow) startingRow = ((LINES - MM_CUBE_GRAPHIC_HEIGHT)/4) + 
							MM_CUBE_GRAPHIC_HEIGHT + 5 + 1;	//By default tarts 5 rows below cube graphic, + 1 due to outer border
	else startingRow = altStartingRow;
	WINDOW *subscrnMenu = newwin(menuLength + 2, menuWidth, startingRow, startingCol);			
	wattron(subscrnMenu, COLOR_PAIR(WHITE_BLACK));							
	if(seedColor != -1) {
		box(subscrnMenu, '|', '_'); 
		wborder(subscrnMenu, '|', '|', '-', '-', '*', '*', '*', '*');
	}
	wrefresh(subscrnMenu);
	
	//Clear cmdoutlines so menu items can be stored
	//(Stored menu item strings and line colors are needed for the highlight function.)
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
			mvwprintw(subscrnMenu, i, (menuWidth - it->length())/2, it->c_str());	//centered for menus 1 & 2
		else
			mvwprintw(subscrnMenu, i, 0, it->c_str());	//left - justified for menu 3
		cmdoutlines.push_back(*it);		//Stores current menu items in cmdoutlines
		wrefresh(subscrnMenu);
		if(seedColor != -1) seedColor++;
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

WINDOW* paintCubeGraphic(WINDOW *subscrnGraphic, const char* fileName, int offset = 0) {
	//Clear subscrnGraphic and delete, if not NULL
	if(subscrnGraphic){
		wattron(subscrnGraphic, COLOR_PAIR(BLACK_BLACK));
		for(int row = 0; row < MM_CUBE_GRAPHIC_HEIGHT; row++)
			mvwhline(subscrnGraphic, row, 0, ' ', MM_CUBE_GRAPHIC_WIDTH);
		wrefresh(subscrnGraphic);
		delwin(subscrnGraphic);
	}
	
	//Setup subscreen for cube graphic
	int startingColCG = ((COLS - MM_CUBE_GRAPHIC_WIDTH)/2) + offset,
		startingRowCG = (LINES - MM_CUBE_GRAPHIC_HEIGHT)/4;		//Graphic starts one-quarter of the way down the screen
	WINDOW *subscrnGraphicNEW = newwin(MM_CUBE_GRAPHIC_HEIGHT, MM_CUBE_GRAPHIC_WIDTH, 
							 startingRowCG, startingColCG);
	
	//Random starting color for cube graphic
	//wattron(subscrnGraphic, COLOR_PAIR(rand() % 6 + 15));
	
	//Paint initial cube graphic
	paintGraphic(subscrnGraphicNEW, fileName, rand()%6+1, false);
	
	return subscrnGraphicNEW;
}

WINDOW *hostPrompt(int startingColMenu3, int startingRowMenu3, 
						WINDOW* subscrnGraphic, int *currMenu) {
	//Replace Game Menu header with hostname/port prompt
	attron(COLOR_PAIR(WHITE_BLACK));
	mvaddstr(startingRowMenu3 - 2, startingColMenu3, networkPrompt.c_str());
	refresh();
	WINDOW *subscrnMenu = printMenu(menu3Items, -1, NULL,
								MENU3_LENGTH, COLS, startingColMenu3,
								startingRowMenu3);
	subscrnGraphic = paintCubeGraphic(subscrnGraphic,
						"menuCubeRight1_2.txt");
	*currMenu = 3;
	curs_set(1);
	echo();
	refresh();
	char host[256]; char portStr[6]; int port;
	mvwgetnstr(subscrnMenu, 1, menu3Items[0].length() + 1, host, 255);
	mvwgetnstr(subscrnMenu, 2, menu3Items[1].length() + 1, portStr, 5);
	istringstream istr(portStr); istr >> port;
	curs_set(0);
	noecho();
	//startGame(easy, playerCount, host, port);
	
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
	
	/* //Setup subscreen for cube graphic
	int startingColCG = (COLS - MM_CUBE_GRAPHIC_WIDTH)/2,
		startingRowCG = (LINES - MM_CUBE_GRAPHIC_HEIGHT)/4;		//Graphic starts one-quarter of the way down the screen
	WINDOW *subscrnGraphic = newwin(MM_CUBE_GRAPHIC_HEIGHT, MM_CUBE_GRAPHIC_WIDTH, 
							 startingRowCG, startingColCG);
	//Random starting color for cube graphic
	wattron(subscrnGraphic, COLOR_PAIR(rand() % 6 + 15));
	
	//Print initial cube graphic
	paintGraphic(subscrnGraphic, "menuCubeRight1_1.txt", rand()%6+1, false); */
	
	WINDOW* subscrnGraphic = paintCubeGraphic(NULL, "menuCubeRight1_1.txt");
	
	//Setup subscreen for menu outer border
	int startingCol = (COLS - MM_CUBE_GRAPHIC_WIDTH)/2 + 
					  (MM_CUBE_GRAPHIC_WIDTH - MM_WIDTH) / 2, 
		startingRow = ((LINES - MM_CUBE_GRAPHIC_HEIGHT)/4) + 
					   MM_CUBE_GRAPHIC_HEIGHT + 5;		//Menu starts 5 lines below the graphic
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
	WINDOW *subscrnMenu1 = printMenu(menu2Items, startingLineColor, lineColors,
										MENU1_LENGTH, MM_WIDTH);
	
	//mvaddstr(2, 2, menu2Items[0].c_str()); refresh();
	
	//Declare menu 2 and 3 for future use below
	WINDOW *subscrnMenu2, *subscrnMenu3;
	
	//Used later...
	int startingColMenu3 = startingCol + 
		(MM_WIDTH - networkPrompt.length() - 2)/2;
	int startingRowMenu3 = startingRow + 1;
	
	//Start highlighting at line 1
	int cursorPos = 1, currMenu = 1, playerCount = 1;
	highlight(subscrnMenu1, cursorPos, lineColors[cursorPos-1], startingLineColor,
				menu1Items, MENU1_LENGTH, MM_WIDTH);
					 
	bool gameOn = false; 
	omp_set_num_threads(3); 
	#pragma omp parallel sections shared(cursorPos, playerCount)
	{
		#pragma omp section
		{
			int graphicIterator = cursorPos, seedColor = rand() % 6 + 1, 
				prevCurPos = cursorPos;
			bool toggled = false;
			double time = omp_get_wtime(); 
			while(!gameOn) { 
				usleep(50 * 1000);			
				if(time + 1 < omp_get_wtime()) {
					time = omp_get_wtime();
					graphicIterator++;
					if(graphicIterator == 4) graphicIterator = 0;
					seedColor++;
					if(seedColor == 7) seedColor = 1;
					if(prevCurPos != cursorPos) {
						toggled = true;
						prevCurPos = cursorPos;
						/* //Clear contents of sub-window
						wattron(subscrnGraphic, COLOR_PAIR(BLACK_BLACK));
						for (int y = 0; y < MM_CUBE_GRAPHIC_HEIGHT; y++)
							mvwhline(subscrnGraphic, y, 0, ' ', MM_CUBE_GRAPHIC_WIDTH);
						wrefresh(subscrnGraphic); */
					}
					else toggled = false;
				}
				if(currMenu == 1 && cursorPos == 1) {
					if(graphicIterator == 0) paintGraphic(subscrnGraphic, 
						"menuCubeLeft1_1.txt", seedColor, toggled);
					else if(graphicIterator == 1) paintGraphic(subscrnGraphic, 
						"menuCubeRight1_1.txt", seedColor, toggled);
					else if(graphicIterator == 2) paintGraphic(subscrnGraphic, 
						"menuCubeLeft2_1.txt", seedColor, toggled);
					else paintGraphic(subscrnGraphic, 
						"menuCubeRight2_1.text", seedColor, toggled);
				}
				else if(currMenu == 1 && cursorPos == 2) {
					if(graphicIterator == 0) paintGraphic(subscrnGraphic, 
						"menuCubeLeft1_2.txt", seedColor, toggled);
					else if(graphicIterator == 1) paintGraphic(subscrnGraphic, 
						"menuCubeRight1_2.txt", seedColor, toggled);
					else if(graphicIterator == 2) paintGraphic(subscrnGraphic, 
						"menuCubeLeft2_2.txt", seedColor, toggled);
					else paintGraphic(subscrnGraphic, 
						"menuCubeRight2_2.text", seedColor, toggled);
				}
				else if(currMenu == 1 && cursorPos == 3) {
					paintGraphic(subscrnGraphic, "highScore.txt", seedColor, toggled);
				}
				else if(currMenu == 1 && cursorPos == 4) 
					if(toggled == true)
					paintGraphic(subscrnGraphic, 
						"menuCubeLeft1_1.txt", seedColor, toggled);
			}
		}
		
		#pragma omp section 
		{
			double time1 = omp_get_wtime(); 
			int colOffset = 0;
			while(1) {
				usleep(25 * 1000);
				if(currMenu == 1) colOffset = 0;
				if(currMenu == 2){
					if(cursorPos == 1 && time1 + 0.25 < omp_get_wtime()) {
						time1 = omp_get_wtime();
						if(playerCount == 1)
							subscrnGraphic = 
								paintCubeGraphic(subscrnGraphic,
									"menuCubeRight1_1.txt", colOffset);
						else
							subscrnGraphic = 
								paintCubeGraphic(subscrnGraphic,
									"menuCubeRight1_2.txt", colOffset);
						wrefresh(subscrnGraphic);
						colOffset++;
					}
					else if(cursorPos == 2 && time1 + 0.1 < omp_get_wtime()) {
						time1 = omp_get_wtime();
						if(playerCount == 1)
							subscrnGraphic = 
								paintCubeGraphic(subscrnGraphic,
									"menuCubeRight1_1.txt", colOffset);
						else
							subscrnGraphic = 
								paintCubeGraphic(subscrnGraphic,
									"menuCubeRight1_2.txt", colOffset);
						wrefresh(subscrnGraphic);
						colOffset++;
					}
					else if(cursorPos == 3 && time1 + 0.05 < omp_get_wtime()) {
						time1 = omp_get_wtime();
						if(playerCount == 1)
							subscrnGraphic = 
								paintCubeGraphic(subscrnGraphic,
									"menuCubeRight1_1.txt", colOffset);
						else
							subscrnGraphic = 
								paintCubeGraphic(subscrnGraphic,
									"menuCubeRight1_2.txt", colOffset);
						wrefresh(subscrnGraphic);
						colOffset++;
					}
					if(colOffset == COLS - (COLS - MM_CUBE_GRAPHIC_WIDTH)/2)
						colOffset = -((COLS - MM_CUBE_GRAPHIC_WIDTH)/2);
				}
			}
		}
		
		#pragma omp section
		{
			while(1) {
				int c = getch();
				if(c == KEY_DOWN || c == 'k') {
					if(cursorPos != 4 && currMenu == 1) {
						cursorPos++;
						highlight(subscrnMenu1, cursorPos, lineColors[cursorPos-1], 
							startingLineColor, menu1Items, MENU1_LENGTH, MM_WIDTH);
					}
					else if(cursorPos != 4 && currMenu == 2) {
						cursorPos++;
						highlight(subscrnMenu2, cursorPos, lineColors[cursorPos-1], 
							startingLineColor, menu2Items, MENU1_LENGTH, MM_WIDTH);
					}
					if(currMenu == 1 && cursorPos == 2) playerCount = 2;
				}
				else if(c == KEY_UP || c == 'i') {  
					if(cursorPos != 1 && currMenu == 1) {
						cursorPos--;
						highlight(subscrnMenu1, cursorPos, lineColors[cursorPos-1], 
							startingLineColor, menu1Items, MENU1_LENGTH, MM_WIDTH);
					}
					else if(cursorPos != 1 && currMenu == 2) {
						cursorPos--;
						highlight(subscrnMenu2, cursorPos, lineColors[cursorPos-1], 
							startingLineColor, menu2Items, MENU1_LENGTH, MM_WIDTH);
					}
					if(currMenu == 1 && cursorPos == 1) playerCount = 1;
					else if(currMenu == 1 && cursorPos == 2) playerCount = 2;
				}
				else if(c == KEY_ENTER || c == 10 || c == 13) {
					if(currMenu == 1 && (cursorPos == 1 || cursorPos == 2)) {
						delwin(subscrnMenu1);
						subscrnMenu2 = printMenu(menu2Items, 
							startingLineColor, NULL, MENU2_LENGTH, MM_WIDTH);
						cursorPos = 2;
						highlight(subscrnMenu2, cursorPos, lineColors[cursorPos-1], 
							startingLineColor, menu2Items, MENU1_LENGTH, MM_WIDTH);
						currMenu = 2;
					}
					else if(currMenu == 1 && cursorPos == 3) {
						//delwin(subscrnMenu1);
						//showHighScores();
					}
					else if(currMenu == 1 && cursorPos == 4) {
						delwin(subscrnMenu1);
						// restore original settings and leave
						endwin();
						exit(0);
					}
					else if(currMenu == 2 && cursorPos == 1) {
						delwin(subscrnMenu2); 
						werase(subscrnMenuBorder); wrefresh(subscrnMenuBorder); //delwin(subscrnMenuBorder);
						WINDOW *subscrnMenu3 = 
							hostPrompt(startingColMenu3, startingRowMenu3, 
								subscrnGraphic, &currMenu);
						//startGame(easy, playerCount);
					}
					else if(currMenu == 2 && cursorPos == 2) {
						delwin(subscrnMenu2); 
						werase(subscrnMenuBorder); wrefresh(subscrnMenuBorder); //delwin(subscrnMenuBorder);
						WINDOW *subscrnMenu3 = 
							hostPrompt(startingColMenu3, startingRowMenu3, 
								subscrnGraphic, &currMenu);
						//startGame(normal, playerCount);
					}
					else if(currMenu == 2 && cursorPos == 3) {
						delwin(subscrnMenu2); 
						werase(subscrnMenuBorder); wrefresh(subscrnMenuBorder); //delwin(subscrnMenuBorder);
						WINDOW *subscrnMenu3 = 
							hostPrompt(startingColMenu3, startingRowMenu3, 
								subscrnGraphic, &currMenu);
						//startGame(hard, playerCount);
					}
					else if(currMenu == 2 && cursorPos == 4) {
						delwin(subscrnMenu2);
						subscrnMenu1 = printMenu(menu1Items, startingLineColor, NULL,
													MENU1_LENGTH, MM_WIDTH);
						subscrnGraphic = paintCubeGraphic(subscrnGraphic,
											"menuCubeRight1_1.txt");
						cursorPos = 1;
						highlight(subscrnMenu1, cursorPos, lineColors[cursorPos-1], 
							startingLineColor, menu1Items, MENU1_LENGTH, MM_WIDTH);
						currMenu = 1;
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
// https://stackoverflow.com/questions/1182665/curses-getting-arrow-keys
// https://stackoverflow.com/questions/11067800/ncurses-key-enter-is-fail
// https://stackoverflow.com/questions/2545720/error-default-argument-given-for-parameter-1