/***************************************************
** Program name: printMenu.hpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/23/2019
** Description: Header file for printMenu function.
****************************************************/

#ifndef printMenu_hpp
#define printMenu_hpp

#include "include.hpp"
#include "constants.hpp"
#include "using.hpp"

extern vector<string> cmdoutlines;

WINDOW *printMenu(vector<string> menuprintMenus, int seedColor, 
					int lineColors[], int menuLength, int menuWidth, 
					int altStartingCol = 0, int altStartingRow = 0);
					
#endif /* printMenu_hpp */