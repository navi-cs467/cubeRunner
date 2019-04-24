/*****************************************************
** Program name: highlight.hpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/23/2019
** Description: Header file for highlight function.
*****************************************************/

#ifndef highlight_hpp
#define highlight_hpp

#include "include.hpp"
#include "constants.hpp"
#include "using.hpp"
#include "printMenu.hpp"

extern vector<string> cmdoutlines;

void highlight(WINDOW *subscrn, int cursorPos, int color, int startingLineColor,
				const vector<string> &menuItems, int menuLength, int menuWidth);
				
#endif /* highlight_hpp */