/**********************************************************************
** Program name: cubeRunnerServer.cpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 4/23/2019
** Description: Main for Capstone Project "Cube Runner" game (server).
**				(See "cubeRunner.cpp" for client main.)
***********************************************************************/

//See constants.hpp for constant definitions
#include "../HEADER/constants.hpp"

//See using.hpp for "using" declarations
#include "../HEADER/using.hpp"

//Function prototypes
//#include "../HEADER/highlight.hpp"		//Shouldn't need
//#include "../HEADER/hostPrompt.hpp"		//Shouldn't need
//#include "../HEADER/initColors.hpp"		//Shouldn't need
//#include "../HEADER/transitionAnimation.hpp"			//Shouldn't need	//This will probably be renamed, changed a bit (i.e. parameter added to specify file name) and then also used for transitions
//#include "../HEADER/loadGraphic.hpp"		//Shouldn't need
//#include "../HEADER/paintCubeGraphic.hpp"	//Shouldn't need
//#include "../HEADER/paintGraphic.hpp"		//Shouldn't need
//#include "../HEADER/printMenu.hpp"		//Shouldn't need
//#include "../HEADER/validateWinSize.hpp"	//Shouldn't need

//Game class
#include "../HEADER/Game.hpp"

//server network functions
#include "../HEADER/server.hpp"

/*************************** GLOBALS *********************************/

int LINES = MIN_WIN_HEIGHT;
int COLS = MIN_WIN_WIDTH;

/************************** END GLOBALS ******************************/

