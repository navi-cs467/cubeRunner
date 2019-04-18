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

#define INTRO_HEIGHT 28
#define INTRO_WIDTH 120
#define MAIN_BORDER_ROW_WIDTH 2
#define MAIN_BORDER_COL_WIDTH 6

#define RED     "\033[31m"      /* Red */
#define RESET   "\033[0m"

WINDOW *scrn; 	// will point to curses window object

vector<string> cmdoutlines(MAXROW, "");  // Screen output vector

int ncmdlines,  // number of rows in cmdoutlines
    nwinlines,  // number of rows our "ps ax" output occupies in the
                //  xterm (or equiv.) window
    winrow,  	// current row position in screen
    cmdstartrow,  // index of first row in cmdoutlines to be displayed
    cmdlastrow;  // index of last row in cmdoutlines to be displayed

// rewrites the line at winrow in bold font
void highlight()
{  int clinenum;
   attron(A_BOLD);  // this curses library call says that whatever we
                    // write from now on (until we say otherwise)
                    // will be in bold font
   // we'll need to rewrite the cmdoutlines line currently displayed
   // at line winrow in the screen, so as to get the bold font
   clinenum = cmdstartrow + winrow;
   mvaddstr(winrow,0,cmdoutlines[clinenum].c_str());
   attroff(A_BOLD);  // OK, leave bold mode
   refresh();  // make the change appear on the screen
}


// loads intro graphic in cmdoutlines
void loadIntro()
{  fstream f; string line;
   f.open("cubeRunner.txt", ios::in);  // open intro graphic file
   for (int row = 0; row < INTRO_HEIGHT; row++) {
      if (getline(f, line))	{	  // if not end of file, add line to cmdoutlines
		  // don't want stored line to exceed width of screen, which the
		  // curses library provides to us in the variable COLS, so truncate
		  // to at most COLS characters
		  //cout << line << " oogle" << endl;
		  if(line.size() > COLS) line = line.substr(0, COLS);
		  // remove EOL character
		  cmdoutlines[row] = line;
		  //cout << cmdoutlines[row] << " oogle" << endl;
	  }
	  else break;
   }
   ncmdlines = INTRO_HEIGHT;
   f.close();  // close pipe
}

// displays last part of command output (as much as fits in screen)
void intro()
{  
	clear();  // curses clear-screen call
	
	// paint screen black
	attron(COLOR_PAIR(BLACK_BLACK));
    for (int y = 0; y < LINES; y++) {
        mvhline(y, 0, ' ', COLS);
    }
	refresh();
   
    // turn on bold attribute
    attron(A_BOLD);
	
	int startingCol = (COLS - INTRO_WIDTH)/2, 
		startingRow = (LINES - INTRO_HEIGHT)/2;
	WINDOW *subscrn = newwin(INTRO_HEIGHT, INTRO_WIDTH, 
							 startingRow, startingCol);
   
    // paint the rows to the screen
    int color, row;
    for (row = 0, color = rand() % 6 + 1; 
			row < INTRO_HEIGHT - 2; row++, color++) {  
       if(color == 7) color = 1;		//Cycle to first index when necessary
	   //Change color
	   wattron(subscrn, COLOR_PAIR(color)); 
	   mvwaddstr(subscrn, row, 0,
		 cmdoutlines[row].c_str());  // curses call to move to the
									 // specified position and
									 // paint a string there
		usleep(100 * 1000);	//Sleep for 100 milliseconds for animation effect
		wrefresh(subscrn);  // now make the changes actually appear on the screen,
							// using this call to the curses library
    }
	
	wattron(subscrn, COLOR_PAIR(BLACK_BLACK));		//Ensures these two lines are painted 
	mvwhline(subscrn, row, 0, ' ', INTRO_WIDTH);	//correctly before threading below
	mvwhline(subscrn, row + 1, 0, ' ', INTRO_WIDTH);	
		
	usleep(200 * 1000);	//Sleep for 200 milliseconds
	
	// print blinking continue prompt
	bool inputReceived = false;
	omp_set_num_threads(2);
	#pragma omp parallel sections shared(subscrn, inputReceived, winrow, row, COLS, cmdoutlines)
	{
		#pragma omp section
		{
			while(!inputReceived) {
				wattron(subscrn, COLOR_PAIR(RED_BLACK));
				mvwaddstr(subscrn, row + 1, 0, cmdoutlines[row + 1].c_str());
				wrefresh(subscrn);
				usleep(1000 * 1000);
				wattron(subscrn, COLOR_PAIR(BLACK_BLACK));
				mvwhline(subscrn, row + 1, 0, ' ', INTRO_WIDTH);
				wrefresh(subscrn);
				usleep(400 * 1000);
			}
		}
		
		#pragma omp section
		{
			if(!inputReceived) getch();
			inputReceived = true;
		}
	}
	
	//attron(A_BLINK);	//Doesn't work, (at least for some terminals)
    
	//attron(COLOR_PAIR(0));	//Reset to default
    //attroff(A_BOLD);
	
	//delwin(subscrn);
}

