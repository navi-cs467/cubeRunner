/************************************************************
** Program name: transitionAnimationInsideThread.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/23/2019
** Description: Source file for transitionAnimation function,
**				when called within an existing thread (i.e.
**				from within game execution).
*************************************************************/

#include "../HEADER/transitionAnimation.hpp"

void transitionAnimationInsideThread(const char* fileName, 
									 int graphicWidth, 
									 int graphicHeight,
									 int backgroundColor,
									 int startingSeedColor,
									 int promptColor,
									 int *userInput,
									 int *confirmedGameOver,
									 struct gameData *gameStats)
{ 
	clear();  //Curses clear-screen call
	
	//Explicitly set userInput
	*userInput = 0;
	
	//animation1Completed marker
	bool animation1Completed = false;
	
	attron(A_BOLD);
	if(*userInput != 10) {
		// paint screen background color
		attron(COLOR_PAIR(backgroundColor));
		for (int y = 0; y < LINES; y++) {
			mvhline(y, 0, ' ', COLS);
		}
		refresh();
	}
   
    //Turn on bold attribute
    attron(A_BOLD);
	
	int startingCol = (COLS - graphicWidth)/2, 
		startingRow = (LINES - graphicHeight)/2;
	WINDOW *subscrn = newwin(graphicHeight, graphicWidth, 
							 startingRow, startingCol);
	
	if(*userInput != 10) {
		// paint subscreen background color
		wattron(subscrn, COLOR_PAIR(backgroundColor)); wattron(subscrn, A_BOLD);
		for (int y = 0; y < graphicHeight; y++) {
			mvwhline(subscrn, y, 0, ' ', graphicWidth);
		}
		wrefresh(subscrn);
	}
   
    //Load transitionAnimation graphic into cmdoutlinesGraphics from file
    loadGraphic(fileName);
	
    int color, row;
	//Paint the rows to the screen
	for (row = 0, color = rand() % 6 + startingSeedColor; 
			row < graphicHeight - 2 && *userInput != 10;
			row++, color++) {  
	   if(color == startingSeedColor + 6) color = startingSeedColor;		//Cycle to first index when necessary
	   //Change color
	   wattron(subscrn, COLOR_PAIR(color)); 
	   mvwaddstr(subscrn, row, 0,
		 cmdoutlinesGraphics[row].c_str());  //Curses call to move to the
									 //specified position and
									 //paint a string there
		usleep(100 * 1000);	//Sleep for 100 milliseconds for animation effect
		//usleep(1 * 1000);	//Sleep for 1 milliseconds (testing only)
		wrefresh(subscrn);  //Now make the changes actually appear on the screen,
							//using this call to the curses library
	}
		
	wattron(subscrn, COLOR_PAIR(backgroundColor));		//Ensures these two lines are painted 
	mvwhline(subscrn, row, 0, ' ', graphicWidth);	//correctly before threading below
	mvwhline(subscrn, row + 1, 0, ' ', graphicWidth);	
		
	if(!userInput != 10) usleep(200 * 1000);	//Sleep for 200 milliseconds
	
	//Print blinking continue prompt (if animation was not skipped)
	//attron(A_BLINK);	//Doesn't work, (at least for some terminals)
	double time = omp_get_wtime(); 
	bool visible = false; 
	if(*userInput != 10){
		animation1Completed = true;
		while(*userInput != 10) {
			if(time + 0.6 < omp_get_wtime() && visible == false){
				time = omp_get_wtime();
				wattron(subscrn, COLOR_PAIR(promptColor));
				char prompt[28] = "Press Enter Key to Continue";
				mvwaddstr(subscrn, row + 1, graphicWidth / 2 -
										    strlen(prompt) / 2, 
						  "Press Enter Key to Continue");
				wrefresh(subscrn);
				visible = true;
			}
			else if(time + 1.75 < omp_get_wtime() && visible == true){
				time = omp_get_wtime();
				wattron(subscrn, COLOR_PAIR(backgroundColor));
				mvwhline(subscrn, row + 1, 0, ' ', graphicWidth);
				wrefresh(subscrn);
				visible = false;
			}
		}
	}

