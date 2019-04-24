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

void paintGraphic(WINDOW *subscrn, const char* fileName, 
					int seedColor, bool toggled);

#endif /* paintGraphic_hpp */