int main(int argc, char* argv[]) {

	srand(time(NULL));	//Seed random number generator with system time

	while(1) {
		//Optional: int gmP1, gmP2;		//"game mode player 1" & "game mode player 2"

		// Set up listener1, wait for connection

		// RECEIVE connection1 (Optional: RECEIVE gameMode into gmP1)
		// SEND connection1 "0" (1 Player connected)

		// Set up listener2, wait for connection

		// RECEIVE connection2 (Optional: RECEIVE gameMode into gmP2)
		// (Optional) If gmP1 == gmP2:
			// SEND connection1 "1" (2 Players connected - same game mode specified)
			// SEND connection2 "1" (2 Players connected - same game mode specified)
		// (Optional) Else If gmP1 != gmP2:
			//	Optional: SEND connection1 "2" [2 Players connected, but game mode does not match])
		//	//	Optional: SEND connection2 "2" [2 Players connected, but game mode does not match])
			//  Optional: RECEIVE connection1 "1" (Confirmation acknowledgment that game modes do not match)
			//  Optional: RECEIVE connection2 "1" (Confirmation acknowledgment that game modes do not match)

		//receive port number to start server on
		char* portNum = argv[1];

		//hold player socket FDs
		int player1, player2 = -1;

		//confirmation flag
		char confirm[2] = "0";

		//initialize server and accept connections
		//also send confirmation messages to players when they are connected
		initServer(portNum, &player1, &player2);
		
		if(DEBUG)
			printf("Both Connections Established...\n");
		
		//confirm both players are connected and check game modes
		sendMessage_S(player1, confirm);

		memset(confirm, '\0', sizeof confirm);
		sprintf(confirm, "%d", 1);

		sendMessage_S(player2, confirm);
		
		if(DEBUG)
			printf("Both Confirmations Sent...\n");

		//receive game mode from player1
		char gm1Str[2];
		receiveMessage_S(player1, gm1Str);

		//receive game mode from player2
		char gm2Str[2];
		receiveMessage_S(player2, gm2Str);
		
		printf("Received Both Game Modes...\n");

		int gmP1 = atoi(gm1Str);
		int gmP2 = atoi(gm2Str);

		if (gmP1 == gmP2)
		{
			//send indicator to players
			memset(confirm, '\0', sizeof confirm);
			sprintf(confirm, "%d", 1);
			sendMessage_S(player1, confirm);
			sendMessage_S(player2, confirm);
			if(DEBUG)
				printf("Sent Game Mode Indicator (GM Match)...\n");
		}
		
		printf("After sending GM Match Loop (if applicable)...\n");

		char message[256];
		memset(message, '\0', sizeof message);

		if (gmP1 != gmP2)
		{
			//send indicator to players
			//send indicator to players
			memset(confirm, '\0', sizeof confirm);
			sprintf(confirm, "%d", 2);
			sendMessage_S(player1, confirm);
			sendMessage_S(player2, confirm);
			if(DEBUG)
				printf("Sent Game Mode Indicator (GM No Match)...\n");

			//receive confirmation back from each player
			//leaving out for now, might add later

		}
		
		if(DEBUG)
			printf("After sending GM No Match Loop (if applicable)...\n");
		
		//Initialize gameMode
		int gameMode;
		gmP1 < gmP2 ? gameMode = gmP1 : gameMode = gmP2;			//If no match, use easiest of two modes specified
		
		//Initialize world
		World *world = new Water(gameMode, true, true);
		
		if(DEBUG) 
			printf("Initial World Instantiated...\n");
		
		//Initialize cube
		Cube *cube;
		if(gameMode == EASY) cube = new Cube(world, 5);
		else if(gameMode == NORMAL) cube = new Cube(world, 4);
		else if(gameMode == HARD) cube = new Cube(world, 3);	

		if(DEBUG) 
			printf("Cube Instantiated...\n");
		
		//Set number of omp threads		//2 for user inputs, 1 for game engine
		omp_set_num_threads(3);
		
		if(DEBUG)
			printf("After Thread Declaration...\n");

		//Lock needed so threads do not update the same
		//data at the same time
		omp_lock_t lock1;

		//Initialize lock
		omp_init_lock(&lock1);

		//Used to ensure user sees every move input
		//(so cube doesn't appear to "skip" around)
		//if input is coming in too fast
		bool renderedLastMv1 = true, renderedLastMv2 = true, deathFlag = false;
		int userInput1, userInput2;
		
		if(DEBUG)
			printf("Instantiating Game and starting...\n"); fflush(stdout);
		
		#pragma omp parallel sections shared(deathFlag, lock1, \
											 renderedLastMv1, renderedLastMv2, userInput1, userInput2)
		{
			//Thread (1) for updating userInput1 and cube position
			#pragma omp section
			{
				//Variable for user 1 input
				char confirm[256];
				memset(confirm, '\0', sizeof(confirm));
				char messageToReceive[256];
				memset(messageToReceive, '\0', sizeof(messageToReceive));

				while (userInput1 != 'q') {

					// Blocks here waiting for input

					// RECEIVE int_1 into userInput1 from connection1;
					receiveMessage_S(player1, messageToReceive);
					//convert to int
					userInput1 = atoi(messageToReceive);

					//reset char array
					memset(messageToReceive, '\0', sizeof(messageToReceive));

					// (Optional ?) SEND Confirmation Connection1
					sprintf(confirm, "%d", 1);
					sendMessage_S(player1, confirm);

					while(renderedLastMv1 == false){}

					//Will block here if other player input thread is doing the same thing
					//Or if data is being sent (Ensures players see every move they input)
					omp_set_lock(&lock1);

					if((userInput1 == KEY_UP || userInput1 == '8' || userInput1 == 'w')
							&& cube->getCubeCoords()[0][0] > 0){
						cube->updateCubePosition(0, 0, 0, 1);
						cube->setCubeDirection(up);
					}
					else if((userInput1 == KEY_DOWN || userInput1 == '2' || userInput1 == 's') &&
								cube->getCubeCoords()[15][0] < world->getBottomRow()) {
						cube->updateCubePosition(0, 0, 1, 0);
						cube->setCubeDirection(down);
					}
					else if(userInput2 == 32) {
						cube->fireShot();
					}

					renderedLastMv1 = false;
					omp_unset_lock(&lock1);
				}
			}

			//Thread (2) for updating userInput2 and cube position
			#pragma omp section
			{
				//Variable for user 2 input
				char confirm[256];
				memset(confirm, '\0', sizeof(confirm));
				char messageToReceive[256];
				memset(messageToReceive, '\0', sizeof(messageToReceive));

				while (userInput2 != 'q') {

					// Blocks here waiting for input
					// RECEIVE int_1 into userInput2 from connection2;
					receiveMessage_S(player2, messageToReceive);
					//convert to int
					userInput2 = atoi(messageToReceive);

					//reset char array
					memset(messageToReceive, '\0', sizeof(messageToReceive));

					// (Optional ?) SEND: Confirmation Connection2
					sprintf(confirm, "%d", 1);

					sendMessage_S(player2, confirm);

					while(renderedLastMv2 == false){}

					//Will block here if other player input thread is doing the same thing to the same data
					//Or if data is being sent (Ensures players see every move they input)
					omp_set_lock(&lock1);

					if((userInput2 == KEY_RIGHT || userInput2 == '6' || userInput2 == 'd') &&
								cube->getCubeCoords()[15][1] < COLS) {
						cube->updateCubePosition(1, 0, 0, 0);
						cube->setCubeDirection(right);
					}
					else if((userInput2 == KEY_LEFT || userInput2 == '4' || userInput2 == 'a') &&
							cube->getCubeCoords()[0][1] > 0) {
						cube->updateCubePosition(0, 1, 0, 0);
						cube->setCubeDirection(left);
					}
					else if(userInput2 == 32) {
						cube->fireShot();
					}

					renderedLastMv2 = false;
					omp_unset_lock(&lock1);
				}
			}

			//Thread (3) for game engine
			#pragma omp section
			{
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

				//Timer Variables
				double lastScrollTime = omp_get_wtime(),
					   lastMoveTime = omp_get_wtime(),
					   lastRefreshTime = omp_get_wtime(),
					   lastNewObsTime = omp_get_wtime(),
					   lastShotTime = omp_get_wtime();
				int statsTime, startTime, scrollCount = 0,
					seconds = 0, minutes = 0, hours = 0;
				bool startTimeLogged = false;

				//Flag to indicate world transition, int to indicate next world type
				bool isNewWorldFlag = false; int newWorldType;

				//Main game engine loop
				while (1) {
					char messageToSend[256]; char clientConfirm[10];
					memset(clientConfirm, '\0', sizeof clientConfirm);
					memset(clientConfirm, '\0', sizeof clientConfirm);
					/**** SEND EARLY TERMINATION STATUS ****/
					//If either user quits, report back early termination to other player,
					//and score to both, then break
					if(userInput1 == 'q') {
						//SEND Connection1 score
						sprintf(messageToSend, "%d", cube->getCubeScore());
						sendMessage_S(player1, messageToSend);

						//CLOSE CONNECTION1
						close(player1);

						//SEND Connection2 "ET"		//Early Termination
						memset(messageToSend, '\0', sizeof messageToSend);
						strcat(messageToSend, "ET");
						sendMessage_S(player2, messageToSend);
						//(Optional ?) RECEIVE confirmation Connection2
						//SEND Connection2 score
						memset(messageToSend, '\0', sizeof messageToSend);
						sprintf(messageToSend, "%d", cube->getCubeScore());
						sendMessage_S(player2, messageToSend);

						//CLOSE CONNECTION2
						close(player2);
						break;
					}
					//Otherwise report no early termination to other player
					else {
						//SEND Connection 2 "NT"	//No Early Termination
						strcat(messageToSend, "NT");
						sendMessage_S(player2, messageToSend);
						//(Optional ?) RECEIVE Confirmation Connection2
					}

					if(userInput2 == 'q') {
						//SEND Connection2 score
						//CLOSE CONNECTION2
						close(player2);
						//SEND Connection1 "ET"		//Early Termination
						//(Optional ?) RECEIVE confirmation Connection1
						memset(messageToSend, '\0', sizeof messageToSend);
						strcat(messageToSend, "ET");
						sendMessage_S(player1, messageToSend);
						//SEND Connection1 score
						memset(messageToSend, '\0', sizeof messageToSend);
						sprintf(messageToSend, "%d", cube->getCubeScore());
						sendMessage_S(player2, messageToSend);
						//CLOSE CONNECTION1
						close(player1);
						break;
					}
					//Otherwise report no early termination to other player
					else {
						//SEND Connection 1 "NT"	//No Early Termination
						strcat(messageToSend, "NT");
						sendMessage_S(player1, messageToSend);
						//(Optional ?) RECEIVE Confirmation Connection1
					}
					/**** END SEND EARLY TERMINATION STATUS ****/

					if(omp_get_wtime() - lastRefreshTime > REFRESH_RATE) {
						lastRefreshTime = omp_get_wtime();

						// World transition if cube->transitionCount
						//reaches TRANSITION_SCORE_INTERVAL

						if(cube->getCubeTransitionScore() >= TRANSITION_SCORE_INTERVAL) {

							//Delete all Obstacles
							for(list<Obstacle*>::iterator it = world->getObstacles().begin();
							it != world->getObstacles().begin(); it++) {
								delete *it;
							}

							// //Delete and create new world
							// if(typeid(*world) == typeid(Water)) {
							// 	delete world;
							// 	world = new Land(gameMode, isTwoPlayer);
							// 	newWorldType = 1;
							// }
							// else if(typeid(*world) == typeid(Land)) {
							// 	delete world;
							// 	world = new Space(gameMode, isTwoPlayer);
							// 	newWorldType = 2;
							// }
							// else if(typeid(*world) == typeid(Space)) {
							// 	delete world;
							// 	world = new Water(gameMode, isTwoPlayer);
							// 	newWorldType = 3;
							// }

							isNewWorldFlag = true;

							//If score is less than 3000, increase scroll and move time intervals by a constant
							//(This is the point at which all three worlds have been cycled 3 times each,
							// and the speeds are capped.)
							scrollRate *= SCROLL_MOVE_DECREASE_RATE;
							moveRate *= SCROLL_MOVE_DECREASE_RATE;

							//Reset cubes position to left-middle starting point
							cube->cubeReset(world);

							//Reset transitionCount
							cube->resetCubeTransitionScore();
						}

						// ** COMMS WITH CLIENTS **

						omp_set_lock(&lock1);	//Block here if updating cube parameters via playerInputs,
												//then lock out input threads from updating until finished

						//Check for death
						cube->checkCubeCollision(world);
						deathFlag = cube->getCubeIsDead();

						/**** SEND DEATH FLAG ****/
						// SEND Connection1: deathFlag
						// if(deathFlag) {
						//
						// 	if(cube->getCubeLives() == 0) {
						// //		SEND Connection1: 1		//Game over, no need for confirmation
						// //		CLOSE Connection1
						// 	}
						// 	else {
						// //		SEND Connection1: 0			//Death but no game over
						// //		(Optional ?) RECEIVE connection1: confirmation		//Probably not optional, need to wait for death animation
						// 	}
						// }
						// else {
						// //	(Optional ?) RECEIVE connection1: confirmation		//No Death
						// }

						//reset variable
						memset(messageToSend, '\0', sizeof messageToSend);
						sprintf(messageToSend, "%d", 0);

						if(deathFlag)
						{
							memset(messageToSend, '\0', sizeof messageToSend);
							sprintf(messageToSend, "%d", 1);
						}

						//send to both connections
						sendMessage_S(player1, messageToSend);
						sendMessage_S(player2, messageToSend);

						// SEND Connection2: deathFlag
						if(deathFlag) {
							if(cube->getCubeLives() == 0) {
						//		SEND Connection2: 1		//Game over, no need for confirmation
							//send to both connections
								sendMessage_S(player1, messageToSend);
								sendMessage_S(player2, messageToSend);
						//		CLOSE Connection2
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
							}
							else {
								//Reset player position
								world->resetPlayer(cube);
						//		SEND Connection2: 0		//Death but no game over
								memset(messageToSend, '\0', sizeof messageToSend);
								sprintf(messageToSend, "%d", 0);
								sendMessage_S(player1, messageToSend);
								sendMessage_S(player2, messageToSend);

						//	 	(Optional ?) RECEIVE connection2: confirmation		//Probably not optional, need to wait for death animation
								receiveMessage_S(player1, clientConfirm);
								memset(clientConfirm, '\0', sizeof clientConfirm);
								receiveMessage_S(player2, clientConfirm);
							}
						}
						else {
							//No deathFlag, process cubeShot
							cube->processShot();
						//	(Optional ?) RECEIVE connection1: confirmation			//No Death
						}
						/**** END SEND DEATH FLAG ****/

						/**** SEND CUBE DATA ****/
						// SEND connection1: cube->getCubeCoords()[array], cube->getCubeChars()[array],
											 // cube->getCubeLives, cube->getCubePositionRow()
											 // cube->getCubePositionCol(), cube->getShotCoords()

						//send lives to clients
						memset(messageToSend, '\0', sizeof messageToSend);
						sprintf(messageToSend, "%d", cube->getCubeLives());
						sendMessage_S(player1, messageToSend);
						sendMessage_S(player2, messageToSend);

						//send row to clients
						memset(messageToSend, '\0', sizeof messageToSend);
						sprintf(messageToSend, "%d", cube->getCubePositionRow());
						sendMessage_S(player1, messageToSend);
						sendMessage_S(player2, messageToSend);

						//send col to clients
						memset(messageToSend, '\0', sizeof messageToSend);
						sprintf(messageToSend, "%d", cube->getCubePositionCol());
						sendMessage_S(player1, messageToSend);
						sendMessage_S(player2, messageToSend);

						//send cubeChars

						//buffers for character array
						char cubeCharsBuff[256]; char cubeCharBuff[2];

						memset(cubeCharsBuff, '\0', sizeof cubeCharsBuff);

						//send array by concatenating characters onto buffer
						for (int i = 0; i < CUBE_CHARS_HEIGHT; i++)
						{
							for (int j = 0; j < CUBE_CHARS_WIDTH; j++)
							{
								memset(cubeCharBuff, '\0', sizeof cubeCharBuff);
								//move(20, 5); printw(
								//create strings from characters in array
								sprintf(cubeCharBuff, "%c", cube->getCubeChars()[i][j]);
								strcat(cubeCharsBuff, cubeCharBuff);
							}
						}

						//ADD cube->shotCoords HERE TO cubeCharsBuff,
						//GET x WITH cube->getShotCoords().first
						//GET y WITH cube->getShotCoords().second...

						//send the whole buffer to both clients
						sendMessage_S(player1, cubeCharsBuff);
						sendMessage_S(player2, cubeCharsBuff);

						// (Optional ?) RECEIVE connection1: confirmation

						// SEND connection2: cube->getCubeCoords()[array], cube->getCubeChars()[array],
											 // cube->getCubeLives, cube->getCubePositionRow()
											 // cube->getCubePositionCol()
						// (Optional ?) RECEIVE connection2: confirmation
						/**** END SEND CUBE DATA ****/

						/**** SEND GAME SCORE ****/
						// SEND connection1: cube->getCubeScore()
						memset(messageToSend, '\0', sizeof messageToSend);
						sprintf(messageToSend, "%d", cube->getCubeScore());
						sendMessage_S(player1, messageToSend);
						sendMessage_S(player2, messageToSend);

						// (Optional ?) RECEIVE connection1: confirmation

						// SEND connection2: cube->getCubeScore()
						// (Optional ?) RECEIVE connection2: confirmation
						/**** END SEND GAME SCORE ****/

						/**** SEND NEW WORLD INDICATOR AND (IF APPLICABLE) TYPE  ****/

						// SEND connection1: isNewWorldFlag
						// (Optional ?) RECEIVE connection1: confirmation

						if (isNewWorldFlag)
						{
							//send flag to both clients
							memset(messageToSend, '\0', sizeof messageToSend);
							sprintf(messageToSend, "%d", 1);
							sendMessage_S(player1, messageToSend);
							sendMessage_S(player2, messageToSend);

							//send new world type to clients
							memset(messageToSend, '\0', sizeof messageToSend);
							sprintf(messageToSend, "%d", newWorldType);
							sendMessage_S(player1, messageToSend);
							sendMessage_S(player2, messageToSend);

							//receive confirmation back from both clients
							memset(clientConfirm, '\0', sizeof clientConfirm);
							receiveMessage_S(player1, clientConfirm);

							memset(clientConfirm, '\0', sizeof clientConfirm);
							receiveMessage_S(player2, clientConfirm);
						}

						else
						{
							//let both clients know there is no new world
							memset(messageToSend, '\0', sizeof messageToSend);
							sprintf(messageToSend, "%d", 0);
							sendMessage_S(player1, messageToSend);
							sendMessage_S(player2, messageToSend);
						}


						// If isNewWorldFlag == true:
						//	  SEND connection1: newWorldType
						// (Optional ?) RECEIVE connection1: confirmation	//Probably not optional, need to wait for world transition animation

						// SEND connection2: isNewWorldFlag
						// (Optional ?) RECEIVE connection2: confirmation
						// If isNewWorldFlag == true:
						//	  SEND connection2: newWorldType
						// (Optional ?) RECEIVE connection2: confirmation	//Probably not optional, need to wait for world transition animation
						/**** END SEND NEW WORLD INDICATOR AND (IF APPLICABLE) TYPE  ****/

						//Reset isNewWorldFlag if necessary
						if(isNewWorldFlag) isNewWorldFlag = false;

						/**** SEND ONSCREEN OBSTACLES  ****/
						list<Obstacle*>::iterator itObs;

						//Count how many Obstacles are on screen
						int onScreenCount = 0;
						for(itObs = world->getObstacles().begin();
							itObs != world->getObstacles().end();
							itObs++) {
								if(((*itObs)->getPosY() >= 0) &&
								   ((*itObs)->getPosY() < COLS) &&
								   ((*itObs)->getPosX() >= 0) &&
								   ((*itObs)->getPosX() <= world->getBottomRow()))
									onScreenCount++;
						}

						// SEND connection1: onScreenCount;
						// send total number of obstacles
						// (Optional ?) RECEIVE connection1: confirmation
						memset(messageToSend, '\0', sizeof messageToSend);
						sprintf(messageToSend, "%d", onScreenCount);
						sendMessage_S(player1, messageToSend);

						// loop through obstacles
						for(itObs = world->getObstacles().begin();
							itObs != world->getObstacles().end();
							itObs++) {
								if(((*itObs)->getPosY() >= 0) &&
								   ((*itObs)->getPosY() < COLS) &&
								   ((*itObs)->getPosX() >= 0) &&
								   ((*itObs)->getPosX() <= world->getBottomRow())) {
								if(typeid(**itObs) == typeid(Seaweed))
								{
									// SEND connection1: 1
									memset(messageToSend, '\0', sizeof messageToSend);
									sprintf(messageToSend, "%d", 1);
									sendMessage_S(player1, messageToSend);
								}

								else if(typeid(**itObs) == typeid(Coral))
								{
									// SEND connection1: 2
									memset(messageToSend, '\0', sizeof messageToSend);
									sprintf(messageToSend, "%d", 2);
									sendMessage_S(player1, messageToSend);
								}

								else if(typeid(**itObs) == typeid(Shark))
								{
									// SEND connection1: 3
									memset(messageToSend, '\0', sizeof messageToSend);
									sprintf(messageToSend, "%d", 3);
									sendMessage_S(player1, messageToSend);
								}

								else if(typeid(**itObs) == typeid(Octopus))
								{
									// SEND connection1: 4
									memset(messageToSend, '\0', sizeof messageToSend);
									sprintf(messageToSend, "%d", 4);
									sendMessage_S(player1, messageToSend);
								}

								/* else if(typeid(**itObs) == typeid(Tree))
								// SEND connection1: 5
								{
								// SEND connection1: 5
								memset(messageToSend, '\0', sizeof messageToSend);
								sprintf(messageToSend, "%d", 5);
								sendMessage_S(player1, messageToSend);
							}
								else if(typeid(**itObs) == typeid(Rock))
								{
								// SEND connection1: 6
								memset(messageToSend, '\0', sizeof messageToSend);
								sprintf(messageToSend, "%d", 6);
								sendMessage_S(player1, messageToSend);
							}
								=
								else if(typeid(**itObs) == typeid(Bird))
								{
								// SEND connection1: 7
								memset(messageToSend, '\0', sizeof messageToSend);
								sprintf(messageToSend, "%d", 7);
								sendMessage_S(player1, messageToSend);
							}

								else if(typeid(**itObs) == typeid(Bat))
								{
								// SEND connection1: 8
								memset(messageToSend, '\0', sizeof messageToSend);
								sprintf(messageToSend, "%d", 8);
								sendMessage_S(player1, messageToSend);
							}
						9
								else if(typeid(**itObs) == typeid(Asteroid))
								{
								// SEND connection1: 9
								memset(messageToSend, '\0', sizeof messageToSend);
								sprintf(messageToSend, "%d", 9);
								sendMessage_S(player1, messageToSend);
							}

								else if(typeid(**itObs) == typeid(Planet))
								{
								// SEND connection1: 10
								memset(messageToSend, '\0', sizeof messageToSend);
								sprintf(messageToSend, "%d", 10);
								sendMessage_S(player1, messageToSend);
							}

								else if(typeid(**itObs) == typeid(Comet))
								{
								// SEND connection1: 11
								memset(messageToSend, '\0', sizeof messageToSend);
								sprintf(messageToSend, "%d", 11);
								sendMessage_S(player1, messageToSend);
							}

								else if(typeid(**itObs) == typeid(Spaceship))
								{
								// SEND connection1: 12
								memset(messageToSend, '\0', sizeof messageToSend);
								sprintf(messageToSend, "%d", 12);
								sendMessage_S(player1, messageToSend);
							}
								*/

								// SEND connection1: (*itObs)->getPosX(),
								//					 (*itObs)->getPosY(),
								//					 (*itObs)->getGT(),
								//					 (*itObs)->getGTS()

								//posX
								memset(messageToSend, '\0', sizeof messageToSend);
								sprintf(messageToSend, "%d", (*itObs)->getPosX());
								sendMessage_S(player1, messageToSend);

								// posY
								memset(messageToSend, '\0', sizeof messageToSend);
								sprintf(messageToSend, "%d", (*itObs)->getPosY());
								sendMessage_S(player1, messageToSend);

								// gt
								memset(messageToSend, '\0', sizeof messageToSend);
								sprintf(messageToSend, "%d", (*itObs)->getGT());
								sendMessage_S(player1, messageToSend);

								// gts
								memset(messageToSend, '\0', sizeof messageToSend);
								sprintf(messageToSend, "%d", (*itObs)->getGTS());
								sendMessage_S(player1, messageToSend);

								// (Optional ?) RECEIVE connection1: confirmation
							}
						}

						// SEND connection2: world->getObstacles().size();
						// (Optional ?) RECEIVE connection1: confirmation
						memset(messageToSend, '\0', sizeof messageToSend);
						sprintf(messageToSend, "%d", onScreenCount);
						sendMessage_S(player2, messageToSend);

						// loop through obstacles
						for(itObs = world->getObstacles().begin();
							itObs != world->getObstacles().end();
							itObs++) {
								if(((*itObs)->getPosY() >= 0) &&
								   ((*itObs)->getPosY() < COLS) &&
								   ((*itObs)->getPosX() >= 0) &&
								   ((*itObs)->getPosX() <= world->getBottomRow())) {
								if(typeid(**itObs) == typeid(Seaweed))
								{
									// SEND connection2: 1
									memset(messageToSend, '\0', sizeof messageToSend);
									sprintf(messageToSend, "%d", 1);
									sendMessage_S(player2, messageToSend);
								}

								else if(typeid(**itObs) == typeid(Coral))
								{
									// SEND connection2: 2
									memset(messageToSend, '\0', sizeof messageToSend);
									sprintf(messageToSend, "%d", 2);
									sendMessage_S(player2, messageToSend);
								}

								else if(typeid(**itObs) == typeid(Shark))
								{
									// SEND connection2: 3
									memset(messageToSend, '\0', sizeof messageToSend);
									sprintf(messageToSend, "%d", 3);
									sendMessage_S(player2, messageToSend);
								}

								else if(typeid(**itObs) == typeid(Octopus))
								{
									// SEND connection2: 4
									memset(messageToSend, '\0', sizeof messageToSend);
									sprintf(messageToSend, "%d", 4);
									sendMessage_S(player2, messageToSend);
								}

								/* else if(typeid(**itObs) == typeid(Tree))
								// SEND connection2: 5
								{
								// SEND connection2: 5
								memset(messageToSend, '\0', sizeof messageToSend);
								sprintf(messageToSend, "%d", 5);
								sendMessage_S(player2, messageToSend);
							}
								else if(typeid(**itObs) == typeid(Rock))
								{
								// SEND connection2: 6
								memset(messageToSend, '\0', sizeof messageToSend);
								sprintf(messageToSend, "%d", 6);
								sendMessage_S(player2, messageToSend);
							}
								=
								else if(typeid(**itObs) == typeid(Bird))
								{
								// SEND connection2: 7
								memset(messageToSend, '\0', sizeof messageToSend);
								sprintf(messageToSend, "%d", 7);
								sendMessage_S(player2, messageToSend);
							}

								else if(typeid(**itObs) == typeid(Bat))
								{
								// SEND connection2: 8
								memset(messageToSend, '\0', sizeof messageToSend);
								sprintf(messageToSend, "%d", 8);
								sendMessage_S(player2, messageToSend);
							}
						9
								else if(typeid(**itObs) == typeid(Asteroid))
								{
								// SEND connection2: 9
								memset(messageToSend, '\0', sizeof messageToSend);
								sprintf(messageToSend, "%d", 9);
								sendMessage_S(player2, messageToSend);
							}

								else if(typeid(**itObs) == typeid(Planet))
								{
								// SEND connection2: 10
								memset(messageToSend, '\0', sizeof messageToSend);
								sprintf(messageToSend, "%d", 10);
								sendMessage_S(player2, messageToSend);
							}

								else if(typeid(**itObs) == typeid(Comet))
								{
								// SEND connection2: 11
								memset(messageToSend, '\0', sizeof messageToSend);
								sprintf(messageToSend, "%d", 11);
								sendMessage_S(player2, messageToSend);
							}

								else if(typeid(**itObs) == typeid(Spaceship))
								{
								// SEND connection2: 12
								memset(messageToSend, '\0', sizeof messageToSend);
								sprintf(messageToSend, "%d", 12);
								sendMessage_S(player2, messageToSend);
							}
								*/

								// SEND connection2: (*itObs)->getPosX(),
								//					 (*itObs)->getPosY(),
								//					 (*itObs)->getGT(),
								//					 (*itObs)->getGTS()

								//posX
								memset(messageToSend, '\0', sizeof messageToSend);
								sprintf(messageToSend, "%d", (*itObs)->getPosX());
								sendMessage_S(player2, messageToSend);

								// posY
								memset(messageToSend, '\0', sizeof messageToSend);
								sprintf(messageToSend, "%d", (*itObs)->getPosY());
								sendMessage_S(player2, messageToSend);

								// gt
								memset(messageToSend, '\0', sizeof messageToSend);
								sprintf(messageToSend, "%d", (*itObs)->getGT());
								sendMessage_S(player2, messageToSend);

								// gts
								memset(messageToSend, '\0', sizeof messageToSend);
								sprintf(messageToSend, "%d", (*itObs)->getGTS());
								sendMessage_S(player2, messageToSend);

								// (Optional ?) RECEIVE connection2: confirmation
							}
						}
						/**** END SEND ONSCREEN OBSTACLES  ****/

						/**** SEND MINICUBES  ****/
						set<pair<int, int>>::iterator itMiniCubes;

						// SEND connection1: world->getMiniCubes().size();
						// (Optional ?) RECEIVE connection1: confirmation
						memset(messageToSend, '\0', sizeof messageToSend);
						sprintf(messageToSend, "%ld", world->getMiniCubes().size());
						sendMessage_S(player1, messageToSend);




						for(itMiniCubes = world->getMiniCubes().begin();
							itMiniCubes != world->getMiniCubes().end();
							itMiniCubes++) {
								//SEND connection1: miniCubes->first,
								//					miniCubes->second

								memset(messageToSend, '\0', sizeof messageToSend);
								sprintf(messageToSend, "%d", itMiniCubes->first);
								sendMessage_S(player1, messageToSend);

								memset(messageToSend, '\0', sizeof messageToSend);
								sprintf(messageToSend, "%d", itMiniCubes->second);
								sendMessage_S(player1, messageToSend);

								// (Optional ?) RECEIVE connection1: confirmation
						}

						// SEND connection2: world->getMiniCubes().size();
						memset(messageToSend, '\0', sizeof messageToSend);
						sprintf(messageToSend, "%ld", world->getMiniCubes().size());
						sendMessage_S(player2, messageToSend);

						// (Optional ?) RECEIVE connection2: confirmation
						for(itMiniCubes = world->getMiniCubes().begin();
							itMiniCubes != world->getMiniCubes().end();
							itMiniCubes++) {
								//SEND connection2: miniCubes->first,
								//					miniCubes->second
								memset(messageToSend, '\0', sizeof messageToSend);
								sprintf(messageToSend, "%d", itMiniCubes->first);
								sendMessage_S(player2, messageToSend);

								memset(messageToSend, '\0', sizeof messageToSend);
								sprintf(messageToSend, "%d", itMiniCubes->second);
								sendMessage_S(player2, messageToSend);

								// (Optional ?) RECEIVE connection2: confirmation
						}
						/**** END SEND MINICUBES  ****/

						/**** SEND TIME INFO  ****/
						//SEND connection1: hours
						memset(messageToSend, '\0', sizeof messageToSend);
						sprintf(messageToSend, "%d", hours);
						sendMessage_S(player1, messageToSend);

						// (Optional ?) RECEIVE connection1: confirmation
						//SEND connection1: minutes
						memset(messageToSend, '\0', sizeof messageToSend);
						sprintf(messageToSend, "%d", minutes);
						sendMessage_S(player1, messageToSend);

						// (Optional ?) RECEIVE connection1: confirmation
						//SEND connection1: seconds
						memset(messageToSend, '\0', sizeof messageToSend);
						sprintf(messageToSend, "%d", seconds);
						sendMessage_S(player1, messageToSend);

						// (Optional ?) RECEIVE connection1: confirmation

						//SEND connection2: hours
						memset(messageToSend, '\0', sizeof messageToSend);
						sprintf(messageToSend, "%d", hours);
						sendMessage_S(player1, messageToSend);
						// (Optional ?) RECEIVE connection1: confirmation

						//SEND connection2: minutes
						memset(messageToSend, '\0', sizeof messageToSend);
						sprintf(messageToSend, "%d", minutes);
						sendMessage_S(player1, messageToSend);
						// (Optional ?) RECEIVE connection2: confirmation

						//SEND connection2: seconds
						memset(messageToSend, '\0', sizeof messageToSend);
						sprintf(messageToSend, "%d", seconds);
						sendMessage_S(player1, messageToSend);

						// (Optional ?) RECEIVE connection2: confirmation
						/**** END SEND TIME INFO  ****/

						//Any move entered has been rendered
						renderedLastMv1 = renderedLastMv2 = true;

						//Done Sending - Unset Lock!
						omp_unset_lock(&lock1);
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
								< (NUM_MCS_EASY / gameMode) / 2)
							world->initMiniCubes(1);

						if(scrollCount == COLS) scrollCount = 0;
						else scrollCount++;
					}

					//Move Obstacles according to moveRate
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

					//Update time every second
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
		}
	}
	return 0;
}


