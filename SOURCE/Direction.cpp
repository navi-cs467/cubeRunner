/***************************************************
** Program name: Direction.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Operator++ overload for enumerated
**				type Direction.
***************************************************/

#include "../HEADER/Direction.hpp"

Direction& operator++(Direction& dir) {
	return dir = static_cast<Direction>(static_cast<int>(dir) + 1);
}