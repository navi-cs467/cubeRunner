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
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include "introColorCodes.h"
using std::string;
using std::fstream;
using std::vector;
using std::ios;
using std::cout;
using std::endl;

#define RED     "\033[31m"      /* Red */
#define RESET   "\033[0m"

WINDOW *scrn;; 	// will point to curses window object

vector<string> cmdoutlines(MAXROW, "");  // Screen output vector

int ncmdlines,  // number of rows in cmdoutlines
    nwinlines,  // number of rows our "ps ax" output occupies in the
                //  xterm (or equiv.) window
    winrow,  // current row position in screen
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
{  fstream f; string line; int row;
   f.open("cubeRunner.txt", ios::in);  // open intro graphic file
   for (row = 0; row < MAXROW; row++)  {
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
   ncmdlines = row;
   f.close();  // close pipe
}

// displays last part of command output (as much as fits in screen)
void intro()
{  int row;
   clear();  // curses clear-screen call
   // prepare to paint the (last part of the) 'ps ax' output on the screen
   // two cases, depending on whether there is more output than screen rows;
   // first, the case in which the entire output fits in one screen:
   if (ncmdlines <= LINES)  { // LINES is an int maintained by the curses
                              // library, equal to the number of lines in
                              // the screen
      cmdstartrow = 0;
      nwinlines = ncmdlines;
   }
   else  { // now the case in which the output is bigger than one screen
      //Show only the last part of the graphic as space permits
	  cmdstartrow = ncmdlines - LINES;
      nwinlines = LINES;
   }
   cmdlastrow = cmdstartrow + nwinlines - 1;
   // now paint the rows to the screen
   int color;
   for (row = cmdstartrow, winrow = 0, color = 1;
	row <= cmdlastrow; row++,winrow++, color++) {
      if(color == 7) color = 1;		//Cycle to first index when necessary
	  //Change color
	  attron(COLOR_PAIR(color));
	  mvaddstr(winrow,0,
		cmdoutlines[row].c_str());  // curses call to move to the
												// specified position and
												// paint a string there
   usleep(100 * 1000);	//Sleep for 100 milliseconds for animation effect
   refresh();  			// now make the changes actually appear on the screen,
						// using this call to the curses library
   }
   attron(COLOR_PAIR(0));	//Reset to default
   // highlight the last line
   //winrow--;
   //highlight();
}

// moves up/down one line
void updown(int inc)
{  int tmp = winrow + inc;
   // ignore attempts to go off the edge of the screen
   if (tmp >= 0 && tmp < LINES)  {
      // rewrite the current line before moving; since our current font
      // is non-BOLD (actually A_NORMAL), the effect is to unhighlight
      // this line
      mvaddstr(winrow,0,cmdoutlines[cmdstartrow+winrow].c_str());
      // highlight the line we're moving to
      winrow = tmp;
      highlight();
   }
}

// run/re-run "ps ax"
/* void rerun()
{  runpsax();
   showlastpart();
} */

// kills the highlighted process
/* void prockill()
{  char *pid;
   // strtok() is from C library; see man page
   pid = strtok(cmdoutlines[cmdstartrow+winrow]," ");
   kill(atoi(pid),9);  // this is a Unix system call to send signal 9,
                       // the kill signal, to the given process
   rerun();
} */

int main()
{

	//Tried putting these in both initColorCodes.h & here...
	//start_color();
	//refresh();

	char c;
   // window setup, next 3 lines are curses library calls, a standard
   // initializing sequence for curses programs
   scrn = initscr();
   noecho();  // don't echo keystrokes
   cbreak();  // keyboard input valid immediately, not after hit Enter
   // run 'ps ax' and process the output

   //Exit if terminal does not support color
 	if (has_colors() == FALSE) {
    endwin();
    printf("Your terminal does not support color\n");
    exit(1);
 	}

   int colors = init_intro_color();	//Initialize color indexes for intro
   loadIntro();
   // display in the window
   intro();
   // user command loop
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

   //cout << RED << "TEST" << RESET << endl;

   return 0;
}

// References
// http://www.cplusplus.com/reference/string/string/getline/
// https://stackoverflow.com/questions/8460666/c-does-getline-have-a-maximum-string-or-character-capacity
// http://www.cplusplus.com/reference/string/string/substr/
// https://stackoverflow.com/questions/347949/how-to-convert-a-stdstring-to-const-char-or-char
// https://www.linuxjournal.com/content/programming-color-ncurses
// https://www.linuxjournal.com/content/about-ncurses-colors-0
