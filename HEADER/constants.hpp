/*******************************************************
** Program name: constants.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/23/2019
** Description: Header file with constant definitions.
*******************************************************/

#ifndef constants_hpp
#define constants_hpp

/*** Menu-Related ***/
#define MIN_WIN_HEIGHT 32
#define MIN_WIN_WIDTH 135
#define INTRO_HEIGHT 28
#define INTRO_WIDTH 120
#define MM_GRAPHIC_HEIGHT 9			//MM stands for Main Menu
#define MM_GRAPHIC_WIDTH 40
#define MM_HEIGHT 25
#define MM_WIDTH 25
#define MAIN_BORDER_ANIMATION_ROW_WIDTH 2
#define MAIN_BORDER_ANIMATION_COL_WIDTH 6
#define MAX_MENU_ITEMS 4
#define MENU1_LENGTH 4
#define MENU2_LENGTH 4
#define MENU3_LENGTH 4
#define ONE_PLAYER 1
#define TWO_PLAYER 2
#define HIGH_SCORE 3
#define EXIT 4
#define EASY 1
#define NORMAL 2
#define HARD 3
#define BACK 4
/*** End Menu-Related ***/

/*** Game-Related ***/
#define REFRESH_RATE 0.001
#define OBS_COUNT_HARD 25
#define OBS_COUNT_MED 15
#define OBS_COUNT_EASY 10
#define NUM_MCS_EASY 60
#define NUM_DIRECTIONS 8
#define MAX_MOVE_COUNTER 25
#define MIN_MOVE_COUNTER 5
#define TRANSITION_SCORE_INTERVAL 500

//"Rates" are scan intervals,
//so as the following three constants decrease,
//their  respective rates increase
#define SCROLL_RATE_HARD 0.35
#define SCROLL_RATE_NORMAL 0.75
#define SCROLL_RATE_EASY 1.5

//Scroll "rate" is divided by this constant
//to yield move "rate". As this constant
//increases, move rate (i.e. the scan interval
//interval between calls to move()) increases.
#define MOVE_RATE_DIVISOR 6.

//Number of moves since last hit 
//before Obstacle's hit count is incremented
//(up to max hits(
#define OBS_REGEN_RATE_EASY 60
#define OBS_REGEN_RATE_NORMAL 40
#define OBS_REGEN_RATE_HARD 20

//"Rate" at which the rate intervals described
//above decrease as the game progresses.
//As this value decreases, the rate at which
//move and scroll speeds increases.
#define SCROLL_MOVE_UPDATE_RATE 0.9

//message buffer size for all messages sent and received between client and server
#define MSG_SIZE 20

//Number of randomly generated Asteroid graphic types
#define NUM_ASTEROID_TYPES 100
#define NUM_ROCK_TYPES 100

/*** End Game-Related ***/

/*** DEBUG ***/
#define DEBUG 0

#endif /* constants_hpp */
