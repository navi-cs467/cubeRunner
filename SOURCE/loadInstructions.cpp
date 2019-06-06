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
	/*fstream f;
	string line;
	f.open("../GRAPHICS/instructionsText.txt", ios::in);
	move(0,0);
	if(f.is_open()){
		while(getline(f, line)){	
           		instructions.push_back("%s\n", line.c_str());
		}
		f.close();  //Close file
	}
	else{
		printw("INSTRUCTIONS GRAPHIC FILE DID NOT OPEN\n\n\n");
		printw("Press any key to continue...\n\n\n");
   	}*/
	
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
	instructions.push_back("   GAME CONTROLS:\n");
	instructions.push_back("   Command the token to move in various directions using the following keys:\n");
	instructions.push_back("\n");
	instructions.push_back("      w or up arrow - up\n");
	instructions.push_back("      s or down arrow - down\n");
   	instructions.push_back("      a or left arrow - left\n");
	instructions.push_back("      d or right arrow - right\n");
	instructions.push_back("      e - left_up    (single player only)\n");
	instructions.push_back("      r - right_up   (single player only)\n");
	instructions.push_back("      c - left_down  (single player only)\n");
	instructions.push_back("      v - right_down (single player only)\n");
	instructions.push_back("      p - pause game (MULTIPLAYER ONLY: Cube will reset and moving Obstacles may advance beyond their current positions when resumed.)\n");
	instructions.push_back("      spacebar - fire shot (shot is released in the last commanded token direction)\n");
	instructions.push_back("      l/u - lock/unlock scroll direction changes in the Space world\n");
	instructions.push_back("      t - transition to next world (intended for testing/grading purposes only, set TRANSITION_WITH_INPUT in constants.hpp)\n");
	instructions.push_back("\n");
	instructions.push_back("   In a single player game mode, the player will have access to all game controls. In a two player game mode, player 1 has up, down,\n");
	instructions.push_back("   and fire shots control and player 2 has left, right, and fire shots control. (By enabling the constant MULTIPLAYER_DUAL_AXIS_MODE,\n");
	instructions.push_back("   both players are given control of movement along both axes. See constants.hpp to modify this constant.)\n");
	instructions.push_back("\n");
	instructions.push_back("   SCORE: \n");
	instructions.push_back("   Collect as many mini-cubes as possible. Each mini-cube collected is worth 10 points.\n");
	instructions.push_back("   Shoot down as many Obstacles as possible. The number of hits required to do so varies by\n");
	instructions.push_back("   Obstacle type and game mode (difficulty). Points obtained equals number of hits required x 10.\n");
	instructions.push_back("   When an Obstacle has been hit and is 'damaged', it will turn red and an indicator at its top-\n");
	instructions.push_back("   left corner will display the number of hits remaining before it is shot down. Careful though,\n");
	instructions.push_back("   as Obstacles will regenerate after they are hit if they are not hit again in quick enough\n");
	instructions.push_back("   succession!\n");
	
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