#ifndef init_color_h
#define init_color_h

#include <curses.h>

int init_intro_color(){
	
	start_color();
	refresh();

	//Setup indexes 1-6 with white background 
	//and each other curses color available
	//(except black)
	init_pair(1, COLOR_RED, COLOR_WHITE);
	init_pair(2, COLOR_YELLOW, COLOR_WHITE);
	init_pair(3, COLOR_GREEN, COLOR_WHITE);
	init_pair(4, COLOR_CYAN, COLOR_WHITE);
	init_pair(5, COLOR_BLUE, COLOR_WHITE);
	init_pair(6, COLOR_MAGENTA, COLOR_WHITE);
	
	return 0;
}

#endif

// References
// https://www.linuxjournal.com/content/programming-color-ncurses
// https://www.linuxjournal.com/content/about-ncurses-colors-0
