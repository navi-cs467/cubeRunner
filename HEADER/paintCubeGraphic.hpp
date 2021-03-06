/************************************************************
** Program name: paintCubeGraphic.hpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/23/2019
** Description: Header file for paintCubeGraphic function.
************************************************************/

#ifndef paintCubeGraphic_hpp
#define paintCubeGraphic_hpp

#include "include.hpp"
#include "constants.hpp"
#include "using.hpp"
#include "paintGraphic.hpp"

WINDOW* paintCubeGraphic(WINDOW *subscrnGraphic, 
						 char* fileName, int offset = 0,
						 int obsEndCol = -1, int obsColor = -1, 
					     int backgroundColor = -1);
							
#endif /* paintCubeGraphic_hpp */