	//Makes sure nothing erroneous is printed next time cmdoutlinesGraphics
	//is used.
	for(vector<string>::iterator it = cmdoutlinesGraphics.begin();
		it < cmdoutlinesGraphics.end(); it++)
			*it = "";
	
	//Paint in alternating color perimeters converging into middle of screen
	if(animation1Completed)	{
		//Reset input so, second animation can be bypassed with another stroke
		//of the enter key
		*userInput = 0;
		for(int i = LINES, j = COLS, color = rand() % 6 + 9, z = 0; 
			*userInput != 10 && i >= LINES/2 && j >= COLS/2; 
			i -= MAIN_BORDER_ANIMATION_ROW_WIDTH, j -= MAIN_BORDER_ANIMATION_COL_WIDTH, color++, z++) {
			if(color == 15) color = 9;	//Reset color when necessary
			attron(COLOR_PAIR(color));
			for(int k = LINES - i, l = 0; l < MAIN_BORDER_ANIMATION_ROW_WIDTH; l++) 
				mvhline(k + l, COLS - j, ' ', j - (z * MAIN_BORDER_ANIMATION_COL_WIDTH));
			for(int k = i - 1, l = 0; l < MAIN_BORDER_ANIMATION_ROW_WIDTH; l++) 
				mvhline(k - l, COLS - j, ' ', j - (z * MAIN_BORDER_ANIMATION_COL_WIDTH));
			for(int k = COLS - j, l = 0; l < MAIN_BORDER_ANIMATION_COL_WIDTH; l++) 
				mvvline(LINES - i, k + l, ' ', i - (z * MAIN_BORDER_ANIMATION_ROW_WIDTH));
			for(int l = 0; l < MAIN_BORDER_ANIMATION_COL_WIDTH; l++) 
				mvvline(LINES - i, j - l, ' ', i - (z * MAIN_BORDER_ANIMATION_ROW_WIDTH));
			
			usleep(100 * 1000);	//Sleep for 100 milliseconds for animation effect
			//usleep(1 * 1000);	//Sleep for 1 milliseconds (testing only)
			refresh();
		}
	}

	//attron(COLOR_PAIR(0));	//Reset to default
    //attroff(A_BOLD);
	
