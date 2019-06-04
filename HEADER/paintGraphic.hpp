/******************************************************
** Program name: paintGraphic.hpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/23/2019
** Description: Header file for paintGraphic function.
*******************************************************/

#ifndef paintGraphic_hpp
#define paintGraphic_hpp

#include "loadGraphic.hpp"
#include "include.hpp"
#include "constants.hpp"
#include "using.hpp"

void paintGraphic(WINDOW *subscrn, char* fileName, 
					int seedColor, bool toggled, int offset = 0,
					int obsEndCol = -1, int obsColor = -1, 
					int backgroundColor = -1);

#endif /* paintGraphic_hpp */