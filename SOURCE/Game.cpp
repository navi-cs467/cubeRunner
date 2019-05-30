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
extern WINDOW *scrn;

Game::Game(int gameMode, bool isTwoPlayer) :
	isTwoPlayer(isTwoPlayer), gameMode(gameMode) {

		score = 0;

		if(!isTwoPlayer) world = new Water(gameMode, isTwoPlayer);

		else world = new Water(isTwoPlayer);		//"Blank" world if running as client

		if(gameMode == EASY) cube = new Cube(world, 5);
		else if(gameMode == NORMAL) cube = new Cube(world, 4);
		else if(gameMode == HARD) cube = new Cube(world, 3);

}

struct gameData Game::playGame(char host[], char port[], char username[]) {

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
	bool scrollLock = false, scrollDirChanged = false;
	Direction lockedScrollDir = right, lastScrollDir = right;
	int playerNum = 0;
	int socketFD = -1;
	int inputSocket = -1;
	char inputPort[MSG_SIZE];
	char secondName[MSG_SIZE];
	memset(inputPort, '\0', sizeof inputPort);
	memset(secondName, '\0', sizeof secondName);

	struct gameData scoreInfo;

	scoreInfo.firstName = username;

	bool hasTerminated = false;

	#pragma omp parallel sections shared(userInput, deathFlag, isConnected, \
	playerNum, socketFD, hasTerminated, inputPort, scrollLock, lockedScrollDir, secondName)
	{
		//Thread (1) for updating userInput and cube position
		#pragma omp section
		{

			while ( // cube->getCubeLives() > 0 &&
					userInput != 27 &&
				    userInput != KEY_END &&
				    userInput != 'q' &&
				    userInput != 'Q' && !hasTerminated) {

				userInput = getch();

				//Single Player
				if(!isTwoPlayer && !deathFlag) {

					//Block the move update if render is in progress
					omp_set_lock(&userInputLock);

					if((userInput == KEY_UP || userInput == '8' || userInput == 'w')
							&& cube->getCubePositionRow() > 0) {
						cube->updateCubePosition(0, 0, 0, 1);
						cube->setCubeDirection(up);
					}
					else if((userInput == KEY_DOWN || userInput == '2' || userInput == 's') &&
								cube->getCubePositionRow() + CUBE_CHARS_HEIGHT - 1
									< world->getBottomRow()) {
						cube->updateCubePosition(0, 0, 1, 0);
						cube->setCubeDirection(down);
					}
					else if((userInput == KEY_RIGHT || userInput == '6' || userInput == 'd') &&
								cube->getCubePositionCol() + CUBE_CHARS_WIDTH - 1 < COLS) {
						cube->updateCubePosition(1, 0, 0, 0);
						cube->setCubeDirection(right);
					}
					else if((userInput == KEY_LEFT || userInput == '4' || userInput == 'a') &&
							cube->getCubePositionCol() > 0) {
						cube->updateCubePosition(0, 1, 0, 0);
						cube->setCubeDirection(left);
					}
					else if((userInput == 7 || userInput == 'e') &&
							 cube->getCubePositionRow() > 0 &&
							 cube->getCubePositionCol() > 0) {
						cube->updateCubePosition(0, 1, 0, 1);
						cube->setCubeDirection(left_up);
					}
					else if((userInput == 1 || userInput == 'c') &&
							cube->getCubePositionCol() > 0 &&
							cube->getCubePositionRow() + CUBE_CHARS_HEIGHT - 1
									< world->getBottomRow()) {
						cube->updateCubePosition(0, 1, 1, 0);
						cube->setCubeDirection(left_down);
					}
					else if((userInput == 9 || userInput == 'r')
						    && cube->getCubePositionRow() > 0 &&
							cube->getCubePositionCol() + CUBE_CHARS_WIDTH - 1 < COLS) {
						cube->updateCubePosition(1, 0, 0, 1);
						cube->setCubeDirection(right_up);
					}
					else if((userInput == 3 || userInput == 'v') &&
							cube->getCubePositionRow() > 0 &&
							cube->getCubePositionRow() + CUBE_CHARS_HEIGHT - 1
									< world->getBottomRow()) {
						cube->updateCubePosition(1, 0, 1, 0);
						cube->setCubeDirection(right_down);
					}
					else if(userInput == 32) {
						cube->fireShot();
					}
					else if(userInput == 'l') {
						scrollLock = true;
						lockedScrollDir = cube->getCubeDirection();
					}
					else if(userInput == 'u') {
						scrollLock = false;
					}
					else if(userInput == 't') {
						cube->setTransitionScore(TRANSITION_SCORE_INTERVAL);
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
					char messageToSend[MSG_SIZE];
					memset(messageToSend, '\0', sizeof(messageToSend));
					char confirm[MSG_SIZE];
					memset(confirm, '\0', sizeof(confirm));

					if(playerNum == 1) {
						if(userInput == KEY_UP) {
							// SEND: KEY_UP
							sprintf(messageToSend, "%d", userInput);
							sendMessage_C(inputSocket, messageToSend);
							// RECEIEVE confirmation
							receiveMessage_C(inputSocket, confirm);
						}
						else if(userInput == KEY_DOWN) {
							// SEND: KEY_DOWN
							sprintf(messageToSend, "%d", userInput);
							sendMessage_C(inputSocket, messageToSend);
							// RECEIEVE confirmation
							receiveMessage_C(inputSocket, confirm);
						}
						else if(userInput == KEY_LEFT) {
							// SEND: KEY_LEFT
							sprintf(messageToSend, "%d", userInput);
							sendMessage_C(inputSocket, messageToSend);
							// RECEIEVE confirmation
							receiveMessage_C(inputSocket, confirm);
						}
						else if(userInput == KEY_RIGHT) {
							// SEND: KEY_RIGHT
							sprintf(messageToSend, "%d", userInput);
							sendMessage_C(inputSocket, messageToSend);
							// RECEIEVE confirmation
							receiveMessage_C(inputSocket, confirm);
						}
						else if(userInput == 32) {
							// SEND: 32 (SPACE_BAR - For cube->fireShot())
							sprintf(messageToSend, "%d", userInput);
							sendMessage_C(inputSocket, messageToSend);
							// RECEIEVE confirmation
							receiveMessage_C(inputSocket, confirm);
						}
						else if(userInput == 'l') {
							// SEND: 'l'
							sprintf(messageToSend, "%d", userInput);
							sendMessage_C(inputSocket, messageToSend);
							// RECEIEVE confirmation
							receiveMessage_C(inputSocket, confirm);
						}
						else if(userInput == 'u') {
							// SEND: 'u'
							sprintf(messageToSend, "%d", userInput);
							sendMessage_C(inputSocket, messageToSend);
							// RECEIEVE confirmation
							receiveMessage_C(inputSocket, confirm);
						}
						else if(userInput == 't') {
							// SEND: 't'
							sprintf(messageToSend, "%d", userInput);
							sendMessage_C(inputSocket, messageToSend);
							// RECEIEVE confirmation
							receiveMessage_C(inputSocket, confirm);
						}
						else if(userInput == 27 ||
								userInput == KEY_END ||
								userInput == 'q' ||
								userInput == 'Q') {
							// SEND: q
							userInput = 'q';
							sprintf(messageToSend, "%d", userInput);
							sendMessage_C(inputSocket, messageToSend);
							// RECEIEVE confirmation
							receiveMessage_C(inputSocket, confirm);

							// RECEIVE score into score (as in, into this->score)
							memset(confirm, '\0', sizeof(confirm));
							receiveMessage_C(inputSocket, confirm);
							cube->setCubeScore(atoi(confirm));

							// CLOSE CONNECTION
							close(socketFD);
							close(inputSocket);

							hasTerminated = true;
							break;
						}
					}
					else if(playerNum == 2){
						if(userInput == KEY_LEFT) {
							// SEND: KEY_LEFT
							sprintf(messageToSend, "%d", userInput);
							sendMessage_C(inputSocket, messageToSend);
							// RECEIEVE confirmation
						}
						else if(userInput == KEY_RIGHT) {
							// SEND: KEY_RIGHT
							sprintf(messageToSend, "%d", userInput);
							sendMessage_C(inputSocket, messageToSend);
							// RECEIEVE confirmation
						}
						else if(userInput == KEY_UP) {
							// SEND: KEY_UP
							sprintf(messageToSend, "%d", userInput);
							sendMessage_C(inputSocket, messageToSend);
							// RECEIEVE confirmation
							receiveMessage_C(inputSocket, confirm);
						}
						else if(userInput == KEY_DOWN) {
							// SEND: KEY_DOWN
							sprintf(messageToSend, "%d", userInput);
							sendMessage_C(inputSocket, messageToSend);
							// RECEIEVE confirmation
							receiveMessage_C(inputSocket, confirm);
						}
						else if(userInput == 32) {
							// SEND: 32 (SPACE_BAR - For cube->fireShot())
							sprintf(messageToSend, "%d", userInput);
							sendMessage_C(inputSocket, messageToSend);
							// RECEIEVE confirmation
						}
						else if(userInput == 'l') {
							// SEND: 'l'
							sprintf(messageToSend, "%d", userInput);
							sendMessage_C(inputSocket, messageToSend);
							// RECEIEVE confirmation
							receiveMessage_C(inputSocket, confirm);
						}
						else if(userInput == 'u') {
							// SEND: 'u'
							sprintf(messageToSend, "%d", userInput);
							sendMessage_C(inputSocket, messageToSend);
							// RECEIEVE confirmation
							receiveMessage_C(inputSocket, confirm);
						}
						else if(userInput == 't') {
							// SEND: 't'
							sprintf(messageToSend, "%d", userInput);
							sendMessage_C(inputSocket, messageToSend);
							// RECEIEVE confirmation
							receiveMessage_C(inputSocket, confirm);
						}
						else if(userInput == 27 ||
								userInput == KEY_END ||
								userInput == 'q' ||
								userInput == 'Q') {
							// SEND: q
							userInput = 'q';
							sprintf(messageToSend, "%d", userInput);
							sendMessage_C(inputSocket, messageToSend);
							// RECEIEVE confirmation
							receiveMessage_C(inputSocket, confirm);
							// RECEIVE score into score (as in, into this->score)
							memset(confirm, '\0', sizeof(confirm));

							receiveMessage_C(inputSocket, confirm);
							cube->setCubeScore(atoi(confirm));

							// CLOSE CONNECTION
							close(socketFD);
							close(inputSocket);

							//set the FLAG
							hasTerminated = true;
							break;
						}
					}
					fflush(stdin);		//This may not be portable and/or not work as intended, but let's hope that's not the case
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
				int statsTime, startTime, 
					scrollCountRight = 0, scrollCountLeft = 0,
					scrollCountUp = 0, scrollCountDown = 0,
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
						userInput != 'Q' && !hasTerminated) {

					if(omp_get_wtime() - lastRefreshTime > REFRESH_RATE) {
						lastRefreshTime = omp_get_wtime();

						//Movement updates must wait until current render completes
						omp_set_lock(&userInputLock);

						// World transition if cube->transitionCount
						//reaches TRANSITION_SCORE_INTERVAL

						if(cube->getCubeTransitionScore() >= TRANSITION_SCORE_INTERVAL) {

							//Delete all Obstacles
							for(list<Obstacle*>::iterator it = world->getObstacles().begin();
							it != world->getObstacles().begin(); it++) {
								delete *it;
							}

							//Create new world
							if(typeid(*world) == typeid(Water)) {
								
								//Delete the existing world
								delete world;
								
								//Create new world
								world = new Land(gameMode, isTwoPlayer);
								cube->transitionWorld(world);
								omp_unset_lock(&userInputLock);		//Unlock input thread to confirm death animation
								transitionAnimationInsideThread("GRAPHICS/Land.txt", 107,
									16, WHITE_WHITE, 15, GREEN_WHITE, &userInput);
								omp_set_lock(&userInputLock);		//Relock
							}
							else if(typeid(*world) == typeid(Land)) {
								
								//Delete the existing world
								delete world;
								
								//Create new world
								world = new Space(gameMode, isTwoPlayer);
								cube->transitionWorld(world);
								omp_unset_lock(&userInputLock);		//Unlock input thread to confirm death animation
								transitionAnimationInsideThread("GRAPHICS/Space.txt", 121,
									16, BLACK_BLACK, 1, WHITE_BLACK, &userInput);
								omp_set_lock(&userInputLock);		//Relock
							}
							else if(typeid(*world) == typeid(Space)) {
								
								//Delete the existing world
								delete world;
								
								//Create new world
								world = new Water(gameMode, isTwoPlayer);
								cube->transitionWorld(world);
								omp_unset_lock(&userInputLock);		//Unlock input thread to confirm death animation
								transitionAnimationInsideThread("GRAPHICS/Water.txt", 110,
									16, BLUE_BLUE, 30, WHITE_BLUE, &userInput);
								omp_set_lock(&userInputLock);		//Relock
							}

							//Increase speeds after each transition. 
							//Speeds are capped after 6 world transitions
							if(cube->getCubeScore() <= TRANSITION_SCORE_INTERVAL * 6) {
								scrollRate *= SCROLL_MOVE_UPDATE_RATE;
								moveRate *= SCROLL_MOVE_UPDATE_RATE;
							}

							//Reset cubes position to left-middle starting point
							world->resetPlayer(cube);

							//Reset transitionCount
							cube->resetCubeTransitionScore();
						}

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
						if(cube->getCubeLives() == 0) {
							// transitionAnimation("gameOver.txt");

							//Delete all Obstacles
							for(list<Obstacle*>::iterator it = world->getObstacles().begin();
							it != world->getObstacles().begin(); it++) {
								delete *it;
							}
							//Delete cube
							delete cube;
							//Delete world
							delete world;

							hasTerminated = true;

							break;
						}

						//Reset player if death occurred (but no game over)
						if(deathFlag) world->resetPlayer(cube);

						//Reset death flag
						deathFlag = false;

						string output; ostringstream timeDisplay, livesDisplay, scoreDisplay;

						//Set time ostringstream
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

						scoreInfo.hours = hours;
						scoreInfo.minutes = minutes;
						scoreInfo.seconds = seconds;

						//Setup life count ostringstream 
						livesDisplay.clear();
						livesDisplay << "Lives: " << cube->getCubeLives() << "   ";
						/* if(gameMode == EASY) livesDisplay << "Lives: " << 5;
						else if(gameMode == NORMAL) livesDisplay << "Lives: " << 4;
						else if(gameMode == HARD) livesDisplay << "Lives: " << 3; */

						//Setup score ostreamstring
						scoreDisplay.clear();
						scoreDisplay << "Score: " << cube->getCubeScore();
						
						//If space world, prepare scroll lock indicator
						string scrollLockIndicator;
						if(typeid(*world) == typeid(Space)) {
							if(scrollLock)
								scrollLockIndicator = "Scroll Direction Locked. Press 'u' to Unlock.";
							else
								scrollLockIndicator = "Scroll Direction Unlocked. Press 'l' to Lock.";
						}
						else
							scrollLockIndicator = "";
						
						//Render scroll lock indicator
						attron(COLOR_PAIR(YELLOW_BLACK));
						mvhline(LINES - 1, 0, ' ', COLS);
						mvaddstr(LINES - 1, 10, scrollLockIndicator.c_str());
						
						//Render game stats display
						output = string(timeDisplay.str().c_str())  + "   " +
								 string(scoreDisplay.str().c_str()) + "   " +
								 string(livesDisplay.str().c_str());
						attron(COLOR_PAIR(YELLOW_BLACK));
						mvhline(LINES - 1, 10 + scrollLockIndicator.length(), ' ', COLS);
						mvaddstr(LINES - 1, COLS - output.length() - 10, output.c_str());
						refresh();

						//Pending movement update can now be processed
						omp_unset_lock(&userInputLock);

					}

					//Scroll
					if(omp_get_wtime() - lastScrollTime > scrollRate) {
						lastScrollTime = omp_get_wtime();
						
						if(typeid(*world) != typeid(Space)) {
							world->scroll_(cube);
						}
						else {
							if(scrollLock)
								world->scroll_(cube, lockedScrollDir);
							else
								world->scroll_(cube, none);
								//Used for loading offscreen Obstacles when scroll,
								//direction changes (Space only)
								if(lastScrollDir != cube->getCubeDirection())
									scrollDirChanged = true;
						}
						
						//Load new offscreen Obstacles and miniCubes every time
						//a screens-worth has been scrolled, or the scroll direction
						//changes (Space only)
						if(scrollCountRight++ == COLS) {
							world->loadOSObs(right);
							world->loadOSMCs(right);
							scrollCountRight = 0;
						}
						if(scrollCountLeft++ == COLS) {
							world->loadOSObs(left);
							world->loadOSMCs(left);
							scrollCountLeft = 0;
						}
						if(scrollCountUp++ == LINES) {
							world->loadOSObs(up);
							world->loadOSMCs(up);
							scrollCountUp = 0;
						}
						if(scrollCountDown++ == LINES) {
							world->loadOSObs(down);
							world->loadOSMCs(down);
							scrollCountDown = 0;
						}
						if(scrollDirChanged) {
							world->loadOSObs(cube->getCubeDirection());
							world->loadOSMCs(cube->getCubeDirection());
							scrollDirChanged = false;
						}

						//Repopulate onscreen miniCubes if too many have been
						//consumed by moving obstacles according to this
						//threshold
						
						//Determine existing onscreen miniCube count
						int onscreenMCCount = 0;
						set<pair<int, int>>::iterator mcs;
						for(mcs = world->getMiniCubes().begin(); 
							mcs != world->getMiniCubes().end(); mcs++) {
							if(mcs->second < COLS && mcs->second >= 0 &&
							   mcs->first < world->getBottomRow() && mcs->first >= 0) 
									onscreenMCCount++;
						}
						if(onscreenMCCount < (NUM_MCS_EASY / gameMode) / 2)
							world->initMiniCubes(1);
						
						//Used to determine if scroll direction changes 
						//(Space only), for loading offscreen Obstacles
						if(scrollLock) lastScrollDir = lockedScrollDir;
						else if(typeid(*world) == typeid(Space))
							lastScrollDir = cube->getCubeDirection();
						else lastScrollDir = right;
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

				//Set LINES and COLS to minimum values (save original values)
				int original_LINES = LINES, original_COLS = COLS;
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

				if(DEBUG) {
					move(6,5); printw("Received Player Indicator..."); refresh();
				}

				//check for other player
				//if we received 0, we are connected but other player isn't
				//so we check for next confirmation which should tell us if gamemodes match or not
				if (strcmp(message,"0") == 0)
				{
					//send chosen game mode
					char gM[MSG_SIZE];
					sprintf(gM, "%d", gameMode);
					sendMessage_C(socketFD, gM);
					if(DEBUG)
						move(7,5); printw("Sent GM Player 1...\n"); refresh();

					//check for other player and gamemode match
					memset(message, '\0', sizeof message);
					receiveMessage_C(socketFD, message);

					if(DEBUG) {
						move(8,5); printw("Received Confirmation Other Player Now Connected & GM Match Indicator...\n"); refresh();
					}

					//if we got to this point, player is player 1 so change the value
					playerNum = 1;
				}

				if ((strcmp(message,"1") == 0) && playerNum != 1)
				{
					//send chosen game mode
					char gM[MSG_SIZE];
					sprintf(gM, "%d", gameMode);
					sendMessage_C(socketFD, gM);

					if(DEBUG) {
						move(9,5); printw("Sent GM Player 2...\n"); refresh();
					}

					//check for gamemode match
					memset(message, '\0', sizeof message);
					receiveMessage_C(socketFD, message);

					if(DEBUG) {
						move(10,5); printw("Received GM Match Indicator...\n"); refresh();
					}

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

				//receive dataPort from server, send confirmation to server
				char confirm[MSG_SIZE];
				memset(confirm, '\0', sizeof confirm);
				memset(message, '\0', sizeof message);

				//receive other player's username
				if (playerNum == 2)
				{
					sendMessage_C(socketFD, username);
				}

				if(playerNum == 1)
				{
					receiveMessage_C(socketFD, secondName);
					sendMessage_C(socketFD, secondName);
				}

				//receive dataPort from server, send confirmation to server
				memset(confirm, '\0', sizeof confirm);
				sprintf(confirm, "%d", 1);

				receiveMessage_C(socketFD, inputPort);
				sendMessage_C(socketFD, confirm);

				//connect to input socket
				inputSocket = initSocket(host, inputPort);

				//receive confirmation from server
				memset(message, '\0', sizeof message);
				receiveMessage_C(inputSocket, message);

				//send confirmation on new connection
				memset(confirm, '\0', sizeof confirm);
				sprintf(confirm, "%d", playerNum);
				sendMessage_C(inputSocket, confirm);

				//set isConnected to TRUE
				isConnected = TRUE;

				//For Time Display
				int seconds, minutes, hours;
				string output; ostringstream timeDisplay, livesDisplay, scoreDisplay;
				
				//For death message display
				int collisionType = 0;

				if(DEBUG) {
					move(11,5); printw("Starting Game Loop..."); refresh();
				}
				
				//initialize cube coords
				cube->initializeCubeCoords();
				
				while (!hasTerminated) {
					
						//Block movement updates while render completes
						omp_set_lock(&userInputLock);

						//Pseudocode variables... change as desired
						int int_1, int_2, int_3, int_4, int_5, int_6, int_7, int_8;
						char earlyTerm[MSG_SIZE], scoreStr[MSG_SIZE],
							 gameData[MSG_SIZE], sendConfirm[MSG_SIZE];

						memset(earlyTerm, '\0', sizeof earlyTerm);
						memset(scoreStr, '\0', sizeof scoreStr);
						memset(gameData, '\0', sizeof gameData);
						memset(sendConfirm, '\0', sizeof sendConfirm);

						/**** RECEIVE (OTHER PLAYER) EARLY TERMINATION STATUS ****/

						// RECEIVE (10 bytes) into earlyTerm
						receiveMessage_C(socketFD, earlyTerm);
						memset(sendConfirm, '\0', sizeof sendConfirm);
						sprintf(sendConfirm, "%d", 1);
						sendMessage_C(socketFD, sendConfirm);

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

							memset(sendConfirm, '\0', sizeof sendConfirm);
							sprintf(sendConfirm, "%d", 1);
							sendMessage_C(socketFD, sendConfirm);

							cube->setCubeScore(atoi(scoreStr));

							// close connection
							close(socketFD);
							close(inputSocket);

							break;
						}

						/**** END RECEIVE (OTHER PLAYER) EARLY TERMINATION STATUS ****/

						//Used for debugging only
						int z = 13;

						/**** RECEIVE DEATH FLAG ****/
						//RECEIVE int_1
						receiveMessage_C(socketFD, gameData);
						if(DEBUG) {
							move(z++, 8); printw("RECEIVED (deathflag): %s\n", gameData); refresh();
						}
						memset(sendConfirm, '\0', sizeof sendConfirm);
						sprintf(sendConfirm, "%d", 1);
						sendMessage_C(socketFD, sendConfirm);
						if(DEBUG) {
							//move(z, 60); printw("SEND CONFIRM (deathflag): %s\n", sendConfirm); refresh();
						}

						int_1 = atoi(gameData);
						//If int_1 == 1:		//Death happened
						if (int_1 == 1)
						{
							// receive game over flag
							memset(gameData, '\0', sizeof gameData);
							receiveMessage_C(socketFD, gameData);
							if(DEBUG) {
								move(z++, 8); printw("RECEIVED (game over flag): %s\n", gameData); refresh();
							}

							memset(sendConfirm, '\0', sizeof sendConfirm);
							sprintf(sendConfirm, "%d", 1);
							sendMessage_C(socketFD, sendConfirm);
							if(DEBUG) {
								//move(z, 60); printw("SEND CONFIRM (game over flag): %s\n", sendConfirm); refresh();
							}

							int_2 = atoi(gameData);

							//	  If int_2 == 1:		//Game Over
							if (int_2 == 1)
							{
								//receive final score
								memset(gameData, '\0', sizeof gameData);
								receiveMessage_C(socketFD, gameData);
								if(DEBUG) {
									move(z++, 8); printw("RECEIVED (final score): %s\n", gameData); refresh();
								}

								cube->setCubeScore(atoi(gameData));

								//send confirmation
								memset(sendConfirm, '\0', sizeof sendConfirm);
								sprintf(sendConfirm, "%d", 1);
								sendMessage_C(socketFD, sendConfirm);
								if(DEBUG) {
									//move(z++, 8); printw("SEND CONFIRM (final score): %s\n", gameData); refresh();
								}

							  //CLOSE connections
								close(socketFD);
								close(inputSocket);

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

									hasTerminated = true;

									break;
							}

							//	  Else If int_2 == 0	//Death But No Game Over
							//		  //deathFlag = true;
							//	      // (Optional ?) SEND: confirmation		//Probably not optional, server needs to wait for death animation

							else if (int_2 == 0)
							{
								//set deathFlag
								deathFlag = true;
								
								//receive Obstacle collision type
								memset(gameData, '\0', sizeof gameData);
								receiveMessage_C(socketFD, gameData);
								if(DEBUG) {
									move(z++, 8); printw("RECEIVED (ob collision type): %s\n", gameData); refresh();
								}
								
								collisionType = atoi(gameData);
								
								//send confirmation
								memset(sendConfirm, '\0', sizeof sendConfirm);
								sprintf(sendConfirm, "%d", 1);
								sendMessage_C(socketFD, sendConfirm);
								if(DEBUG) {
									//move(z++, 8); printw("SEND CONFIRM (ob collision type): %s\n", gameData); refresh();
								}
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
						if(DEBUG) {
							move(z++, 8); printw("RECEIVED (lives): %s\n", gameData); refresh();
						}

						memset(sendConfirm, '\0', sizeof sendConfirm);
						sprintf(sendConfirm, "%d", 1);
						sendMessage_C(socketFD, sendConfirm);
						if(DEBUG) {
							//move(z, 60); printw("SENT CONFIRM (lives): %s\n", sendConfirm); refresh();
						}

						int lives = atoi(gameData);
						cube->setCubeLives(lives);

						//receive row
						memset(gameData, '\0', sizeof gameData);
						receiveMessage_C(socketFD, gameData);
						if(DEBUG) {
							move(z++, 8); printw("RECEIVED (row): %s\n", gameData); refresh();
						}

						memset(sendConfirm, '\0', sizeof sendConfirm);
						sprintf(sendConfirm, "%d", 1);
						sendMessage_C(socketFD, sendConfirm);
						if(DEBUG) {
							//move(z, 60); printw("SENT CONFIRM (row): %s\n", sendConfirm); refresh();
						}

						int row = atoi(gameData);
						int prevRow = cube->getCubePositionRow();
						cube->setCubePositionRow(row);

						//receive col
						memset(gameData, '\0', sizeof gameData);
						receiveMessage_C(socketFD, gameData);
						if(DEBUG) {
							move(z++, 8); printw("RECEIVED (col): %s\n", gameData); refresh();
						}

						memset(sendConfirm, '\0', sizeof sendConfirm);
						sprintf(sendConfirm, "%d", 1);
						sendMessage_C(socketFD, sendConfirm);
						if(DEBUG) {
							//move(z, 60); printw("SENT CONFIRM (col): %s\n", sendConfirm); refresh();
						}

						int col = atoi(gameData);
						int prevCol = cube->getCubePositionCol();
						cube->setCubePositionCol(col);
						
						//Update cubeCoords
						cube->updateCubeCoords(prevCol, cube->getCubePositionCol(),
											   prevRow, cube->getCubePositionRow());
						

						//initialize cubeCoors (OLD CUBE ONLY)
						/* int cubeCoordsArray[CUBE_COORDS_HEIGHT][CUBE_COORDS_WIDTH];

						//calculate cube coords from row and col received from server
						for(int i = 0, lineInc = 0, colInc = 0;
							i < CUBE_COORDS_HEIGHT * CUBE_COORDS_WIDTH / 2; i++)
						{
						   if(i % 4 == 0) colInc = 0;
						   for(int j = 0; j < 2; j++)
							{
								if( j % 2 == 0)
								{
								   if(i != 0 && i % CUBE_CHARS_WIDTH == 0) lineInc++;
								   cubeCoordsArray[i][j] = row + lineInc;
								}
								else
								{
									cubeCoordsArray[i][j] = col + colInc++;
								}
							}
						 }

						// set the cube coords
						cube->loadCubeCoords(cubeCoordsArray); */

						/* //receive string of cube chars (OLD CUBE ONLY)
						char cubeCharsArray[CUBE_CHARS_HEIGHT][CUBE_CHARS_WIDTH];

						memset(gameData, '\0', sizeof gameData);
						receiveMessage_C(socketFD, gameData);
						if(DEBUG) {
							move(z++, 8); printw("RECEIVED (cubeChars): %s\n", gameData); refresh();
						}

						memset(sendConfirm, '\0', sizeof sendConfirm);
						sprintf(sendConfirm, "%d", 1);
						sendMessage_C(socketFD, sendConfirm);
						if(DEBUG) {
							//move(z, 60); printw("SENT CONFIRM (cubeChars): %s\n", sendConfirm); refresh();
						}

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
						cube->loadCubeChars(cubeCharsArray); */

						//RECEIVE CUBE SHOT COORDS...
						//LOAD CUBE SHOT COORDS (USE cube->setShotCoords(x, y)...

						//receive x shot coord
						memset(gameData, '\0', sizeof gameData);
						receiveMessage_C(socketFD, gameData);
						if(DEBUG) {
							move(z++, 8); printw("RECEIVED (x shot coord): %s\n", gameData); refresh();
						}

						memset(sendConfirm, '\0', sizeof sendConfirm);
						sprintf(sendConfirm, "%d", 1);
						sendMessage_C(socketFD, sendConfirm);
						if(DEBUG) {
							//move(z, 60); printw("SENT CONFIRM (x shot coord): %s\n", sendConfirm); refresh();
						}

						int xShotCoord = atoi(gameData);

						//receive y shot coord
						memset(gameData, '\0', sizeof gameData);
						receiveMessage_C(socketFD, gameData);
						if(DEBUG) {
							move(z++, 8); printw("RECEIVED (y shot coord): %s\n", gameData); refresh();
						}

						memset(sendConfirm, '\0', sizeof sendConfirm);
						sprintf(sendConfirm, "%d", 1);
						sendMessage_C(socketFD, sendConfirm);
						if(DEBUG) {
							//move(z, 60); printw("SENT CONFIRM (y shot coord): %s\n", sendConfirm); refresh();
						}
						int yShotCoord = atoi(gameData);

						cube->setShotCoords(xShotCoord, yShotCoord);
						
						//receive cube direction
						memset(gameData, '\0', sizeof gameData);
						receiveMessage_C(socketFD, gameData);
						if(DEBUG) {
							move(z++, 8); printw("RECEIVED (cube direction): %s\n", gameData); refresh();
						}

						memset(sendConfirm, '\0', sizeof sendConfirm);
						sprintf(sendConfirm, "%d", 1);
						sendMessage_C(socketFD, sendConfirm);
						if(DEBUG) {
							//move(z, 60); printw("SENT CONFIRM (cube direction): %s\n", sendConfirm); refresh();
						}
						Direction cubeDirection = static_cast<Direction>(atoi(gameData));

						cube->setCubeDirection(cubeDirection);
						if(cubeDirection == right || cubeDirection == right_down || cubeDirection == right_up)
							cube->setUseLeftCube(false);
						else if(cubeDirection == left || cubeDirection == left_down || cubeDirection == left_up)
							cube->setUseLeftCube(true);

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
						if(DEBUG) {
							move(z++, 8); printw("RECEIVED (score): %s\n", gameData); refresh();
						}

						memset(sendConfirm, '\0', sizeof sendConfirm);
						sprintf(sendConfirm, "%d", 1);
						sendMessage_C(socketFD, sendConfirm);
						if(DEBUG) {
							//move(z, 60); printw("SENT CONFIRM (score): %s\n", sendConfirm); refresh();
						}

						cube->setCubeScore(atoi(gameData));

						/**** END RECEIVE GAME SCORE ****/

						//Delete all Obstacles
						for(list<Obstacle*>::iterator it = world->getObstacles().begin();
							it != world->getObstacles().begin(); it++) {
								delete *it;
						}

						/**** RECEIVE NEW WORLD INDICATOR AND (IF APPLICABLE) TYPE  ****/
						//RECEIVE int_1			//isNewWorld flag

						memset(gameData, '\0', sizeof gameData);
						receiveMessage_C(socketFD, gameData);
						if(DEBUG) {
							move(z++, 8); printw("RECEIVED (new world indicator): %s\n", gameData); refresh();
						}

						memset(sendConfirm, '\0', sizeof sendConfirm);
						sprintf(sendConfirm, "%d", 1);
						sendMessage_C(socketFD, sendConfirm);
						if(DEBUG) {
							//move(z, 60); printw("SENT CONFIRM (new world indicator): %s\n", sendConfirm); refresh();
						}

						int_1 = atoi(gameData);

						//If int_1 == 1:		//If world transition has occurred, or first loop iteration
						if (int_1 == 1)
						{
							delete world;
							
							//Receive new world type
							memset(gameData, '\0', sizeof gameData);
							receiveMessage_C(socketFD, gameData);
							if(DEBUG) {
								move(z++, 8); printw("RECEIVED (new world type): %s\n", gameData); refresh();
							}
							
							//Confirm new world type
							memset(sendConfirm, '\0', sizeof sendConfirm);
							sprintf(sendConfirm, "%d", 1);
							sendMessage_C(socketFD, sendConfirm);
							if(DEBUG) {
								//move(z, 60); printw("SENT CONFIRM (new world type): %s\n", sendConfirm); refresh();
							}

							int_2 = atoi(gameData);

							if(int_2 == 1)
							{
								world = new Water(isTwoPlayer);
								cube->transitionWorld(world);
								transitionAnimationInsideThread("GRAPHICS/Water.txt", 120,
									16, BLUE_BLUE, 30, WHITE_BLUE, &userInput);
							}
							
							else if(int_2 == 2)
							{
								world = new Land(isTwoPlayer);
								cube->transitionWorld(world);
								transitionAnimationInsideThread("GRAPHICS/Land.txt", 115,
									16, WHITE_WHITE, 15, GREEN_WHITE, &userInput);
							}
							
							else if(int_2 == 3)
							{
								world = new Space(isTwoPlayer);
								cube->transitionWorld(world);
								transitionAnimationInsideThread("GRAPHICS/Space.txt", 121,
									16, BLACK_BLACK, 1, WHITE_BLACK, &userInput);
							}
							
							//print waiting screen, in case waiting on other player's confirmation
							waitingForOtherPlayer(playerNum);
							
							//Receive new world player confirm request
							memset(gameData, '\0', sizeof gameData);
							receiveMessage_C(socketFD, gameData);
							if(DEBUG) {
								move(z++, 8); printw("RECEIVED (new world player confirm request): %s\n", gameData); refresh();
							}
							
							//Confirm "player confirmed" new world 
							memset(sendConfirm, '\0', sizeof sendConfirm);
							sprintf(sendConfirm, "%d", 1);
							sendMessage_C(socketFD, sendConfirm);
							if(DEBUG) {
								//move(z, 60); printw("SENT CONFIRM (new world player confirm request): %s\n", sendConfirm); refresh();
							}

						}

						else if (int_1 == 0)
						{
							//If no world transition, we need to clear these containers
							if(world->getObstacles().size() > 0) world->getObstacles().clear();
							if(world->getMiniCubes().size() > 0) world->getMiniCubes().clear();
						}

						/**** END RECEIVE NEW WORLD INDICATOR AND (IF APPLICABLE) TYPE  ****/

						/**** RECEIVE ONSCREEN OBSTACLES  ****/
						//RECEIEVE int_1		//number of (onscreen) Obstacles

						// (Optional ?) SEND: confirmation
						memset(gameData, '\0', sizeof gameData);
						receiveMessage_C(socketFD, gameData);
						if(DEBUG) {
							move(z++, 8); printw("RECEIVED (numObs): %s\n", gameData); refresh();
						}

						memset(sendConfirm, '\0', sizeof sendConfirm);
						sprintf(sendConfirm, "%d", 1);
						sendMessage_C(socketFD, sendConfirm);
						if(DEBUG) {
							//move(z, 60); printw("SENT CONFIRM (numObs): %s\n", sendConfirm); refresh();
						}

						int_1 = atoi(gameData);

						//Used only for debugging
						int obsNum = 0;

						// loop to rebuild Obstacles
						for(int i = 0; i < int_1; i++, obsNum++, z = 20) {
							//RECEIVE int_2, int_3, int_4, int_5, int_6, int_7, int_8		// type of Obs, posX, posY, gt, gts(not strictly necessary, but used as convenience), hits

							memset(gameData, '\0', sizeof gameData);
							receiveMessage_C(socketFD, gameData);
							if(DEBUG) {
								move(z++, 8); printw("RECEIVED (ob %d type): %s\n", obsNum, gameData); refresh();
							}

							memset(sendConfirm, '\0', sizeof sendConfirm);
							sprintf(sendConfirm, "%d", 1);
							sendMessage_C(socketFD, sendConfirm);
							if(DEBUG) {
								//move(z, 60); printw("SENT CONFIRM (ob %d type): %s\n", obsNum, sendConfirm); refresh();
							}

							int_2 = atoi(gameData);

							memset(gameData, '\0', sizeof gameData);
							receiveMessage_C(socketFD, gameData);
							if(DEBUG) {
								move(z++, 8); printw("RECEIVED (ob %d posX): %s\n", obsNum, gameData); refresh();
							}

							memset(sendConfirm, '\0', sizeof sendConfirm);
							sprintf(sendConfirm, "%d", 1);
							sendMessage_C(socketFD, sendConfirm);
							if(DEBUG) {
								//move(z, 60); printw("SENT CONFIRM (ob %d posX): %s\n", obsNum, sendConfirm); refresh();
							}

							int_3 = atoi(gameData);

							memset(gameData, '\0', sizeof gameData);
							receiveMessage_C(socketFD, gameData);
							if(DEBUG) {
								move(z++, 8); printw("RECEIVED (ob %d posY): %s\n", obsNum, gameData); refresh();
							}

							memset(sendConfirm, '\0', sizeof sendConfirm);
							sprintf(sendConfirm, "%d", 1);
							sendMessage_C(socketFD, sendConfirm);
							if(DEBUG) {
								//move(z, 60); printw("SENT CONFIRM (ob %d posY): %s\n", obsNum, sendConfirm); refresh();
							}

							int_4 = atoi(gameData);

							memset(gameData, '\0', sizeof gameData);
							receiveMessage_C(socketFD, gameData);
							if(DEBUG) {
								move(z++, 8); printw("RECEIVED (ob %d gt): %s\n", obsNum, gameData); refresh();
							}

							memset(sendConfirm, '\0', sizeof sendConfirm);
							sprintf(sendConfirm, "%d", 1);
							sendMessage_C(socketFD, sendConfirm);
							if(DEBUG) {
								//move(z, 60); printw("SENT CONFIRM (ob %d gt): %s\n", obsNum, sendConfirm); refresh();
							}

							int_5 = atoi(gameData); //move(23, 5); printw("gt: %d", int_5); refresh();

							memset(gameData, '\0', sizeof gameData);
							receiveMessage_C(socketFD, gameData);
							if(DEBUG) {
								move(z++, 8); printw("RECEIVED (ob %d gts): %s\n", obsNum, gameData); refresh();
							}

							memset(sendConfirm, '\0', sizeof sendConfirm);
							sprintf(sendConfirm, "%d", 1);
							sendMessage_C(socketFD, sendConfirm);
							if(DEBUG) {
								//move(z, 60); printw("SENT CONFIRM (ob %d gts): %s\n", obsNum, sendConfirm); refresh();
							}

							int_6 = atoi(gameData);

							memset(gameData, '\0', sizeof gameData);
							receiveMessage_C(socketFD, gameData);
							if(DEBUG) {
								move(z++, 8); printw("RECEIVED (ob %d color/colorSeed): %s\n", obsNum, gameData); refresh();
							}

							memset(sendConfirm, '\0', sizeof sendConfirm);
							sprintf(sendConfirm, "%d", 1);
							sendMessage_C(socketFD, sendConfirm);
							if(DEBUG) {
								//move(z, 60); printw("SENT CONFIRM (ob %d color/colorSeed): %s\n", obsNum, sendConfirm); refresh();
							}

							int_7 = atoi(gameData);

							memset(gameData, '\0', sizeof gameData);
							receiveMessage_C(socketFD, gameData);
							if(DEBUG) {
								move(z++, 8); printw("RECEIVED (ob %d hits): %s\n", obsNum, gameData); refresh();
							}

							memset(sendConfirm, '\0', sizeof sendConfirm);
							sprintf(sendConfirm, "%d", 1);
							sendMessage_C(socketFD, sendConfirm);
							if(DEBUG) {
								//move(z, 60); printw("SENT CONFIRM (ob %d hits): %s\n", obsNum, sendConfirm); refresh();
							}

							int_8 = atoi(gameData);


							//If we are going to include shooting functionality, need Obstacle->hits
							/* memset(gameData, '\0', sizeof gameData);
							receiveMessage_C(socketFD, gameData);
							int_7 = atoi(gameData); //move(25, 5); printw("gts: %d", int_7); refresh(); */

							if(int_2 == 1) 
								world->getObstacles().push_back(new Seaweed(int_2, int_3, int_4,
																			int_5, int_6, int_7,
																			int_8, gameMode));
								
							else if(int_2 == 2)
								world->getObstacles().push_back(new Coral(int_2, int_3, int_4,
																			int_5, int_6, int_7,
																			int_8, gameMode));
							else if(int_2 == 3) 
								world->getObstacles().push_back(new Shark(int_2, int_3, int_4,
																			int_5, int_6, int_7,
																			int_8, gameMode));
							
							else if(int_2 == 4) 
								world->getObstacles().push_back(new Octopus(int_2, int_3, int_4,
																			int_5, int_6, int_7,
																			int_8, gameMode));
							
							if(int_2 == 5) 
								world->getObstacles().push_back(new Tree(int_2, int_3, int_4,
																			int_5, int_6, int_7,
																			int_8, gameMode));
								
							else if(int_2 == 6)
								world->getObstacles().push_back(new Rock(int_2, int_3, int_4,
																			int_5, int_6, int_7,
																			int_8, gameMode));
							else if(int_2 == 7) 
								world->getObstacles().push_back(new Bird(int_2, int_3, int_4,
																			int_5, int_6, int_7,
																			int_8, gameMode));
							
							else if(int_2 == 8) 
								world->getObstacles().push_back(new Bat(int_2, int_3, int_4,
																			int_5, int_6, int_7,
																			int_8, gameMode));
							else if(int_2 == 9) 
								world->getObstacles().push_back(new Asteroid(int_2, int_3, int_4,
																			int_5, int_6, int_7,
																			int_8, gameMode));
							else if(int_2 == 10) 
								world->getObstacles().push_back(new Planet(int_2, int_3, int_4,
																			int_5, int_6, int_7,
																			int_8, gameMode));	 
							else if(int_2 == 11) 
								world->getObstacles().push_back(new Comet(int_2, int_3, int_4,
																			int_5, int_6, int_7,
																			int_8, gameMode));
							else if(int_2 == 12) 
								world->getObstacles().push_back(new Spaceship(int_2, int_3, int_4,
																			  int_5, int_6, int_7,
																			  int_8, gameMode));												

							// (Optional ?) SEND: confirmation
							
							//If Obstacle is stationary, get holes
							if(world->getObstacles().back()->getIsStationary()) {
								
								//Recieve number of holes
								memset(gameData, '\0', sizeof gameData);
								receiveMessage_C(socketFD, gameData);
								if(DEBUG) {
									move(z++, 8); printw("RECEIVED (ob %d num holes): %s\n", obsNum, gameData); refresh();
								}

								memset(sendConfirm, '\0', sizeof sendConfirm);
								sprintf(sendConfirm, "%d", 1);
								sendMessage_C(socketFD, sendConfirm);
								
								int numHoles = atoi(gameData);
								
								for(int j = 0; j < numHoles; j++) {
									
									//get hole position x
									memset(gameData, '\0', sizeof gameData);
									receiveMessage_C(socketFD, gameData);
									if(DEBUG) {
										move(z++, 8); printw("RECEIVED (ob %d hole %d x pos): %s\n", obsNum, j, gameData); refresh();
									}

									memset(sendConfirm, '\0', sizeof sendConfirm);
									sprintf(sendConfirm, "%d", 1);
									sendMessage_C(socketFD, sendConfirm);
								
									int holePosX = atoi(gameData);
									
									//get hole position y
									memset(gameData, '\0', sizeof gameData);
									receiveMessage_C(socketFD, gameData);
									if(DEBUG) {
										move(z++, 8); printw("RECEIVED (ob %d hole %d y pos): %s\n", obsNum, j, gameData); refresh();
									}

									memset(sendConfirm, '\0', sizeof sendConfirm);
									sprintf(sendConfirm, "%d", 1);
									sendMessage_C(socketFD, sendConfirm);
								
									int holePosY = atoi(gameData);
									
									world->getObstacles().back()->getHoles().insert(make_pair(holePosX, holePosY));
								}
							}
						}
						/**** END RECEIVE ONSCREEN OBSTACLES  ****/

						/**** RECEIVE MINICUBES  ****/
						//RECEIEVE int_1		//number of miniCubes
						memset(gameData, '\0', sizeof gameData);
						receiveMessage_C(socketFD, gameData);

						memset(sendConfirm, '\0', sizeof sendConfirm);
						sprintf(sendConfirm, "%d", 1);
						sendMessage_C(socketFD, sendConfirm);

						int_1 = atoi(gameData);

						// (Optional ?) SEND: confirmation
						for(int i = 0; i < int_1; i++) {
							//RECEIVE int_2, int_3			//miniCube x & y

							memset(gameData, '\0', sizeof gameData);
							receiveMessage_C(socketFD, gameData);

							memset(sendConfirm, '\0', sizeof sendConfirm);
							sprintf(sendConfirm, "%d", 1);
							sendMessage_C(socketFD, sendConfirm);

							int_2 = atoi(gameData);

							memset(gameData, '\0', sizeof gameData);
							receiveMessage_C(socketFD, gameData);

							memset(sendConfirm, '\0', sizeof sendConfirm);
							sprintf(sendConfirm, "%d", 1);
							sendMessage_C(socketFD, sendConfirm);

							int_3 = atoi(gameData);

							world->getMiniCubes().insert(make_pair(int_2, int_3));
							// (Optional ?) SEND: confirmation
						}
						/**** END RECEIVE MINICUBES  ****/

						//clear();

						//Render World
						world->renderWorld(cube);

						//Render Cube and Cube Shot
						attron(COLOR_PAIR(cube->getColor()));
						if(!deathFlag) cube->printShot();
						cube->drawCube();
						if(deathFlag) {
							cube->drawCubeDeath(&userInput, collisionType);
							deathFlag = false;
						}

						/**** RECEIVE WORLD RENDER CONFIRM REQUEST ****/
						// memset(gameData, '\0', sizeof gameData);
						// receiveMessage_C(socketFD, gameData);
						//
						// memset(sendConfirm, '\0', sizeof sendConfirm);
						// sprintf(sendConfirm, "%d", 1);
						// sendMessage_C(socketFD, sendConfirm);
						/**** END RECEIVE WORLD RENDER CONFIRM REQUEST ****/

						/**** RECEIVE TIME  ****/
						//RECEIVE int_1 into hours
						memset(gameData, '\0', sizeof gameData);
						receiveMessage_C(socketFD, gameData);

						memset(sendConfirm, '\0', sizeof sendConfirm);
						sprintf(sendConfirm, "%d", 1);
						sendMessage_C(socketFD, sendConfirm);

						hours = atoi(gameData);
						// (Optional ?) SEND: confirmation

						//RECEIVE int_1 into minutes
						memset(gameData, '\0', sizeof gameData);
						receiveMessage_C(socketFD, gameData);

						memset(sendConfirm, '\0', sizeof sendConfirm);
						sprintf(sendConfirm, "%d", 1);
						sendMessage_C(socketFD, sendConfirm);

						minutes = atoi(gameData);
						// (Optional ?) SEND: confirmation

						//RECEIVE int_1 into seconds
						memset(gameData, '\0', sizeof gameData);
						receiveMessage_C(socketFD, gameData);

						memset(sendConfirm, '\0', sizeof sendConfirm);
						sprintf(sendConfirm, "%d", 1);
						sendMessage_C(socketFD, sendConfirm);

						seconds = atoi(gameData);
						// (Optional ?) SEND: confirmation
						/**** END RECEIVE TIME  ****/

						string output; ostringstream timeDisplay, livesDisplay, scoreDisplay;

						//Set time ostringstream
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

						scoreInfo.hours = hours;
						scoreInfo.minutes = minutes;
						scoreInfo.seconds = seconds;

						//Setup life count ostringstream 
						livesDisplay.clear();
						livesDisplay << "Lives: " << cube->getCubeLives() << "   ";
						/* if(gameMode == EASY) livesDisplay << "Lives: " << 5;
						else if(gameMode == NORMAL) livesDisplay << "Lives: " << 4;
						else if(gameMode == HARD) livesDisplay << "Lives: " << 3; */

						//Setup score ostreamstring
						scoreDisplay.clear();
						scoreDisplay << "Score: " << cube->getCubeScore();
						
						/**** RECEIVE SCROLL LOCK  ****/
						//RECEIVE int_1 into seconds
						memset(gameData, '\0', sizeof gameData);
						receiveMessage_C(socketFD, gameData);

						memset(sendConfirm, '\0', sizeof sendConfirm);
						sprintf(sendConfirm, "%d", 1);
						sendMessage_C(socketFD, sendConfirm);

						scrollLock = atoi(gameData);
						// (Optional ?) SEND: confirmation
						/**** END SCROLL LOCK****/
						
						//If space world, prepare scroll lock indicator
						string scrollLockIndicator;
						if(typeid(*world) == typeid(Space)) {
							if(scrollLock)
								scrollLockIndicator = "Scroll Direction Locked. Press 'u' to Unlock.";
							else
								scrollLockIndicator = "Scroll Direction Unlocked. Press 'l' to Lock.";
						}
						else
							scrollLockIndicator = "";
						
						//Render scroll lock indicator
						attron(COLOR_PAIR(YELLOW_BLACK));
						mvhline(LINES - 1, 0, ' ', COLS);
						mvaddstr(LINES - 1, 10, scrollLockIndicator.c_str());
						
						//Render game stats display
						output = string(timeDisplay.str().c_str())  + "   " +
								 string(scoreDisplay.str().c_str()) + "   " +
								 string(livesDisplay.str().c_str());
						attron(COLOR_PAIR(YELLOW_BLACK));
						mvhline(LINES - 1, 10 + scrollLockIndicator.length(), ' ', COLS);
						mvaddstr(LINES - 1, COLS - output.length() - 10, output.c_str());
						refresh();

						/**** RECEIVE GAME STATS RENDER CONFIRM REQUEST ****/
						//memset(gameData, '\0', sizeof gameData);
						//receiveMessage_C(socketFD, gameData);

						//memset(sendConfirm, '\0', sizeof sendConfirm);
						//sprintf(sendConfirm, "%d", 1);
						//sendMessage_C(socketFD, sendConfirm);
						/**** END RECEIVE WORLD RENDER CONFIRM REQUEST ****/
						
						//Allow user input processing before next render
						omp_unset_lock(&userInputLock);
						usleep(1000);

				}
				//Reset screen size constants
				LINES = original_LINES;
				COLS = original_COLS;
			}
		}
	}

	scoreInfo.secondName = secondName;
	scoreInfo.playerNum = playerNum;
	scoreInfo.finalScore = cube->getCubeScore();
	return scoreInfo;
}


// References
// https://stackoverflow.com/questions/2396430/how-to-use-lock-in-openmp
// https://stackoverflow.com/questions/7898215/how-to-clear-input-buffer-in-c/9750394
