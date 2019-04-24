/*****************************************************
** Program name: highlight.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/23/2019
** Description: Source file for highlight function.
*****************************************************/

#include "../HEADER/highlight.hpp"

//Inverts the line at cursorPos's color scheme. Will repaint the sub-window
//beforehand, so the line at the previous cursorPos will also be unhighlighted.
void highlight(WINDOW *subscrn, int cursorPos, int color, int startingLineColor,
				const vector<string> &menuItems, int menuLength, int menuWidth)
{  
   //Determine and set color scheme based on current line color
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
   
   //Delete the current sub-window
   delwin(subscrn);
   
   //Reprint the menu
   subscrn = printMenu(menuItems, startingLineColor, NULL, 
						menuLength, menuWidth);
   
   //Highlight line
   wattron(subscrn, COLOR_PAIR(barColor));
   //int y, x; getmaxyx(subscrn, y, x);	//Use COLS and LINES instead
   mvwhline(subscrn, cursorPos, 1, ' ', MM_WIDTH - 2);
   wattron(subscrn, COLOR_PAIR(highlightColor));
   wattroff(subscrn, A_BOLD);
   mvwaddstr(subscrn, cursorPos, 
		(MM_WIDTH - cmdoutlines[cursorPos - 1].length())/2, 
		cmdoutlines[cursorPos - 1].c_str());
   wrefresh(subscrn);  // make the change appear on the screen 
}