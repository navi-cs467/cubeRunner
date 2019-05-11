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