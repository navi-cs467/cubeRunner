/*****************************************************
** Program name: loadGraphic.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/23/2019
** Description: Source file for loadGraphic function.
*****************************************************/

#include "../HEADER/loadGraphic.hpp"

//Loads transitionAnimation graphic in cmdoutlinesGraphics
void loadGraphic(const char* fileName)
{  
   
   //Makes sure nothing erroneous is printed next time cmdoutlinesGraphics
   //is used.
   //for(vector<string>::iterator it = cmdoutlinesGraphics.begin();
   //   it < cmdoutlinesGraphics.end(); it++)
   //      *it = "";
   cmdoutlinesGraphics.clear();
   
   fstream f; string line;
   f.open(fileName, ios::in);  //Open transitionAnimation graphic file
   for (int row = 0; row < INTRO_HEIGHT; row++) {
      if (getline(f, line))	{	  //If not end of file, add line to cmdoutlines
		  //Don't want stored line to exceed width of screen, which the
		  //Curses library provides to us in the variable COLS, so truncate
		  //to at most COLS characters
		  if(line.size() > COLS) line = line.substr(0, COLS);
		  cmdoutlinesGraphics.push_back(line);
	  }
	  else break;
   }
   f.close();  //Close file
}

void loadGraphicInstructions(void){
	/*fstream f;
	string line;
	f.open("../GRAPHICS/instructionsText.txt", ios::in);
	move(0,0);
	if(f.is_open()){
		while(getline(f, line)){	
           		printw("%s\n", line.c_str());
		}
		f.close();  //Close file
	}
	else{
		printw("INSTRUCTIONS GRAPHIC FILE DID NOT OPEN\n\n\n");
		printw("Press any key to continue...\n\n\n");
   	}*/

	printw("   OBJECTIVE:\n");
	printw("   Navigate through three worlds (Water, Land, Space) while avoiding stationary/moving obstacles and collecting as many \"mini-cubes\"\n");
   	printw("   as possible. The token movement behaviour and obstacles differ in the various worlds:\n\n");
	printw("      Water - With no user input, the token will remain in the same location relative to console boundaries. Obstacles include\n");
	printw("      sharks, octopuses, coral, and seaweed.\n");
	printw("      Land -  With no user input or only horizontal input, the token will \"fall\" to the bottom of the console. Obstacles include\n");
	printw("      birds, bats, rocks, and trees.\n");
	printw("      Space - With no user input, the token will continue to move in the last commanded direction. Obstacles include comets,\n");
	printw("      asteroids, etc.\n\n");
	printw("   Colliding with obstacles results in token death and colliding with mini-cubes results in points added to the total score. Try to\n");
	printw("   collect as many mini-cubes as you can to achieve the highest score. Shots can be fired to destroy obstacles.\n\n");
	printw("   GAME CONTROLS:\n");
	printw("   Command the token to move in various directions using the following keys:\n\n");
	printw("      w or up arrow - up\n");
	printw("      s or down arrow - down\n");
   	printw("      a or left arrow - left\n");
	printw("      d or right arrow - right\n");
	printw("      spacebar - fire shot (shot is released in the last commanded token direction)\n");
	printw("      t - transition to next world (intended for testing purposes only)\n\n");
	printw("   In a single player game mode, the player will have access to all game controls. In a two player game mode, player 1 has up, down,\n");
	printw("   and fire shots control and player 2 has left, right, and fire shots control.\n\n");
	printw("   SCORE: \n");
	printw("   Collect as many mini-cubes as possible. Each mini-cube collected is worth 10 points.\n\n");
	printw("***Press any key to return to the main menu.***\n");

}