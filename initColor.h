#ifndef init_color_h
#define init_color_h

#include <curses.h>

//First color = foreground, second color = background
//Ex. RED_BLACK has red foreground, black background

#define RED_BLACK 1
#define YELLOW_BLACK 2
#define GREEN_BLACK 3
#define CYAN_BLACK 4
#define BLUE_BLACK 5
#define MAGENTA_BLACK 6
#define WHITE_RED 7
#define BLACK_BLACK 8
#define RED_RED 9
#define YELLOW_YELLOW 10
#define GREEN_GREEN 11
#define CYAN_CYAN 12
#define BLUE_BLUE 13
#define MAGENTA_MAGENTA 14

int initColor(){
	
	start_color();
	refresh();

	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);
	init_pair(4, COLOR_CYAN, COLOR_BLACK);
	init_pair(5, COLOR_BLUE, COLOR_BLACK);
	init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(7, COLOR_WHITE, COLOR_RED);
	init_pair(8, COLOR_BLACK, COLOR_BLACK);
	init_pair(9, COLOR_RED, COLOR_RED);
	init_pair(10, COLOR_YELLOW, COLOR_YELLOW);
	init_pair(11, COLOR_GREEN, COLOR_GREEN);
	init_pair(12, COLOR_CYAN, COLOR_CYAN);
	init_pair(13, COLOR_BLUE, COLOR_BLUE);
	init_pair(14, COLOR_MAGENTA, COLOR_MAGENTA);
	
	return 0;
}

#endif

// References
// https://www.linuxjournal.com/content/programming-color-ncurses
// https://www.linuxjournal.com/content/about-ncurses-colors-0
