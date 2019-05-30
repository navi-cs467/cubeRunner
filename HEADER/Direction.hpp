/*************************************************
** Program name: Direction.hpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Enumeration for Direction type.
*************************************************/

#ifndef Direction_hpp
#define Direction_hpp

enum Direction { none,
				 left,
				 left_down,
				 down,
				 right_down,
				 right,
				 right_up,
				 up,
				 left_up
			   };
			   
Direction& operator++(Direction& dir);

#endif

// References
// https://stackoverflow.com/questions/3475152/why-cant-i-increment-a-variable-of-an-enumerated-type