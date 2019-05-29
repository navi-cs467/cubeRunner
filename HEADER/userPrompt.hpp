/*****************************************************
** Program name: userPrompt.hpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 5/28/2019
** Description: Header file for userPrompt function.
*****************************************************/

#ifndef userPrompt_hpp
#define userPrompt_hpp

#include "include.hpp"
#include "constants.hpp"
#include "using.hpp"
#include "printMenu.hpp"
#include "paintCubeGraphic.hpp"

extern const string usernamePrompt;
extern const vector<string>menu4Items;

WINDOW *userPrompt(int startingColMenu4, int startingRowMenu4,
						WINDOW **subscrnGraphic, int *currMenu,
						bool *escaped, char *username, bool twoPlayer);

#endif
