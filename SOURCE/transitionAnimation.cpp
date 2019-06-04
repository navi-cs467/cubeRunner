/*****************************************************
** Program name: transitionAnimation.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/23/2019
** Description: Source file for transitionAnimation function.
*****************************************************/

#include "../HEADER/transitionAnimation.hpp"

//Introduction sequence (plays when game starts)
void transitionAnimation(const char* fileName, 
						 int graphicWidth, 
						 int graphicHeight,
						 int backgroundColor,
						 int startingSeedColor,
						 int promptColor)
{ 
	clear();  //Curses clear-screen call
	
	// paint screen background color
	attron(COLOR_PAIR(backgroundColor));
    for (int y = 0; y < LINES; y++) {
        mvhline(y, 0, ' ', COLS);
    }
	refresh();
   
    //Turn on bold attribute
    attron(A_BOLD);
	
	int startingCol = (COLS - graphicWidth)/2, 
		startingRow = (LINES - graphicHeight)/2;
	WINDOW *subscrn = newwin(graphicHeight, graphicWidth, 
							 startingRow, startingCol);
	
	// paint subscreen background color
	wattron(subscrn, COLOR_PAIR(backgroundColor)); wattron(subscrn, A_BOLD);
    for (int y = 0; y < graphicHeight; y++) {
        mvwhline(subscrn, y, 0, ' ', graphicWidth);
    }
	wrefresh(subscrn);
   
    //Load transitionAnimation graphic into cmdoutlinesGraphics from file
    loadGraphic(fileName);
	
	//Using non-blocking read() on STDIN_FILENO so animation
	//can be interrupted with keystroke, but will play as
	//intended (and require keystroke after initial graphic
	//animation completes) otherwise.
	fcntl(STDIN_FILENO, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
   
    omp_set_num_threads(2);
    int color, row;
	bool inputInterrupt = false, animationRunning = true, 
		 inputReceived = false, animationCompleted = false; 
	char c;
	//Multi-threading is used here so the user can interrupt the
	//animation and skip directly to the menu
	#pragma omp parallel sections shared(subscrn, inputReceived, \
	row, COLS, cmdoutlinesGraphics)
	{
		#pragma omp section
		{
			//Paint the rows to the screen
			for (row = 0, color = rand() % 6 + startingSeedColor; 
					!inputInterrupt && row < graphicHeight - 2;
					row++, color++) {  
			   if(color == startingSeedColor + 6) color = startingSeedColor;		//Cycle to first index when necessary
			   //Change color
			   wattron(subscrn, COLOR_PAIR(color)); 
			   mvwaddwstr(subscrn, row, 0,
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
				
			if(!inputInterrupt) usleep(200 * 1000);	//Sleep for 200 milliseconds
			
			animationRunning = false;
			//attron(COLOR_PAIR(WHITE_BLACK));
			if(!inputInterrupt) animationCompleted = true;
		}
		
		#pragma omp section
		{
			while(animationRunning) {
				if(read(STDIN_FILENO, &c, sizeof(char)) > 0) {
					inputInterrupt = true;
					break;
				}
			}
		}
	}
	
	//Print blinking continue prompt (if animation was not skipped)
	//attron(A_BLINK);	//Doesn't work, (at least for some terminals)
	omp_set_num_threads(2); 
	double time = omp_get_wtime(); 
	bool visible = false; 
	if(animationCompleted){
		//Set to STDIN_FILENO back to blocking if initial
		//graphic transitionAnimation plays to completion.
		fcntl(STDIN_FILENO, F_SETFL, fcntl(0, F_GETFL) & ~O_NONBLOCK);
		
		#pragma omp parallel sections shared(subscrn, inputReceived, \
		row, COLS, cmdoutlinesGraphics)
		{
			#pragma omp section
			{
				while(!inputReceived) {
					if(time + 0.6 < omp_get_wtime() && visible == false){
						time = omp_get_wtime();
						wattron(subscrn, COLOR_PAIR(promptColor));
						mvwaddwstr(subscrn, row + 1, 0, 
							cmdoutlinesGraphics[row + 1].c_str());
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
			
			#pragma omp section
			{
				if(!inputReceived) getch();
				inputReceived = true;
			}
		}
	}

	//Makes sure nothing erroneous is printed next time cmdoutlinesGraphics
	//is used.
	for(vector<wstring>::iterator it = cmdoutlinesGraphics.begin();
		it < cmdoutlinesGraphics.end(); it++)
			*it = L"";
	
	//Set to STDIN_FILENO back to non-blocking for second
	//animation sequence.
	fcntl(STDIN_FILENO, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
	
	omp_set_num_threads(2); animationRunning = true;
	//Multi-threading is used here so the user can interrupt the
	//animation and skip directly to the menu
	#pragma omp parallel sections shared(inputReceived)
	{
		#pragma omp section
		{
			//Paint in alternating color perimeters converging into middle of screen
			for(int i = LINES, j = COLS, color = rand() % 6 + 9, z = 0; 
				!inputInterrupt && i >= LINES/2 && j >= COLS/2; 
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
				
				usleep(100 * 1000);	//Sleep for 100 milliseconds for animation effect
				//usleep(1 * 1000);	//Sleep for 1 milliseconds (testing only)
				refresh();
			}
			
			animationRunning = false;
		}
		
		#pragma omp section
		{
			while(animationRunning) {
				if(read(STDIN_FILENO, &c, sizeof(char)) > 0) {
					inputInterrupt = true;
					break;
				}
			}
		}
	}

	//attron(COLOR_PAIR(0));	//Reset to default
    //attroff(A_BOLD);
	
	//Turn off non-blocking io on STDIN_FILENO
	fcntl(STDIN_FILENO, F_SETFL, fcntl(0, F_GETFL) & ~O_NONBLOCK);
	
	delwin(subscrn);
}

// References
// https://stackoverflow.com/questions/26807623/interrupt-ncurses-getch-on-incoming-signal
// https://stackoverflow.com/questions/717572/how-do-you-do-non-blocking-console-i-o-on-linux-in-c