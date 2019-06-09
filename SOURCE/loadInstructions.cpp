/***********************************************************
** Program name: loadGInstructions.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 5/30/2019
** Description: Source file for loadInstructions function.
***********************************************************/

#include "../HEADER/loadGraphic.hpp"

extern WINDOW *scrn;

void loadInstructions(void){

	vector<string> instructions;
	instructions.push_back("\n");
	instructions.push_back("   OBJECTIVE:\n");
	instructions.push_back("   Navigate through three worlds (Water, Land, Space) while avoiding both stationary and moving obstacles and collecting as many\n");
	instructions.push_back("   \"mini-cubes\" as possible. The token movement behavior and obstacles differ in the various worlds:\n");
	instructions.push_back("\n");
	instructions.push_back("      Water - With no user input, the token will remain in the same location relative to console boundaries. Obstacles include\n");
	instructions.push_back("              Sharks, Octopuses, Coral, and Seaweed.\n");
	instructions.push_back("      Land -  With no user input or only horizontal input, the token will \"fall\" to the bottom of the console. Obstacles include\n");
	instructions.push_back("              Birds, Bats, Rocks, and Trees.\n");
	instructions.push_back("      Space - The background will scroll in the direction of the last user commanded direction, unless locked by the user(s) (see\n");
	instructions.push_back("              GAME CONTROLS below). Obstacles include comets, Asteroids, Planets, and Spaceships.\n");
	instructions.push_back("\n");
	instructions.push_back("   Colliding with obstacles results in token death and colliding with mini-cubes results in points added to the total score. Try to\n");
	instructions.push_back("   collect as many mini-cubes as you can to achieve the highest score. Shots can be fired to destroy obstacles and earn points.\n");
	instructions.push_back("\n");
	instructions.push_back("   Upon accumulation of 500 points in each world, there will be a transition to the next world in the order Water>Land>Space>Water...\n");
	instructions.push_back("   until game over criteria, token death with 0 lives, is met.\n");
	instructions.push_back("\n");
	instructions.push_back("   Other important information such as elapsed time, score, number of remaining lives, and any other notifications/instructions can\n");
	instructions.push_back("   be found on the bottom line of the client console(s).\n");
	instructions.push_back("\n");
	instructions.push_back("   GAME CONTROLS:\n");
	instructions.push_back("   Command the token to move in various directions using the following keys:\n");
	instructions.push_back("\n");
	instructions.push_back("   NOTE: THE GAME IS NOT COMPATIBLE WITH NUMPAD KEYS, AND THEY MAY CAUSE UNDEFINED BEHAVIOR\n");
	instructions.push_back("      w or up arrow - up\n");
	instructions.push_back("      s or down arrow - down\n");
	instructions.push_back("      a or left arrow - left\n");
	instructions.push_back("      d or right arrow - right\n");
	instructions.push_back("      e - left_up    (single player only)\n");
	instructions.push_back("      r - right_up   (single player only)\n");
	instructions.push_back("      c - left_down  (single player only)\n");
	instructions.push_back("      v - right_down (single player only)\n");
	instructions.push_back("      p - pause game (MULTIPLAYER ONLY: Cube will reset and moving Obstacles may advance beyond their current positions when resumed.)\n");
	instructions.push_back("      spacebar - fire shot (shot is released in the last move direction and fire shot is unavailable until the existing shot clears)\n");
	instructions.push_back("      l/u - lock/unlock scroll direction changes in the Space world\n");
	instructions.push_back("      t - transition to next world (intended for testing/grading purposes only, set TRANSITION_WITH_INPUT in constants.hpp)\n");
	instructions.push_back("      q or Q or END or ESC - quit the game and return to main menu\n");
	instructions.push_back("\n");
	instructions.push_back("   In a single player game mode, the player will have access to all game controls. In a two player game mode, player 1 has up, down,\n");
	instructions.push_back("   and fire shots control and player 2 has left, right, and fire shots control. (By enabling the constant MULTIPLAYER_DUAL_AXIS_MODE,\n");
	instructions.push_back("   both players are given control of movement along both axes. See constants.hpp to modify this constant.)\n");
	instructions.push_back("\n");
	instructions.push_back("   SCORE: \n");
	instructions.push_back("   Collect as many mini-cubes as possible. Each mini-cube collected is worth 10 points. Shoot down as many Obstacles as possible.\n");
	instructions.push_back("   The number of hits required to do so varies by Obstacle type and game mode (difficulty). Points obtained equals number of hits\n");
	instructions.push_back("   required x 10. When an Obstacle has been hit and is 'damaged', it will turn red and an indicator at its top-left corner will display\n");
	instructions.push_back("   the number of hits remaining before it is shot down. Careful though, as Obstacles will regenerate after they are hit if they are not\n");
	instructions.push_back("   hit again in quick enough succession!\n");

	int scrollPos = 0, input;
	do {

		move(0,0);
		clear();

		//Print instructions (as will fit on the screen)
		for(int i = scrollPos, j = 0;
			i < instructions.size() &&
			j < LINES - 3; i++, j++) {
				printw(instructions[i].c_str());
		}

		//Provide navigation instructions
		string navigateInstructions;
		instructions.size() > LINES - 3 ?
		navigateInstructions = "** USE ARROW KEYS UP/DOWN TO SCROLL. PRESS ENTER OR END TO RETURN. **" :
		navigateInstructions = "** PRESS ENTER OR END TO RETURN. **";

		int curX, curY; getyx(scrn, curX, curY);
		move(curX + 1, (COLS - navigateInstructions.length()) / 2);
		printw(navigateInstructions.c_str());

		refresh();


		input = getch();
		if(input == KEY_DOWN &&
		   instructions.size() > LINES - 3 &&
		   scrollPos + 1 <= instructions.size() - (LINES - 3)) {
			scrollPos++;
		}
		else if(input == KEY_UP && scrollPos > 0) {
			scrollPos--;
		}
		clear();
	}
	while(input != 126 && input != 10);

}