// References
// http://heather.cs.ucdavis.edu/~matloff/UnixAndC/CLanguage/Curses.pdf
// http://www.cplusplus.com/reference/string/string/getline/
// https://stackoverflow.com/questions/8460666/c-does-getline-have-a-maximum-string-or-character-capacity
// http://www.cplusplus.com/reference/string/string/substr/
// https://stackoverflow.com/questions/347949/how-to-convert-a-stdstring-to-const-char-or-char
// https://www.linuxjournal.com/content/programming-color-ncurses
// https://www.linuxjournal.com/content/about-ncurses-colors-0
// https://www.youtube.com/watch?v=pjT5wq11ZSE
// https://stackoverflow.com/questions/18837836/how-can-i-hide-the-cursor-in-ncurses
// https://invisible-island.net/ncurses/howto/NCURSES-Programming-HOWTO.html
// https://stackoverflow.com/questions/6281368/multi-line-define-directives
// https://stackoverflow.com/questions/21173308/how-to-best-initialize-a-vector-of-strings-in-c
// https://stackoverflow.com/questions/1182665/curses-getting-arrow-keys
// https://stackoverflow.com/questions/11067800/ncurses-key-enter-is-fail
// https://stackoverflow.com/questions/2545720/error-default-argument-given-for-parameter-1
// https://stackoverflow.com/questions/5288036/how-to-clear-ostringstream
// https://www.gnu.org/software/guile-ncurses/manual/html_node/Getting-characters-from-the-keyboard.html
