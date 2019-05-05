/*****************************************************
** Program name: intro.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/23/2019
** Description: Source file for intro function.
*****************************************************/

#include "../HEADER/intro.hpp"

//Introduction sequence (plays when game starts)
void intro()
{ 
	clear();  //Curses clear-screen call
	
	// paint screen black
	attron(COLOR_PAIR(BLACK_BLACK));
    for (int y = 0; y < LINES; y++) {
        mvhline(y, 0, ' ', COLS);
    }
	refresh();
   
    //Turn on bold attribute
    attron(A_BOLD);
	
	int startingCol = (COLS - INTRO_WIDTH)/2, 
		startingRow = (LINES - INTRO_HEIGHT)/2;
	WINDOW *subscrn = newwin(INTRO_HEIGHT, INTRO_WIDTH, 
							 startingRow, startingCol);
   
    //Load intro graphic into cmdoutlinesGraphics from file
    loadGraphic("GRAPHICS/cubeRunner.txt");
   
    //Paint the rows to the screen
    int color, row;
    for (row = 0, color = rand() % 6 + 1; 
			row < INTRO_HEIGHT - 2; row++, color++) {  
       if(color == 7) color = 1;		//Cycle to first index when necessary
	   //Change color
	   wattron(subscrn, COLOR_PAIR(color)); 
	   mvwaddstr(subscrn, row, 0,
		 cmdoutlinesGraphics[row].c_str());  //Curses call to move to the
									 //specified position and
									 //paint a string there
		//usleep(100 * 1000);	//Sleep for 100 milliseconds for animation effect
		usleep(1 * 1000);	//Sleep for 1 milliseconds (testing only)
		wrefresh(subscrn);  //Now make the changes actually appear on the screen,
							//using this call to the curses library
    }
	
	wattron(subscrn, COLOR_PAIR(BLACK_BLACK));		//Ensures these two lines are painted 
	mvwhline(subscrn, row, 0, ' ', INTRO_WIDTH);	//correctly before threading below
	mvwhline(subscrn, row + 1, 0, ' ', INTRO_WIDTH);	
		
	usleep(200 * 1000);	//Sleep for 200 milliseconds
	
	//Print blinking continue prompt
	//attron(A_BLINK);	//Doesn't work, (at least for some terminals)
	bool inputReceived = false;
	omp_set_num_threads(2); double time = omp_get_wtime(); bool visible = false;
	#pragma omp parallel sections shared(subscrn, inputReceived, \
	row, COLS, cmdoutlinesGraphics)
	{
		#pragma omp section
		{
			while(!inputReceived) {
				if(time + 0.6 < omp_get_wtime() && visible == false){
					time = omp_get_wtime();
					wattron(subscrn, COLOR_PAIR(RED_BLACK));
					mvwaddstr(subscrn, row + 1, 0, 
						cmdoutlinesGraphics[row + 1].c_str());
					wrefresh(subscrn);
					visible = true;
				}
				else if(time + 1.75 < omp_get_wtime() && visible == true){
					time = omp_get_wtime();
					wattron(subscrn, COLOR_PAIR(BLACK_BLACK));
					mvwhline(subscrn, row + 1, 0, ' ', INTRO_WIDTH);
					wrefresh(subscrn);
					visible = false;
				}
			}
		}
		
		#pragma omp section
		{
			if(!inputReceived) getch();
			inputReceived = true;
		}
	}

	//Makes sure nothing erroneous is printed next time cmdoutlinesGraphics
	//is used.
	for(vector<string>::iterator it = cmdoutlinesGraphics.begin();
		it < cmdoutlinesGraphics.end(); it++)
			*it = "";
	
	//Paint in alternating color perimeters converging into middle of screen
	for(int i = LINES, j = COLS, color = rand() % 6 + 9, z = 0; 
		i >= LINES/2 && j >= COLS/2; 
		i -= MAIN_BORDER_ANIMATION_ROW_WIDTH, j -= MAIN_BORDER_ANIMATION_COL_WIDTH, color++, z++){
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
		
		//usleep(100 * 1000);	//Sleep for 100 milliseconds for animation effect
		usleep(1 * 1000);	//Sleep for 1 milliseconds (testing only)
		refresh();
	}
	
	
    
	//attron(COLOR_PAIR(0));	//Reset to default
    //attroff(A_BOLD);
	
	delwin(subscrn);
}