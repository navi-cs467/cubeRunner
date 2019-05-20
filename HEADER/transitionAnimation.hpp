/*****************************************************
** Program name: transitionAnimation.hpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/23/2019
** Description: Header file for transitionAnimation function.
*****************************************************/

#ifndef transitionAnimation_hpp
#define transitionAnimation_hpp

#include "include.hpp"
#include "constants.hpp"
#include "using.hpp"
#include "loadGraphic.hpp"

void transitionAnimation(const char* fileName,
						 int graphicWidth,
						 int graphicHeight,
						 int backgroundColor,
						 int startingSeedColor,
						 int promptColor);

#endif /* transitionAnimation_hpp */