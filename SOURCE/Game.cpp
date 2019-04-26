/******************************************************************
** Program name: Game.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/25/2019
** Description: Source file for Game class "out-of-class" function
**				definitions. The Game class is the maintainer and 
**				executer of the game state and progression.
*******************************************************************/

Game::Game(bool isTwoPlayer, int gameMode) : 
	isTwoPlayer(isTwoPlayer), gameMode(gameMode) {
		//Create a random starting World
		int worldSelector = rand() % 3;
		if(worldSelector == 0)
			world = new Water();
		else if(worldSelector == 1)
			world = new Land();
		else
			world = new Space();
	}

int Game::playGame() {
	
	int userInput = 0;
		
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
			while (cube.lives > 0 && ch != 27 || ch != KEY_END) {
				userInput = getch();
				//if(omp_test_lock(&lockPos)) {
				if(!lock1) {
					if(userInput == KEY_UP && cube.getPosX() > 0) {
						cube.decPosX();
					}
					//Last line is reserved for timer and score display
					else if(userInput == KEY_DOWN && cube.getPosX() < LINES - 1) {
						cube.incPosX();
					}
					else if(userInput == KEY_RIGHT && cube.getPosY() < COLS) {
						cube.incPosY();
					}
					else if(userInput == KEY_LEFT && cube.getPosY() > 0) {
						cube.decPosY();
					}
					lock1 = 1;
				}
			}
		}
		//Thread (2) for updating timer & score display
		#pragma omp section
		{
			string output; ostringstream timeDisplay, scoreDisplay;
			while (cube.lives > 0 && ch != 27 || ch != KEY_END) {
				if(time < omp_get_wtime()) {
					timeDisplay.clear();
					timeDisplay << time / 3600 << ":" << time / 60 
						 << ":" << (time / 60) % 60;
				}
				scoreDisplay.clear();
				scoreDisplay << score;
				output = string(timeDisplay.c_str()) + 
						 string(scoreDisplay.c_str());
				mvaddr(LINES - 1, COLS - output.length() + 10, output.c_str());
				refresh();
			}
		}
		//Thread (3) for game engine
		#pragma omp section
		{
			int scrollRate = gameMode * 200;
			double lastScrollime = numeric_limits<double>::max();
			while (cube.lives > 0 && ch != 27 || ch != KEY_END) {
				if(time - lastScrollTime > scrollRate) 
					world.scroll();
				if(deathCheck()) {
					//Death Sequence
				}
				else {
					cube.paint();
					lock1 = 0;
				}
				if(time - lastRefreshTime > REFRESH_RATE) {
					world.refresh();
				}
			}
		}
	}
}


// References
// https://stackoverflow.com/questions/2396430/how-to-use-lock-in-openmp
