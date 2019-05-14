/*****************************************************
** Program name: hostPrompt.hpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/23/2019
** Description: Header file for hostPrompt function.
*****************************************************/

#ifndef hostPrompt_hpp
#define hostPrompt_hpp

#include "include.hpp"
#include "constants.hpp"
#include "using.hpp"
#include "printMenu.hpp"
#include "paintCubeGraphic.hpp"

extern const string networkPrompt;
extern const vector<string>menu3Items;

WINDOW *hostPrompt(int startingColMenu3, int startingRowMenu3,
						WINDOW **subscrnGraphic, int *currMenu,
						bool *connected, char *host, char *port);

#endif /* hostPrompt_hpp */
