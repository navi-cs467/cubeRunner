/******************************************************************
** Program name: Game.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Source file for Game class "out-of-class" function
**				definitions. The Game class is the maintainer and 
**				executer of the game state and progression.
*******************************************************************/

#include "../HEADER/Game.hpp"

Game::Game(int gameMode, bool isTwoPlayer) : 
	isTwoPlayer(isTwoPlayer), gameMode(gameMode) {
		
		/* //Allocate memory for board
		board = new char*[LINES];
		for(int i = 0; i < LINES; i++)
			board[i] = new char[COLS];
		
		//Print space characters on board
		for(int i = 0; i < LINES - 1; i++)
			for(int j = 0; j < COLS; j++)
				board[i][j] = ' '; */
		
		//Create a random starting World
		int worldSelector = rand() % 3;
		//if(worldSelector == 0)
		if(1)
			world = new Water(gameMode, isTwoPlayer);
		/* else if(worldSelector == 1)
			world = new Land(gameMode, isTwoPlayer);
		else
			world = new Space(gameMode, isTwoPlayer); */
		
		//move(3, 4);
		//printw("%d %d %s", board, 54, "testing_again\n\n"); refresh();
		
		//usleep(100000000);
}

int Game::playGame() {
	
	int userInput = 0, score = 0;
		
	//Set number of omp threads
	omp_set_num_threads(3);
	
	//Lock needed so position cannot be changed
	//in such rapid succession that some positions
	//are not checked for death condition.
	//omp_lock_t lockPos;
	
	//Initialize lock
	//omp_init_lock(&lockPos);
	//** When lock1 = 0, thread (1) can update cube position
	// ** When lock1 = 1, thread (1) can NOT update cube position
	int lock1 = 0;
	
	#pragma omp parallel sections shared(userInput, lock1)
	{
		//Thread (1) for updating userInput and cube position
		#pragma omp section
		{
			while (/* cube.lives > 0 && */ userInput != 27 || 
										   userInput != KEY_END) {
				userInput = getch();
				//if(omp_test_lock(&lockPos)) {
				/* if(!lock1) {
					if(userInput == KEY_UP && cube.getPosX() > 0) {
						cube.decPosX();
					}
					//Last line is reserved for timer and score display
					else if(userInput == KEY_DOWN && cube.getPosX() <= world->bottomRow) {
						cube.incPosX();
					}
					else if(userInput == KEY_RIGHT && cube.getPosY() < COLS) {
						cube.incPosY();
					}
					else if(userInput == KEY_LEFT && cube.getPosY() > 0) {
						cube.decPosY();
					}
					lock1 = 1;
				} */
			}
		}
		//Thread (2) for updating timer & score display
		#pragma omp section
		{
			int time = static_cast<int>(omp_get_wtime());
			string output; ostringstream timeDisplay, livesDisplay, scoreDisplay;
			/* while (// cube.lives > 0 &&
					userInput != 27 || 
					userInput != KEY_END) {
				if(time < static_cast<int>(omp_get_wtime())) {
					time = static_cast<int>(omp_get_wtime());
					timeDisplay.clear();
					timeDisplay << "Time: " << time / 3600 << ":" << time / 60 
						 << ":" << (time / 60) % 60;
				}
				//lifeDisplay.clear();
				//lifeDisplay << "Lives: " << cube.numLives << "   ";
				scoreDisplay.clear();
				scoreDisplay << score << "   ";
				output = string(timeDisplay.str().c_str())  + "   " +
						 string(scoreDisplay.str().c_str()) + "   ";
				mvaddstr(LINES - 1, COLS - output.length() + 10, output.c_str());
				refresh();
			} */
		}
		//Thread (3) for game engine
		#pragma omp section
		{
			//Establish scrollRate & moveRate,
			//based on game difficulty (gameMode)
			double scrollRate, moveRate;
			if(gameMode == 1) {
				scrollRate = 0.35;
			}
			else if(gameMode == 2) {
				scrollRate = 0.75;
			}
			else {
				scrollRate = .25;
			}
			moveRate = scrollRate / 4.;
			
			//Establish the interval (secs) at which new 
			//Obstacles will be populated offscreen, based
			//on scrollRate.
			//double newObsInterval = COLS / (1./scrollRate);
			
			double lastScrollTime = omp_get_wtime(),
				   lastMoveTime = omp_get_wtime(),
				   lastRefreshTime = omp_get_wtime(),
				   lastNewObsTime = omp_get_wtime();
			int statsTime, startTime, scrollCount = 0,
				seconds = 0, minutes = 0, hours = 0;
			bool startTimeLogged = false;
			string output; ostringstream timeDisplay, livesDisplay, scoreDisplay;
			while (/* cube.lives > 0 && */ userInput != 27 || 
										   userInput != KEY_END) {
				
				/* if(deathCheck()) {
					//Death Sequence
				} */
				/* else {
					cube.paint();
					lock1 = 0;
				} */
				if(omp_get_wtime() - lastRefreshTime > REFRESH_RATE) {
					lastRefreshTime = omp_get_wtime();
					world->renderWorld();
				}
				
				if(omp_get_wtime() - lastScrollTime > scrollRate) {
					lastScrollTime = omp_get_wtime();
					world->scroll_();
					//if(typeid(*world) != typeid(Space))
						if(scrollCount == COLS) {
							world->loadOSObs();
							world->loadOSMCs();
						}
					if(world->getMiniCubes().size() 
							< (gameMode * NUM_MCS_HARD) / 2)
						world->initMiniCubes(1);		
					if(scrollCount == COLS) scrollCount = 0;
					else scrollCount++;
				}
				
				if(omp_get_wtime() - lastMoveTime > moveRate) {
					lastMoveTime = omp_get_wtime();
					world->moveObs();
				}
				
				//Render time, life count, and score display every second
				if(!startTimeLogged) {
					statsTime = startTime = static_cast<int>(omp_get_wtime());
					startTimeLogged = true;
				}
				string output; ostringstream timeDisplay, livesDisplay, scoreDisplay;
				if(statsTime < static_cast<int>(omp_get_wtime())) {
					statsTime = static_cast<int>(omp_get_wtime());
					seconds++;
					if(seconds == 60) {
						seconds = 0;
						minutes++;
					}
					if(minutes == 60) {
						minutes = 0;
						hours++;
					}
					
					//Time display
					timeDisplay.clear();
					if(hours < 10)
						timeDisplay << "Time: " << "0" << hours << ":";
					else
						timeDisplay << "Time: " << hours << ":";
					if(minutes < 10)
						timeDisplay << "0" << minutes << ":";
					else
						timeDisplay << minutes << ":";
					if(seconds < 10)
						timeDisplay << "0" << seconds;
					else
						timeDisplay << seconds;
					
					//Life count display
					livesDisplay.clear();
					//livesDisplay << "Lives: " << cube.numLives << "   ";
					livesDisplay << "Lives: " << 1;
					
					//Score display
					scoreDisplay.clear();
					scoreDisplay << "Score: " << score;
					output = string(timeDisplay.str().c_str())  + "   " +
							 string(scoreDisplay.str().c_str()) + "   " +
							 string(livesDisplay.str().c_str());
					attron(COLOR_PAIR(YELLOW_BLACK));
					mvhline(LINES - 1, 0, ' ', COLS);
					mvaddstr(LINES - 1, COLS - output.length() - 10, output.c_str());
					refresh();
				}
			}
		}
	}
	return score;
}
	
char **Game::board = nullptr;


// References
// https://stackoverflow.com/questions/2396430/how-to-use-lock-in-openmp