	//If this is the game over animation, set confirmedGameOver
	//flag so user input loop(s) terminate
	if(confirmedGameOver != NULL) {
		*confirmedGameOver = 1;
		clear(); attron(COLOR_PAIR(WHITE_BLACK)); refresh();
		//printw("Print Final Game Stats Here..."); 
		
		WINDOW *subscrnGraphic = NULL;
		if(gameStats->secondName == NULL)
			subscrnGraphic =
				paintCubeGraphic(subscrnGraphic,
					"GRAPHICS/menuCubeRight1_1.txt");
		else
			subscrnGraphic =
				paintCubeGraphic(subscrnGraphic,
					"GRAPHICS/menuCubeRight1_2.txt");
		
		int startingCol = (COLS - MM_GRAPHIC_WIDTH)/2 + 
						(MM_GRAPHIC_WIDTH - MM_WIDTH)/2 + 1,
			startingRow = ((LINES - MM_GRAPHIC_HEIGHT)/4) + 
							MM_GRAPHIC_HEIGHT + 2;
		
		string gameStatStr = "Game Stats";
		mvprintw(startingRow, COLS / 2 - (gameStatStr.length() / 2),
				 gameStatStr.c_str());
		attron(A_BOLD); refresh();
		
		ostringstream firstName, secondName, playerNum, time, finalScore;
		
		//Setup firstName display
		firstName.clear();
		firstName << " Name: " << gameStats->firstName;
		
		//Setup secondName display
		secondName.clear();
		if(gameStats->secondName != NULL)
			secondName << " Other Player: " << gameStats->secondName;
		else
			secondName << " Other Player: N/A";
		
		//Setup playerNum
		playerNum << " Player Number: " << gameStats->playerNum;
		
		//Setup time display		
		time.clear();
		if(gameStats->hours < 10)
			time << " Time: " << "0" << gameStats->hours << ":";
		else
			time << " Time: " << gameStats->hours << ":";
		if(gameStats->minutes < 10)
			time << "0" << gameStats->minutes << ":";
		else
			time << gameStats->minutes << ":";
		if(gameStats->seconds < 10)
			time << "0" << gameStats->seconds;
		else
			time << gameStats->seconds;
		
		//Setup finalScore display
		finalScore.clear();
		finalScore << " FINAL SCORE: " << gameStats->finalScore;
		
		//Determine longest string that will be displayed
		ostringstream testStr; int longest = 0;
		testStr.clear(); testStr << gameStats->firstName;
		if(testStr.str().length() > longest) longest = testStr.str().length();
		testStr.clear(); testStr << gameStats->secondName;
		if(testStr.str().length() > longest) longest = testStr.str().length();
		if(time.str().length() > longest) longest = time.str().length();
		testStr.clear(); testStr << gameStats->finalScore;
		if(testStr.str().length() > longest) longest = testStr.str().length();
		
		//mvprintw(5, 5, "%d", longest); refresh();
		
		//Create Outer Border
		WINDOW *subscrnMenuBorder = newwin(9, longest + 12, startingRow + 1, 
			//startingCol - 1);
			COLS / 2 - ((longest + 12) / 2));
		wattron(subscrnMenuBorder, A_BOLD);
		box(subscrnMenuBorder, '|', '_');
		wborder(subscrnMenuBorder, '|', '|', '-', '-', '*', '*', '*', '*');
		wattron(subscrnMenuBorder, A_BOLD); wrefresh(subscrnMenuBorder);
		
		//Create and load inner window
		WINDOW *subscrnMenu = newwin(7, longest + 10, startingRow + 2, 
			//startingCol);
			COLS / 2 - ((longest + 10) / 2));		
		wattron(subscrnMenu, COLOR_PAIR(WHITE_BLACK));
		wattron(subscrnMenu, A_BOLD);		
		box(subscrnMenu, '|', '_'); 
		wborder(subscrnMenu, '|', '|', '-', '-', '*', '*', '*', '*');
		
		mvwprintw(subscrnMenu, 1, 2, firstName.str().c_str());
		mvwprintw(subscrnMenu, 2, 2, secondName.str().c_str());
		mvwprintw(subscrnMenu, 3, 2, playerNum.str().c_str());
		mvwprintw(subscrnMenu, 4, 2, time.str().c_str());
		mvwprintw(subscrnMenu, 5, 2, finalScore.str().c_str());
		
		string highScoreMessage = "CONGRATULATIONS! NEW HIGH SCORE.",
			   exitMessage = "** PRESS ANY KEY TO RETURN TO MAIN MENU **";
		if(isHighScore(gameStats->finalScore) != -2) {
			mvprintw(startingRow + 11, COLS / 2 - (highScoreMessage.length() / 2),
					 highScoreMessage.c_str());
			mvprintw(startingRow + 13, COLS / 2 - (exitMessage.length() / 2),
					 exitMessage.c_str());
		}
		else {
			mvprintw(startingRow + 11, COLS / 2 - (exitMessage.length() / 2),
					 exitMessage.c_str());
		}
		
		refresh();
			
		wrefresh(subscrnMenu);
		
		delwin(subscrnMenu);
		delwin(subscrnMenuBorder);
	}
	
	delwin(subscrn);
}

// References
// https://stackoverflow.com/questions/26807623/interrupt-ncurses-getch-on-incoming-signal
// https://stackoverflow.com/questions/717572/how-do-you-do-non-blocking-console-i-o-on-linux-in-c