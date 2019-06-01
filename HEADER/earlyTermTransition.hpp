/**************************************************************
** Program name: earlyTermTransition.hpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 5/31/2019
** Description: Header file for earlyTermTransition function.
***************************************************************/

#ifndef earlyTermTransition_hpp
#define earlyTermTransition_hpp

#include "include.hpp"
#include "constants.hpp"
#include "using.hpp"

extern WINDOW *scrn;

void earlyTermTransition(int playerNum, int *userInput, int *confirmedGameOver, bool *hasTerminated);

#endif /* waitingForOtherPlayer_hpp */
