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

		if(!isTwoPlayer) world = new Water(gameMode, isTwoPlayer);

		else world = new Water(isTwoPlayer);		//"Blank" world if running as client

		if(gameMode == EASY) cube = new Cube(world, 5);
		else if(gameMode == NORMAL) cube = new Cube(world, 4);
		else if(gameMode == HARD) cube = new Cube(world, 3);

}

int Game::playGame(char host[], char port[]) {

	int userInput = 0;

	//Set number of omp threads
	omp_set_num_threads(2);

	//Lock needed so position cannot be changed
	//in such rapid succession that screen rendering
	//cannot keep up.
	omp_lock_t userInputLock;

	//Initialize lock
	omp_init_lock(&userInputLock);

	bool deathFlag = false;
	bool isConnected = false;
	int playerNum = 0;
	int socketFD = -1;
	bool hasTerminated = false;

	#pragma omp parallel sections shared(userInput, deathFlag, isConnected, playerNum, socketFD, hasTerminated)
	{
		//Thread (1) for updating userInput and cube position
		#pragma omp section
		{
			while ( /* cube->getCubeLives() > 0 && */  userInput != 27 &&
									  userInput != KEY_END &&
									  userInput != 'q' &&
									  userInput != 'Q' && !hasTerminated) {


				userInput = getch();

				//Single Player
				if(!isTwoPlayer && !deathFlag) {

					//Block the move update if render is in progress
					omp_set_lock(&userInputLock);

					if((userInput == KEY_UP || userInput == '8' || userInput == 'w')
							&& cube->getCubeCoords()[0][0] > 0) {
						cube->updateCubePosition(0, 0, 0, 1);
						cube->setCubeDirection(up);
					}
					else if((userInput == KEY_DOWN || userInput == '2' || userInput == 's') &&
								cube->getCubeCoords()[15][0] < world->getBottomRow()) {
						cube->updateCubePosition(0, 0, 1, 0);
						cube->setCubeDirection(down);
					}
					else if((userInput == KEY_RIGHT || userInput == '6' || userInput == 'd') &&
								cube->getCubeCoords()[15][1] < COLS) {
						cube->updateCubePosition(1, 0, 0, 0);
						cube->setCubeDirection(right);
					}
					else if((userInput == KEY_LEFT || userInput == '4' || userInput == 'a') &&
							cube->getCubeCoords()[0][1] > 0) {
						cube->updateCubePosition(0, 1, 0, 0);
						cube->setCubeDirection(left);
					}
					else if(userInput == 7 && cube->getCubeCoords()[0][0] > 0 &&
											  cube->getCubeCoords()[0][1] > 0) {
						cube->updateCubePosition(0, 1, 0, 1);
						cube->setCubeDirection(left_up);
					}
					else if(userInput == 1 &&
							cube->getCubeCoords()[15][0] < world->getBottomRow() &&
							cube->getCubeCoords()[0][1] > 0) {
						cube->updateCubePosition(0, 1, 1, 0);
						cube->setCubeDirection(left_down);
					}
					else if(userInput == 9 && cube->getCubeCoords()[0][0] > 0 &&
											  cube->getCubeCoords()[15][1] < COLS) {
						cube->updateCubePosition(1, 0, 0, 1);
						cube->setCubeDirection(right_up);
					}
					else if(userInput == 3 &&
							cube->getCubeCoords()[15][0] < world->getBottomRow() &&
							cube->getCubeCoords()[15][1] < COLS) {
						cube->updateCubePosition(1, 0, 1, 0);
						cube->setCubeDirection(right_down);
					}
					else if(userInput == 32) {
						cube->fireShot();
					}

					//Unlock the lock after movement update
					omp_unset_lock(&userInputLock);

				}

				/***** BEGIN Client for multiplayer (SEND) *****/
				//NOTE: If the player inputs movement commands faster than the
				//	  	server can handle, the "extra commands" go unprocessed,
				//		since the code below blocks at "RECEIVE confirmation",
				//		then the input buffer is flushed
				else if (!deathFlag && isConnected && !hasTerminated) {
					char messageToSend[256];
					memset(messageToSend, '\0', sizeof(messageToSend));
					char confirm[256];
					memset(confirm, '\0', sizeof(confirm));

					if(playerNum == 1) {
						if(userInput == KEY_UP) {
							// SEND: KEY_UP
							sprintf(messageToSend, "%d", userInput);
							sendMessage_C(socketFD, messageToSend);
							// RECEIEVE confirmation
							receiveMessage_C(socketFD, confirm);

							fflush(stdin);		//This may not be portable and/or not work as intended, but let's hope that's not the case
						}
						else if(userInput == KEY_DOWN) {
							// SEND: KEY_DOWN
							sprintf(messageToSend, "%d", userInput);
							sendMessage_C(socketFD, messageToSend);
							// RECEIEVE confirmation
							receiveMessage_C(socketFD, confirm);
							fflush(stdin);		//This may not be portable and/or not work as intended, but let's hope that's not the case
						}
						else if(userInput == 32) {
							// SEND: 32 (SPACE_BAR - For cube->fireShot())
							sprintf(messageToSend, "%d", userInput);
							sendMessage_C(socketFD, messageToSend);
							// RECEIEVE confirmation
							fflush(stdin);		//This may not be portable and/or not work as intended, but let's hope that's not the case
						}
						else if(userInput == 27 ||
								userInput == KEY_END ||
								userInput == 'q' ||
								userInput == 'Q') {
							// SEND: q
							userInput = 'q';
							sprintf(messageToSend, "%d", userInput);
							sendMessage_C(socketFD, messageToSend);
							// RECEIEVE confirmation
							receiveMessage_C(socketFD, confirm);
							// RECEIVE score into score (as in, into this->score)
							memset(confirm, '\0', sizeof(confirm));

							receiveMessage_C(socketFD, confirm);
							cube->setCubeScore(atoi(confirm));
							// CLOSE CONNECTION
							close(socketFD);

							hasTerminated = true;
							break;
						}
					}
					else if(playerNum == 2){
						if(userInput == KEY_LEFT) {
							// SEND: KEY_LEFT
							sprintf(messageToSend, "%d", userInput);
							sendMessage_C(socketFD, messageToSend);
							// RECEIEVE confirmation
							fflush(stdin);		//This may not be portable and/or not work as intended, but let's hope that's not the case
						}
						else if(userInput == KEY_RIGHT) {
							// SEND: KEY_RIGHT
							sprintf(messageToSend, "%d", userInput);
							sendMessage_C(socketFD, messageToSend);
							// RECEIEVE confirmation
							fflush(stdin);		//This may not be portable and/or not work as intended, but let's hope that's not the case
						}
						else if(userInput == 32) {
							// SEND: 32 (SPACE_BAR - For cube->fireShot())
							sprintf(messageToSend, "%d", userInput);
							sendMessage_C(socketFD, messageToSend);
							// RECEIEVE confirmation
							fflush(stdin);		//This may not be portable and/or not work as intended, but let's hope that's not the case
						}
						else if(userInput == 27 ||
								userInput == KEY_END ||
								userInput == 'q' ||
								userInput == 'Q') {
							// SEND: q
							userInput = 'q';
							sprintf(messageToSend, "%d", userInput);
							sendMessage_C(socketFD, messageToSend);
							// RECEIEVE confirmation
							receiveMessage_C(socketFD, confirm);
							// RECEIVE score into score (as in, into this->score)
							memset(confirm, '\0', sizeof(confirm));

							receiveMessage_C(socketFD, confirm);
							cube->setCubeScore(atoi(confirm));

							// CLOSE CONNECTION
							close(socketFD);

							//set the FLAG
							hasTerminated = true;
							break;
						}
					}
				}
				/***** END Client for multiplayer (SEND) *****/
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
				if(gameMode == HARD) {
					scrollRate = SCROLL_RATE_HARD;
				}
				else if(gameMode == NORMAL) {
					scrollRate = SCROLL_RATE_NORMAL;
				}
				else {
					scrollRate = SCROLL_RATE_EASY;
				}

				moveRate = scrollRate / MOVE_RATE_DIVISOR;

				//Timer variables
				double lastScrollTime = omp_get_wtime(),
					   lastMoveTime = omp_get_wtime(),
					   lastRefreshTime = omp_get_wtime(),
					   lastNewObsTime = omp_get_wtime(),
					   lastShotTime = omp_get_wtime();
				int statsTime, startTime, scrollCount = 0,
					seconds = 0, minutes = 0, hours = 0;
				bool startTimeLogged = false;
				string output; ostringstream timeDisplay, livesDisplay, scoreDisplay;

				//Initial Time display
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

				//Initial Life count display
				livesDisplay.clear();
				livesDisplay << "Lives: " << cube->getCubeLives() << "   ";
				/* if(gameMode == EASY) livesDisplay << "Lives: " << 5;
				else if(gameMode == NORMAL) livesDisplay << "Lives: " << 4;
				else if(gameMode == HARD) livesDisplay << "Lives: " << 3; */

				//Initial Score display
				scoreDisplay.clear();
				scoreDisplay << "Score: " << score;
				output = string(timeDisplay.str().c_str())  + "   " +
						 string(scoreDisplay.str().c_str()) + "   " +
						 string(livesDisplay.str().c_str());
				attron(COLOR_PAIR(YELLOW_BLACK));
				mvhline(LINES - 1, 0, ' ', COLS);
				mvaddstr(LINES - 1, COLS - output.length() - 10, output.c_str());
				refresh();

				//Main game engine loop
				while ( userInput != 27 &&
						userInput != KEY_END &&
						userInput != 'q' &&
						userInput != 'Q') {

					if(omp_get_wtime() - lastRefreshTime > REFRESH_RATE) {
						lastRefreshTime = omp_get_wtime();

						//Movement updates must wait until current render completes
						omp_set_lock(&userInputLock);

						// World transition if cube->transitionCount
						//reaches TRANSITION_SCORE_INTERVAL

						/* if(cube->getTransitionScore() >= TRANSITION_SCORE_INTERVAL) {

							//Delete all Obstacles
							for(list<Obstacle*>::iterator it = world->getObstacles().begin();
							it != world->getObstacles().begin(); it++) {
								delete *it;
							}

							//Clear all containers
							world->getObstacles().clear();
							world->getObsCoords().clear();
							world->getMiniCubes().clear();

							//Create new world
							if(typeid(*world) == typeid(Water))
								world = new Land(gameMode, isTwoPlayer);
							else if(typeid(*world) == typeid(Land))
								world = new Space(gameMode, isTwoPlayer);
							else if(typeid(*world) == typeid(Space))
								world = new Water(gameMode, isTwoPlayer);

							//If score is less than 3000, increase scroll and move time intervals by a constant
							//(This is the point at which all three worlds have been cycled 3 times each,
							// and the speeds are capped.)
							//scrollRate *= SCROLL_MOVE_DECREASE_RATE;
							//moveRate *= SCROLL_MOVE_DECREASE_RATE;

							//Reset cubes position to left-middle starting point
							cube->reset();

							//Reset transitionCount
							cube->resetTransitionScore();
						} */

						//Check for death
						cube->checkCubeCollision(world);
						deathFlag = cube->getCubeIsDead();

						//Render World
						world->renderWorld(cube);

						//Render Cube
						cube->drawCube();
						//Death animation if death occurred
						if(deathFlag) {
							//Allow user to confirm death
							omp_unset_lock(&userInputLock);
							cube->drawCubeDeath(&userInput);
							//Relock userInputLock
							omp_set_lock(&userInputLock);
						}

						//Game Over animation and break if game over occurred
						/* if(cube->getCubeLives() == 0) {
							transitionAnimation("gameOver.txt");
							//Delete all Obstacles
							for(list<Obstacle*>::iterator it = world->getObstacles().begin();
							it != world->getObstacles().begin(); it++) {
								delete *it;
							}
							//Delete cube
							delete cube;
							//Delete world
							delete world;
							break;
						} */

						//Reset player if death occurred (but no game over)
						if(deathFlag) world->resetPlayer(cube);

						//Reset death flag
						deathFlag = false;

						string output; ostringstream timeDisplay, livesDisplay, scoreDisplay;

						//Render Time display
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

						//Render Life count display
						livesDisplay.clear();
						livesDisplay << "Lives: " << cube->getCubeLives() << "   ";
						/* if(gameMode == EASY) livesDisplay << "Lives: " << 5;
						else if(gameMode == NORMAL) livesDisplay << "Lives: " << 4;
						else if(gameMode == HARD) livesDisplay << "Lives: " << 3; */

						//Render Score display
						scoreDisplay.clear();
						scoreDisplay << "Score: " << cube->getCubeScore();
						output = string(timeDisplay.str().c_str())  + "   " +
								 string(scoreDisplay.str().c_str()) + "   " +
								 string(livesDisplay.str().c_str());
						attron(COLOR_PAIR(YELLOW_BLACK));
						mvhline(LINES - 1, 0, ' ', COLS);
						mvaddstr(LINES - 1, COLS - output.length() - 10, output.c_str());
						refresh();

						//Pending movement update can now be processed
						omp_unset_lock(&userInputLock);

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
						if(world->getMiniCubes().size() / 2
								< (NUM_MCS_EASY / gameMode))
							world->initMiniCubes(1);

						if(scrollCount == COLS) scrollCount = 0;
						else scrollCount++;
					}

					if(omp_get_wtime() - lastMoveTime > moveRate) {
						lastMoveTime = omp_get_wtime();
						world->moveObs();
					}

					//Shot moves 4 times as fast as Obstacles move if
					//moving horizontally, and 2 times as fast as Obstacles
					//move if moving vertically.
					if((cube->getShotDir() == up || cube->getShotDir() == down) &&
						omp_get_wtime() - lastShotTime > moveRate / 2) {
							lastShotTime = omp_get_wtime();
							cube->moveShot();
					}
					else if(cube->getShotDir() != up &&
							cube->getShotDir() != down &&
							omp_get_wtime() - lastShotTime > moveRate / 4) {
						lastShotTime = omp_get_wtime();
						cube->moveShot();
					}

					//Update time, life count, and score display every second
					if(!startTimeLogged) {
						statsTime = startTime = static_cast<int>(omp_get_wtime());
						startTimeLogged = true;
					}

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
					}
				}
			}

			//Client for multiplayer (RECEIEVE)
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

				clear();  // curses clear-screen call
				refresh();
				
				//Set LINES and COLS to minimum values
				LINES = MIN_WIN_HEIGHT;
				COLS = MIN_WIN_WIDTH;

				//connect to server
				socketFD = initSocket(host, port);

				if(DEBUG)
					move(5,5); printw("Connected..."); refresh();

				//hold server connection confirmation
				char message[256];
				memset(message, '\0', sizeof message);

				//receive message, either 0 or 1 depending on player 1 or 2
				receiveMessage_C(socketFD, message);

				if(DEBUG)
					move(6,5); printw("Received Player Indicator..."); refresh();

				//check for other player
				//if we received 0, we are connected but other player isn't
				//so we check for next confirmation which should tell us if gamemodes match or not
				if (strcmp(message,"0") == 0)
				{
					//send chosen game mode
					char gM[2];
					sprintf(gM, "%d", gameMode);
					sendMessage_C(socketFD, gM);
					if(DEBUG)
						move(7,5); printw("Sent GM Player 1...\n"); refresh();

					//check for other player and gamemode match
					memset(message, '\0', sizeof message);
					receiveMessage_C(socketFD, message);

					if(DEBUG)
						move(8,5); printw("Received Confirmation Other Player Now Connected & GM Match Indicator...\n"); refresh();

					//if we got to this point, player is player 1 so change the value
					playerNum = 1;
				}

				if ((strcmp(message,"1") == 0) && playerNum != 1)
				{
					//send chosen game mode
					char gM[2];
					sprintf(gM, "%d", gameMode);
					sendMessage_C(socketFD, gM);

					if(DEBUG)
						move(9,5); printw("Sent GM Player 2...\n"); refresh();

					//check for gamemode match
					memset(message, '\0', sizeof message);
					receiveMessage_C(socketFD, message);

					if(DEBUG)
						move(10,5); printw("Received GM Match Indicator...\n"); refresh();

					//if we got to this point, player is player 2 so change the value
					playerNum = 2;
				}

				//at this point, message holds gameMode result
				//gamemodes don't match
				if (strcmp(message,"2") == 0)
				{
					//display error message of some kind
					//maybe send acknowledgment if necessary
					//easiest game mode used automatically
				}

				//set isConnected to TRUE
				isConnected = TRUE;

				//For Time Display
				int seconds, minutes, hours;
				string output; ostringstream timeDisplay, livesDisplay, scoreDisplay;

				if(DEBUG)
					move(11,5); printw("Starting Game Loop..."); refresh();

				while (!hasTerminated) {

						//Pseudocode variables... change as desired
						int int_1, int_2, int_3, int_4, int_5, int_6; char earlyTerm[10];
						char scoreStr[256]; char gameData[256]; char sendConfirm[10];

						memset(earlyTerm, '\0', sizeof earlyTerm);
						memset(scoreStr, '\0', sizeof scoreStr);
						memset(gameData, '\0', sizeof gameData);
						memset(sendConfirm, '\0', sizeof sendConfirm);

						/**** RECEIVE (OTHER PLAYER) EARLY TERMINATION STATUS ****/

						// RECEIVE (10 bytes) into earlyTerm
						receiveMessage_C(socketFD, earlyTerm);

						// (Optional ?) SEND Confirmation
						// If earlyTerm == "ET"
						// 	  RECEIVE score
						//
						//	  Display ncurses sub-window informing player that other player has terminated early
						//    break;

						//other client has terminated the connection
						if (strcmp(earlyTerm, "ET") == 0)
						{
							// display message to user here ***

							//receive score
							receiveMessage_C(socketFD, scoreStr);
							cube->setCubeScore(atoi(scoreStr));

							// close connection
							close(socketFD);

							break;
						}

						/**** END RECEIVE (OTHER PLAYER) EARLY TERMINATION STATUS ****/

						/**** RECEIVE DEATH FLAG ****/
						//RECEIVE int_1
						receiveMessage_C(socketFD, gameData);
						int_1 = atoi(gameData);
						//If int_1 == 1:		//Death happened
						if (int_1 == 1)
						{
							//	  RECEIVE int_2
							memset(gameData, '\0', sizeof gameData);
							receiveMessage_C(socketFD, gameData);
							int_2 = atoi(gameData);

							//	  If int_2 == 1:		//Game Over
							if (int_2 == 1)
							{
						      //CLOSE connection
									close(socketFD);
						//	  	  /* animationTransition("GRAPHICS/gameOver.txt"); */
						      //Delete all Obstacles
									for(list<Obstacle*>::iterator it = world->getObstacles().begin();
										it != world->getObstacles().begin(); it++) {
										delete *it;
									}
									//Delete world
									delete world;
									//Delete cube
									delete cube;
								 	break;
							}

							//	  Else If int_2 == 0	//Death But No Game Over
							//		  //deathFlag = true;
							//	      // (Optional ?) SEND: confirmation		//Probably not optional, server needs to wait for death animation

							else if (int_2 == 0)
							{
								//reset deathFlag
								deathFlag = true;

								//send confirmation to server
								sprintf(sendConfirm, "%d", 1);
								sendMessage_C(socketFD, sendConfirm);
							}
						}
						//Else:
						//	  // (Optional ?) SEND: confirmation			//No Death
						/**** RECEIVE DEATH FLAG ****/

						/**** RECEIVE CUBE DATA ****/
						// RECEIVE char coords[CUBE_COORDS_HEGHT][CUBE_COORDS_WIDTH], \	//cube coords
						//		   char chars[CUBE_CHARS_HEGHT][CUBE_CHARS_WIDTH], \	//cube chars
						//		   int_3, int_4, int_5									//cube lives, row, col
						//		   int_6, int_7											//cube shotCoordX, cube shotCoordY

						//receive lives
						memset(gameData, '\0', sizeof gameData);
						receiveMessage_C(socketFD, gameData);
						int lives = atoi(gameData);
						cube->setCubeLives(lives);

						//receive row
						memset(gameData, '\0', sizeof gameData);
						receiveMessage_C(socketFD, gameData);
						int row = atoi(gameData);
						cube->setCubePositionRow(row);

						//receive col
						memset(gameData, '\0', sizeof gameData);
						receiveMessage_C(socketFD, gameData);
						int col = atoi(gameData);
						cube->setCubePositionCol(col);

						int cubeCoordsArray[CUBE_COORDS_HEIGHT][CUBE_COORDS_WIDTH];

						//calculate cube coords from row and col received from server
						for(int i = 0, lineInc = 0, colInc = 0; i < CUBE_COORDS_HEIGHT * CUBE_COORDS_WIDTH / 2; i++)
						{
						   for(int j = 0; j < 2; j++)
							 {
							    if( j % 2 == 0)
									{
							       if(i != 0 && i % CUBE_COORDS_WIDTH == 0) lineInc++;
							       cubeCoordsArray[i][j] = row + lineInc;
							    }
	     						else
									{
										cubeCoordsArray[i][j] = col + colInc++;
									}

               }
						 }

						// set the cube coords
						cube->loadCubeCoords(cubeCoordsArray);

						//receive string of cube chars
						char cubeCharsArray[CUBE_CHARS_HEIGHT][CUBE_CHARS_WIDTH];

						memset(gameData, '\0', sizeof gameData);
						receiveMessage_C(socketFD, gameData);

						//iterate through string, placing characters where they belong in the array
						int k = 0;

						for (int i = 0; i < CUBE_CHARS_HEIGHT; i++)
						{
							for (int j = 0; j < CUBE_CHARS_WIDTH; j++)
							{
								cubeCharsArray[i][j] = gameData[k++];
							}
						}

						//load cube chars
						cube->loadCubeChars(cubeCharsArray);

						//RECEIVE CUBE SHOT COORDS...
						//LOAD CUBE SHOT COORDS (USE cube->setShotCoords(x, y)...

						// cube->loadCubeCoords(coords); cube->loadCubeChars(chars); cube->setLives(int_3);
						// cube->setCubePositionRow(int_5); cube->setCubePositionCol(int_6);
						// (Optional ?) SEND: confirmation
						/**** END RECEIVE CUBE DATA ****/

						/**** RECEIVE GAME SCORE ****/
						// RECEIVE int_1
						// cube->setCubeScore(int_1);
						// (Optional ?) SEND: confirmation

						memset(gameData, '\0', sizeof gameData);
						receiveMessage_C(socketFD, gameData);
						cube->setCubeScore(atoi(gameData));

						/**** END RECEIVE GAME SCORE ****/

						//Now we tear down the entire world, to then rebuild... (Really Hoping we can get away with this performance-wise)
						//(Need to do this before we receive new world indicator in case world gets deleted)

						//Delete all Obstacles
						for(list<Obstacle*>::iterator it = world->getObstacles().begin();
							it != world->getObstacles().begin(); it++) {
								delete *it;
						}

						/**** RECEIVE NEW WORLD INDICATOR AND (IF APPLICABLE) TYPE  ****/
						//RECEIVE int_1			//isNewWorld flag

						memset(gameData, '\0', sizeof gameData);
						receiveMessage_C(socketFD, gameData);

						int_1 = atoi(gameData);

						//If int_1 == 1:		//If world transition has occurred, or first loop iteration
						if (int_1 == 1)
						{
							delete world;
							// (Optional ?) SEND: confirmation
					//	  RECEIVE int_2:	//World type
							memset(gameData, '\0', sizeof gameData);
							receiveMessage_C(socketFD, gameData);

							int_2 = atoi(gameData);

							if(int_2 == 1)
							{
								world = new Water(isTwoPlayer);
								/* transitionAnimation("Water.txt"); */
							}

									// if (int_2 == 2)
									// {
									// 	world = new Land(isTwoPlayer);
									/* transitionAnimation("Land.txt"); */
									// }
									//
									// else
									// {
									// 	world = new Space(isTwoPlayer);
									/* transitionAnimation("Space.txt"); */
									// }

							// (Optional ?) SEND: confirmation		//Probably not optional, need to wait for world transition animation

							memset(sendConfirm, '\0', sizeof sendConfirm);
							sprintf(sendConfirm, "%d", 1);
							sendMessage_C(socketFD, sendConfirm);

						}

						else if (int_1 == 0)
						{
							//Else If int == 0:		//If no world transition, we need to clear these containers
								world->getObstacles().clear();
								world->getMiniCubes().clear();
								// (Optional ?) SEND: confirmation
						}

						/**** END RECEIVE NEW WORLD INDICATOR AND (IF APPLICABLE) TYPE  ****/

						/**** RECEIVE ONSCREEN OBSTACLES  ****/
						//RECEIEVE int_1		//number of (onscreen) Obstacles

						// (Optional ?) SEND: confirmation
						memset(gameData, '\0', sizeof gameData);
						receiveMessage_C(socketFD, gameData);

						int_1 = atoi(gameData);

						// loop to rebuild Obstacles
						for(int i = 0; i < int_1; i++) {
							//RECEIVE int_2, int_3, int_4, int_5, int_6		// type of Obs, posX, posY, gt, gts(not strictly necessary, but used as convenience)

							memset(gameData, '\0', sizeof gameData);
							receiveMessage_C(socketFD, gameData);
							int_2 = atoi(gameData);

							memset(gameData, '\0', sizeof gameData);
							receiveMessage_C(socketFD, gameData);
							int_3 = atoi(gameData);

							memset(gameData, '\0', sizeof gameData);
							receiveMessage_C(socketFD, gameData);
							int_4 = atoi(gameData);

							memset(gameData, '\0', sizeof gameData);
							receiveMessage_C(socketFD, gameData);
							int_5 = atoi(gameData);

							memset(gameData, '\0', sizeof gameData);
							receiveMessage_C(socketFD, gameData);
							int_6 = atoi(gameData);

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

						/**** RECEIVE MINICUBES  ****/
						//RECEIEVE int_1		//number of miniCubes
						memset(gameData, '\0', sizeof gameData);
						receiveMessage_C(socketFD, gameData);

						int_1 = atoi(gameData);

						// (Optional ?) SEND: confirmation
						for(int i = 0; i < int_1; i++) {
							//RECEIVE int_2, int_3			//miniCube x & y

							memset(gameData, '\0', sizeof gameData);
							receiveMessage_C(socketFD, gameData);

							int_2 = atoi(gameData);

							memset(gameData, '\0', sizeof gameData);
							receiveMessage_C(socketFD, gameData);

							int_3 = atoi(gameData);

							world->getMiniCubes().insert(make_pair(int_2, int_3));
							// (Optional ?) SEND: confirmation
						}
						/**** END RECEIVE MINICUBES  ****/

						//Render World
						world->renderWorld(cube);

						//Render Cube
						attron(COLOR_PAIR(cube->getColor()));
						cube->drawCube();
						if(deathFlag) {
							cube->drawCubeDeath(&userInput);
							deathFlag = false;
						}

						/**** RECEIVE TIME  ****/
						//RECEIVE int_1 into hours
						memset(gameData, '\0', sizeof gameData);
						receiveMessage_C(socketFD, gameData);

						hours = atoi(gameData);
						// (Optional ?) SEND: confirmation

						//RECEIVE int_1 into minutes
						memset(gameData, '\0', sizeof gameData);
						receiveMessage_C(socketFD, gameData);

						minutes = atoi(gameData);
						// (Optional ?) SEND: confirmation

						//RECEIVE int_1 into seconds
						memset(gameData, '\0', sizeof gameData);
						receiveMessage_C(socketFD, gameData);

						seconds = atoi(gameData);
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
						livesDisplay << "Lives: " << cube->getCubeLives() << "   ";

						//Score display
						scoreDisplay.clear();
						scoreDisplay << "Score: " << cube->getCubeScore();
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

	return cube->getCubeScore();
}


// References
// https://stackoverflow.com/questions/2396430/how-to-use-lock-in-openmp
// https://stackoverflow.com/questions/7898215/how-to-clear-input-buffer-in-c/9750394