// moves up/down one line
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
}

void validateWinSize() {
	
	// hide the cursor
	curs_set(0);
	
	clear();  // curses clear-screen call
	
	// min window size is 135x30
    // enforce this minimum, asking the user to resize the window
    // and restart the game if necessary.
    if(LINES < 32 || COLS < 135) {
		// Paint screen red
		attron(COLOR_PAIR(RED_RED));
		attron(A_BOLD);
		for (int y = 0; y < LINES; y++) {
			mvhline(y, 0, ' ', COLS);
		}
		
		// create sub-window, place in center of main window
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
		
		// change attribute to white foreground (for sub-window)
		wattron(subscrn, COLOR_PAIR(WHITE_RED));
		
		// put a border around sub-window and output resize message 
		box(subscrn, '|', '_'); 
		wborder(subscrn, '|', '|', '-', '-', '*', '*', '*', '*');
		mvwprintw(subscrn, 1, 7, "Minimum window size is 135x32.");
		// Convert sizeRows and sizeCols to c-string representation
		ostringstream ostr;
		ostr << "(Current window size is " << COLS << "x" << LINES << ".)";
		mvwprintw(subscrn, 2, 6, ostr.str().c_str());
		mvwprintw(subscrn, 4, 3, "Please resize your window and relaunch.");
		mvwprintw(subscrn, 6, 11, "Press any key to exit.");
		refresh();
		wrefresh(subscrn);
		char c = getch();
		
		// cleanup  and exit
		delwin(scrn);
		endwin();
		exit(1);
    }
}

int main(void)
{  
	// window setup, next 3 lines are curses library calls, a standard
    // initializing sequence for curses programs
    scrn = initscr();
	keypad(stdscr, TRUE);	//enables use of arrow keys
    noecho();  // don't echo keystrokes
    cbreak();  // keyboard input valid immediately, not after hit Enter
	
	//Exit if terminal does not support color
	/* if (has_colors() == FALSE) {
    endwin();
    printf("Your terminal does not support color\n");
    exit(1);
	} */
	
	initColor();	//Initialize color indexes (see initColor.h)
	
	validateWinSize();
	
	srand(time(NULL));	//Seed random number generator with system time

    char c;
    // load intro graphic into cmdoutlines from file
    loadIntro();
    // run intro animation
    intro();
    // user command loop
	
	clear();  // curses clear-screen call

	// Paint in alternating color perimeters converging into middle of screen
	for(int i = LINES, j = COLS, color = rand() % 6 + 9, z = 0; 
		//i >= MAIN_BORDER_ROW_WIDTH * 2 && j >= MAIN_BORDER_COL_WIDTH * 2; 
		z < 12;
		i -= MAIN_BORDER_ROW_WIDTH, j -= MAIN_BORDER_COL_WIDTH, color++, z++){
		if(color == 15) color = 9;	//Reset color when necessary
		attron(COLOR_PAIR(color));
		for(int k = LINES - i, l = 0; l < MAIN_BORDER_ROW_WIDTH; l++) 
			mvhline(k + l, COLS - j, ' ', j - (z * MAIN_BORDER_COL_WIDTH));
		for(int k = i - 1, l = 0; l < MAIN_BORDER_ROW_WIDTH; l++) 
			mvhline(k - l, COLS - j, ' ', j - (z * MAIN_BORDER_COL_WIDTH));
		for(int k = COLS - j, l = 0; l < MAIN_BORDER_COL_WIDTH; l++) 
			mvvline(LINES - i, k + l, ' ', i - (z * MAIN_BORDER_ROW_WIDTH));
		for(int k = j - 1, l = 0; l < MAIN_BORDER_COL_WIDTH; l++) 
			mvvline(LINES - i, j - l, ' ', i - (z * MAIN_BORDER_ROW_WIDTH));
		
		usleep(100 * 1000);	//Sleep for 100 milliseconds for animation effect
		refresh();
	}
	
	
    while (1)  {
       // get user command
       c = getch();
       /* if (c == 'u') updown(-1);
       else if (c == 'd') updown(1);
       else if (c == 'r') rerun();
       else if (c == 'k') prockill();
       else break;  // quit */
	   break;
    }
    // restore original settings and leave
    endwin();
   
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
