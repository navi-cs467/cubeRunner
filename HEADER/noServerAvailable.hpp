/**************************************************************
** Program name: noServerAvailable.hpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 6/7/2019
** Description: Header file for noS function.
***************************************************************/

#ifndef noServerAvailable_hpp
#define noServerAvailable_hpp

#include "include.hpp"
#include "constants.hpp"
#include "using.hpp"

extern WINDOW *scrn;

void noServerAvailable(char *host, char *port,
					   int *userInput, int *confirmedGameOver);

#endif /* noServerAvailable_hpp */
