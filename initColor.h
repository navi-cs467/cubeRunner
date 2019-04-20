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
#define RED_WHITE 15
#define YELLOW_WHITE 16
#define GREEN_WHITE 17
#define CYAN_WHITE 18
#define BLUE_WHITE 19
#define MAGENTA_WHITE 20
#define WHITE_BLACK 21
#define WHITE_WHITE 22
#define BLACK_RED 23
#define BLACK_YELLOW 24
#define BLACK_GREEN 25
#define BLACK_CYAN 26
#define BLACK_BLUE 27
#define BLACK_MAGENTA 28
#define BLACK_WHITE 29

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
	init_pair(15, COLOR_RED, COLOR_WHITE);
	init_pair(16, COLOR_YELLOW, COLOR_WHITE);
	init_pair(17, COLOR_GREEN, COLOR_WHITE);
	init_pair(18, COLOR_CYAN, COLOR_WHITE);
	init_pair(19, COLOR_BLUE, COLOR_WHITE);
	init_pair(20, COLOR_MAGENTA, COLOR_WHITE);
	init_pair(21, COLOR_WHITE, COLOR_BLACK);
	init_pair(22, COLOR_WHITE, COLOR_WHITE);
	init_pair(23, COLOR_BLACK, COLOR_RED);
	init_pair(24, COLOR_BLACK, COLOR_YELLOW);
	init_pair(25, COLOR_BLACK, COLOR_GREEN);
	init_pair(26, COLOR_BLACK, COLOR_CYAN);
	init_pair(27, COLOR_BLACK, COLOR_BLUE);
	init_pair(28, COLOR_BLACK, COLOR_MAGENTA);
	init_pair(29, COLOR_BLACK, COLOR_WHITE);
	
	return 0;
}

#endif

// References
// https://www.linuxjournal.com/content/programming-color-ncurses
// https://www.linuxjournal.com/content/about-ncurses-colors-0
