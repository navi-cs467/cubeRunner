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
		
		score = 0;
		
		/* //Allocate memory for board
		board = new char*[LINES];
		for(int i = 0; i < LINES; i++)
			board[i] = new char[COLS];
		
		//Print space characters on board
		for(int i = 0; i < LINES - 1; i++)
			for(int j = 0; j < COLS; j++)
				board[i][j] = ' '; */
		
		
		if(!isTwoPlayer) world = new Water(gameMode, isTwoPlayer);
		
		else world = new Water();		//"Blank" world if running as client
}

int Game::playGame(char host[], int port) {
	
	int userInput = 0;
		
	//Set number of omp threads
	omp_set_num_threads(2);
	
	//Lock needed so position cannot be changed
	//in such rapid succession that some positions
	//are not checked for death condition.
	//omp_lock_t lock1;
	
	//Initialize lock
	//omp_init_lock(&lock1);
	
	#pragma omp parallel sections shared(userInput)
	{
		//Thread (1) for updating userInput and cube position
		#pragma omp section
		{
			while (/* cube.lives > 0 && */ userInput != 27 || 
										   userInput != KEY_END ||
										   userInput != 'q' ||
										   userInput != 'Q') {
				
				userInput = getch();
				/* if(userInput == KEY_UP && cube.getPosX() > 0) {
					cube.decPosX();
					cube.setDirection(up);
				}
				//Last line is reserved for timer and score display
				else if(userInput == KEY_DOWN && cube.getPosX() < world->getBottomRow()) {
					cube.incPosX();
					cube.setDirection(down);
				}
				else if(userInput == KEY_RIGHT && cube.getPosY() < COLS) {
					cube.incPosY();
					cube.setDirection(right);
				}
				else if(userInput == KEY_LEFT && cube.getPosY() > 0) {
					cube.decPosY();
					cube.setDirection(left);
				}
				
				deathFlag = cube.checkDeath();	//checkDeath should "cube.setLives(cube.getLives() - 1)" if player token hits Obstacle - Returns "true" if death has occurred
				cube.checkScore();				//checkScore should increase score by number of miniCubes contacted for this move.
												//Should also "consume miniCubes contacted, i.e. remove them from screen (i.e. erase them from set<pair<int,int>> world->miniCubes)
				*/
			}
		}
		
		//Thread (2) for game engine
		#pragma omp section
		{	
			//Single player game engine
			if(!isTwoPlayer) {
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
				
				//Timer variables
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
					
					if(omp_get_wtime() - lastRefreshTime > REFRESH_RATE) {
						lastRefreshTime = omp_get_wtime();
						world->renderWorld();
						//cube.paint();
					}
					
					if(omp_get_wtime() - lastScrollTime > scrollRate) {
						lastScrollTime = omp_get_wtime();
						world->scroll_();
						//if(typeid(*world) != typeid(Space))
							if(scrollCount == COLS) {
								world->loadOSObs();
								world->loadOSMCs();
							}
						
						//Repopulate miniCubes if too many have been
						//consumed by moving obstacles according to this
						//threshold
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
			
			//Client for multiplayer
			else {
				
				// Send connection request to cubeRunnerServer using parameters "host" and "port"
				// (Optional) Send gameMode with request
				
				// Receive int message
				// If message == 0; wait for another message to indicate player 2 has connected
					
					//Receive message; player 2 has connected (game can start)
					
					//(Optional) If message == gameMode; game difficulties match (game can start)
					//(Optional) If message != gameMode; game difficulties do not match 
					//									 (display error message and "continue" to restart loop, 
					//									 or display ncurses window informing player the
					//									 the easier of the two gameModes will be used, and
					//									 to press any key to continue.)
					
					// (Optional - if display window is used to inform player easier game mode will be used) send message to cubeRunnerServer once character has been input
					// (Optional - if display window is used to inform player easier game mode will be used) receive message back that the other player has also confirmed message (game can start)
					
				// Else if message == 1 (or optionally gameMode); player one already connected (game can start)
				
				//  // (Optional) If message == gameMode; game difficulties match (game can start)
					// (Optional) If message != gameMode; game difficulties do not match 
					//									 (display error message and "continue" to restart loop, 
					//									 or display ncurses window informing player the
					//									 the easier of the two gameModes will be used, and
					//									 to press any key to continue.)
					
					// (Optional - if display window is used to inform player easier game mode will be used) send message to cubeRunnerServer once character has been input
					// (Optional - if display window is used to inform player easier game mode will be used) receive message back that the other player has also confirmed message (game can start)
				
				//For Time Display
				int seconds, minutes, hours;
				string output; ostringstream timeDisplay, livesDisplay, scoreDisplay;
				
				//Pseudocode variables... change as desired
				int int_1, int_2, int_3, int_4, int_5, int_6;
				
				while (/* cube.lives > 0 && */ userInput != 27 || 
											   userInput != KEY_END) {
					
						/**** RECEIVE DEATH FLAG ****/
						//RECEIVE int_1			
						//If int_1 == 1:			//Death happened
						//	  RECEIVE int_2
						//	  If int_2 == 1:		//Game Over
						//		  CLOSE connection
						//	  	  /* game_over_animation(); */
						//		  break
						//	  Else If int_2 == 0	//Death But No Game Over
						//		  /* death_animation(); */
						//	      // (Optional ?) SEND: confirmation		//Probably not optional, server needs to wait for death animation
						//Else:
						//	  // (Optional ?) SEND: confirmation			//No Death
						/**** RECEIVE DEATH FLAG ****/
						
						/**** RECEIVE CUBE DATA ****/
						// RECEIVE int_1, int_2, int_3		//cube position x, y, and life count?
						// cube->setPosX(int_1); cube->setPosY(int_2); cube->setNumLives(int_3);
						// (Optional ?) SEND: confirmation
						/**** END RECEIVE CUBE DATA ****/
						
						//Now we tear down the entire world, to then rebuild... (Really Hoping we can get away with this performance-wise)
						//(Need to do this before we receive new world indicator in case world gets deleted)
						
						//Delete all Obstacles
						for(list<Obstacle*>::iterator it = world->getObstacles().begin();
							it != world->getObstacles().begin(); it++) {
								delete *it;
						}
							
						/**** RECEIVE NEW WORLD INDICATOR AND (IF APPLICABLE) TYPE  ****/
						//RECEIVE int_1			//isNewWorld flag
						//If int_1 == 1:		//If world transition has occurred, or first loop iteration	  
							  delete world;
							  // (Optional ?) SEND: confirmation		
						//	  RECEIVE int_2:	//World type
							  	  if(int_2 == 1) {
									  world = new Water();
									  /* world_transition_animation(); */
								  }
								  //else if(int_2 == 2) {
								  //  world = new Land();
									  /* world_transition_animation(); */
								  //}
								  //else {
								  //  world = new Space();
									  /* world_transition_animation(); */
								  //}
								  // (Optional ?) SEND: confirmation		//Probably not optional, need to wait for world transition animation
						
						//Else If int == 0:		//If no world transition, we need to clear these containers
							world->getObstacles().clear();
							world->getObsCoords().clear();
							world->getMiniCubes().clear();
							// (Optional ?) SEND: confirmation
						/**** END RECEIVE NEW WORLD INDICATOR AND (IF APPLICABLE) TYPE  ****/
						
						/**** RECEIVE ONSCREEN OBSTACLES  ****/
						//RECEIEVE int_1		//number of (onscreen) Obstacles
						// (Optional ?) SEND: confirmation
						
						//Loop to rebuild Obstacles
						for(int i = 0; i < int_1; i++) {
							//RECEIVE int_2, int_3, int_4, int_5, int_6		// type of Obs, posX, posY, gt, gts(not strictly necessary, but used as convenience)
							if(int_2 == 1)
								world->getObstacles().push_back(new Seaweed(int_3, int_4, int_5, int_6));
							else if(int_2 == 2)
								world->getObstacles().push_back(new Coral(int_3, int_4, int_5, int_6));
							else if(int_2 == 3)
								world->getObstacles().push_back(new Shark(int_3, int_4, int_5, int_6));
							else if(int_2 == 4)
								world->getObstacles().push_back(new Octopus(int_3, int_4, int_5, int_6));
							/* else if(int_2 == 5)
								world->getObstacles().push_back(new Tree(int_3, int_4, int_5, int_6));
							else if(int_2 == 6)
								world->getObstacles().push_back(new Rock(int_3, int_4, int_5, int_6));
							else if(int_2 == 7)
								world->getObstacles().push_back(new Bird(int_3, int_4, int_5, int_6));
							else if(int_2 == 8)
								world->getObstacles().push_back(new Bat(int_3, int_4, int_5, int_6));
							else if(int_2 == 9)
								world->getObstacles().push_back(new Asteroid(int_3, int_4, int_5, int_6));
							else if(int_2 == 10)
								world->getObstacles().push_back(new Planet(int_3, int_4, int_5, int_6));
							else if(int_2 == 11)
								world->getObstacles().push_back(new Comet(int_3, int_4, int_5, int_6));
							else if(int_2 == 12)
								world->getObstacles().push_back(new Spaceship(int_3, int_4, int_5, int_6));	 */
							
							// (Optional ?) SEND: confirmation
						}
						/**** END RECEIVE ONSCREEN OBSTACLES  ****/
						
						/**** RECEIVE OBSCOORDS  ****/
						//RECEIEVE int_1		//number of obsCoords
						// (Optional ?) SEND: confirmation	
						for(int i = 0; i < int_1; i++) {
							//RECEIVE int_2, int_3			//obCoord x & y
							world->getObsCoords().insert(make_pair(int_2, int_3));
							// (Optional ?) SEND: confirmation
						}
						/**** END RECEIVE OBSCOORDS  ****/
						
						/**** RECEIVE MINICUBES  ****/
						//RECEIEVE int_1		//number of miniCubes
						// (Optional ?) SEND: confirmation	
						for(int i = 0; i < int_1; i++) {
							//RECEIVE int_2, int_3			//miniCube x & y
							world->getMiniCubes().insert(make_pair(int_2, int_3));
							// (Optional ?) SEND: confirmation
						}
						/**** END RECEIVE MINICUBES  ****/
						
						world->renderWorld();
						//cube.paint();
						
						/**** RECEIVE TIME  ****/
						//RECEIVE int_1 into hours
						// (Optional ?) SEND: confirmation
						
						//RECEIVE int_1 into minutes
						// (Optional ?) SEND: confirmation
						
						//RECEIVE int_1 into seconds
						// (Optional ?) SEND: confirmation
						/**** END RECEIVE TIME  ****/
						
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
