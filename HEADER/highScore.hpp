/*****************************************************
** Program name: highScore.hpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 5/24/2019
** Description: Header file for highScore functions.
*****************************************************/

#ifndef highScore_hpp
#define highScore_hpp

#include "include.hpp"
#include "constants.hpp"

int isHighScore (int score);
void addScoreSingle(int score, char* name, int hours, int minutes, int seconds, int mode);
void addScoreMulti(int score, char* firstName, char* secondName, int hours, int minutes, int seconds, int mode);
void displayScores(void);

#endif
