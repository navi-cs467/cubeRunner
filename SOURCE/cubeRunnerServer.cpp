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

/*************************** GLOBALS *********************************/

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
		
		
		Game game = Game(gmP1 < gmP2 ? gpM1 : gpM2, true);		//2nd argument == true for isTwoPlayer
																//Use easiest of two specified game modes, 
																//if not the same
		
		//Set number of omp threads		//2 for user inputs, 1 for game engine
		omp_set_num_threads(3);
		
		//Lock needed so threads do not update the same
		//data at the same time
		omp_lock_t lock1;
		
		//Initialize lock
		omp_init_lock(&lock1);
		
		//Used to ensure user sees every move input 
		//(so cube doesn't appear to "skip" around) 
		//if input is coming in too fast
		bool renderedLastMv1 = true, renderedLastMv2 = true, deathFlag = false;
		
		#pragma omp parallel sections shared(deathFlag, lock1, \
											 renderedLastMv1, renderedLastMv2)
		{
			//Thread (1) for updating userInput1 and cube position
			#pragma omp section
			{
				//Variable for user 1 input
				int userInput1;
				
				while (userInput1 != 'q') {
					
					// Blocks here waiting for input
					// RECEIVE int_1 into userInput1 from connection1;
					// (Optional ?) SEND Confirmation Connection1
					
					while(renderedLastMv1 == false){}
					
					//Will block here if other player input thread is doing the same thing
					//Or if data is being sent (Ensures players see every move they input)
					omp_set_lock(&lock1);
					
					if(userInput == KEY_UP && cube.getPosX() > 0) {
						cube.decPosX();
						cube.setDirection(up);		
					}
					else if(userInput == KEY_DOWN && cube.getPosX() < world->getBottomRow()) {
						cube.incPosX();
						cube.setDirection(down);	
					}
										
					deathFlag = cube.checkDeath();	//checkDeath should "cube.setLives(cube.getLives() - 1)" if player token hits Obstacle - Returns "true" if death has occurred
					cube.checkScore();				//checkScore should increase score by number of miniCubes contacted for this move.
													//Should also "consume miniCubes contacted, i.e. remove them from screen (i.e. erase them from set<pair<int,int>> world->miniCubes)
					
					renderedLastMv1 = false;
					omp_unset_lock(&lock1);
				}
			}
			
			//Thread (2) for updating userInput2 and cube position
			#pragma omp section
			{
				//Variable for user 2 input
				int userInput2;
				
				while (userInput2 != 'q') {
					
					// Blocks here waiting for input
					// RECEIVE int_1 into userInput2 from connection2;
					// (Optional ?) SEND: Confirmation Connection2
					
					while(renderedLastMv2 == false){}
					
					//Will block here if other player input thread is doing the same thing to the same data
					//Or if data is being sent (Ensures players see every move they input)
					omp_set_lock(&lock1);
					if(userInput == KEY_RIGHT && cube.getPosY() < COLS) {
						cube.incPosY();
						cube.setDirection(right);	//May need further synchronization here, if it problematic for both player threads to attempt updating this value at the same time
					}
					else if(userInput == KEY_LEFT && cube.getPosY() > 0) {
						cube.decPosY();
						cube.setDirection(left);	//May need further synchronization here, if it problematic for both player threads to attempt updating this value at the same time
					}
					
					deathFlag = cube.checkDeath();	//checkDeath should "cube.setLives(cube.getLives() - 1)" if player token hits Obstacle - Returns "true" if death has occurred
					cube.checkScore();				//checkScore should increase score by number of miniCubes contacted for this move.
													//Should also "consume miniCubes contacted, i.e. remove them from screen (i.e. erase them from set<pair<int,int>> world->miniCubes)
					
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
					scrollRate = 0.25;
				}
				else if(gameMode == NORMAL) {
					scrollRate = 0.75;
				}
				else {
					scrollRate = 1.5;
				}
				moveRate = scrollRate / 4.;
				
				//Timer Variables
				double lastScrollTime = omp_get_wtime(),
					   lastMoveTime = omp_get_wtime(),
					   lastRefreshTime = omp_get_wtime(),
					   lastNewObsTime = omp_get_wtime();
				int statsTime, startTime, scrollCount = 0,
					seconds = 0, minutes = 0, hours = 0;
				bool startTimeLogged = false;
				
				//Flag to indicate world transition, int to indicate next world type
				bool isNewWorldFlag = false; int newWorldType;
				
				//Main game engine loop
				while (1) {
					
					/**** SEND EARLY TERMINATION STATUS ****/
					//If either user quits, report back early termination to other player,
					//and score to both, then break
					if(userInput1 == 'q') {
						//SEND Connection1 score
						//CLOSE CONNECTION1
						//SEND Connection2 "ET"		//Early Termination
						//(Optional ?) RECEIVE confirmation Connection2
						//SEND Connection2 score
						//CLOSE CONNECTION2
						break;
					}
					//Otherwise report no early termination to other player
					else {
						//SEND Connection 2 "NT"	//No Early Termination
						//(Optional ?) RECEIVE Confirmation Connection2
					}
					
					if(userInput2 == 'q') {
						//SEND Connection2 score
						//CLOSE CONNECTION2
						//SEND Connection1 "ET"		//Early Termination
						//(Optional ?) RECEIVE confirmation Connection1
						//SEND Connection1 score
						//CLOSE CONNECTION1
						break;
					}
					//Otherwise report no early termination to other player
					else {
						//SEND Connection 1 "NT"	//No Early Termination
						//(Optional ?) RECEIVE Confirmation Connection1
					}
					/**** END SEND EARLY TERMINATION STATUS ****/
					
					if(omp_get_wtime() - lastRefreshTime > REFRESH_RATE) {
						lastRefreshTime = omp_get_wtime();
						
						// World transition if cube->transitionCount 
						//reaches TRANSITION_SCORE_INTERVAL
						if(cube->getTransitionCount() >= TRANSITION_SCORE_INTERVAL) {
							
							//Delete all Obstacles
							for(list<Obstacle*>::iterator it = world->getObstacles().begin();
							it != world->getObstacles().begin(); it++) {
								delete *it;
							}
							
							//Create new world
							if(typeid(*world) == typeid(Water))
								world = new Land(gameMode, isTwoPlayer);
							else if(typeid(*world) == typeid(Land))
								world = new Space(gameMode, isTwoPlayer);
							else if(typeid(*world) == typeid(Space))
								world = new Water(gameMode, isTwoPlayer);
							
							//If score is less than 3000, increase scroll and move time intervals by 10%
							//(This is the point at which all three worlds have been cycled 3 times each,
							// and the speeds are capped.)
							scrollRate *= 0.9;
							moveRate *= 0.9;
							
							//Reset cubes position to left-middle starting point
							cube->reset();
							
							//Reset transitionCount
							cube->setTransitionCount() = 0;
						}
							
						// ** COMMS WITH CLIENTS **
						
						omp_set_lock(&lock1);	//Block here if updating cube parameters via playerInputs,
												//then lock input threads from updating until finished
						
						/**** SEND DEATH FLAG ****/
						// SEND Connection1: deathFlag
						if(deathFlag) {
							if(cube->getLives() == 0) {
						//		SEND Connection1: 1		//Game over, no need for confirmation
						//		CLOSE Connection1
							}
							else {
						//		SEND Connection1: 0			//Death but no game over
						//		(Optional ?) RECEIVE connection1: confirmation		//Probably not optional, need to wait for death animation
							}
						}
						else {
						//	(Optional ?) RECEIVE connection1: confirmation		//No Death
						}
						
						// SEND Connection2: deathFlag
						if(deathFlag) {
							if(cube->getLives() == 0) {
						//		SEND Connection2: 1		//Game over, no need for confirmation
						//		CLOSE Connection2
								break;
							}
							else {
						//		SEND Connection2: 0		//Death but no game over	
						//	 	(Optional ?) RECEIVE connection2: confirmation		//Probably not optional, need to wait for death animation
							}
						}
						else {
						//	(Optional ?) RECEIVE connection1: confirmation			//No Death
						}
						/**** END SEND DEATH FLAG ****/
						
						/**** SEND CUBE DATA ****/
						// SEND connection1: cube->getPosX(), cube->getPosY(), cube->numLives
						// (Optional ?) RECEIVE connection1: confirmation
						
						// SEND connection2: cube->getPosX(), cube->getPosY(), cube->numLives
						// (Optional ?) RECEIVE connection2: confirmation
						/**** END SEND CUBE DATA ****/
						
						/**** SEND GAME SCORE ****/
						// SEND connection1: score
						// (Optional ?) RECEIVE connection1: confirmation
						
						// SEND connection2: score
						// (Optional ?) RECEIVE connection2: confirmation
						/**** END SEND GAME SCORE ****/
						
						/**** SEND NEW WORLD INDICATOR AND (IF APPLICABLE) TYPE  ****/
						// SEND connection1: isNewWorldFlag
						// (Optional ?) RECEIVE connection1: confirmation
						// If isNewWorldFlag == true:
						//	  SEND connection1: newWorldType
						// (Optional ?) RECEIVE connection1: confirmation	//Probably not optional, need to wait for world transition animation
						
						// SEND connection2: isNewWorldFlag
						// (Optional ?) RECEIVE connection2: confirmation
						// If isNewWorldFlag == true:
						//	  SEND connection2: newWorldType
						// (Optional ?) RECEIVE connection2: confirmation	//Probably not optional, need to wait for world transition animation
						/**** END SEND NEW WORLD INDICATOR AND (IF APPLICABLE) TYPE  ****/
						
						/**** SEND ONSCREEN OBSTACLES  ****/
						list<Obstacle*>::iterator itObs; 
						
						//Count how many Obstacles are on screen
						int onScreenCount = 0;
						for(itObs = world->getObstacles().begin(); 
							itObs < world->getObstacles().end();
							itObs++) {
								if(((*it)->getPosY() + (*it)->getLongestGS() > 0) &&
								   ((*it)->getPosY() < COLS) &&
								   ((*it)->getPosX() + (*it)->getGTS() > 0) &&
								   ((*it)->getPosY() < world->getBottomRow()))
									onScreenCount++;
						}
						
						// SEND connection1: onScreenCount;
						// (Optional ?) RECEIVE connection1: confirmation
						for(itObs = world->getObstacles().begin(); 
							itObs < world->getObstacles().end();
							itObs++) {
								if(((*it)->getPosY() + (*it)->getLongestGS() > 0) &&
								   ((*it)->getPosY() < COLS) &&
								   ((*it)->getPosX() + (*it)->getGTS() > 0) &&
								   ((*it)->getPosY() < world->getBottomRow())) {
								if(typeid(**it) == typeid(Seaweed))
								// SEND connection1: 1
								else if(typeid(**it) == typeid(Coral))
								// SEND connection1: 2
								else if(typeid(**it) == typeid(Shark))
								// SEND connection1: 3
								else if(typeid(**it) == typeid(Octopus))
								// SEND connection1: 4
								/* else if(typeid(**it) == typeid(Tree))
								// SEND connection1: 5
								else if(typeid(**it) == typeid(Rock))
								// SEND connection1: 6
								else if(typeid(**it) == typeid(Bird))
								// SEND connection1: 7
								else if(typeid(**it) == typeid(Bat))
								// SEND connection1: 8
								else if(typeid(**it) == typeid(Asteroid))
								// SEND connection1: 9
								else if(typeid(**it) == typeid(Planet))
								// SEND connection1: 10
								else if(typeid(**it) == typeid(Comet))
								// SEND connection1: 11
								else if(typeid(**it) == typeid(Spaceship))
								// SEND connection1: 12 */
							
								// SEND connection1: (*it)->getPosX(), 
								//					 (*it)->getPosY(),
								//					 (*it)->getGT(), 
								//					 (*it)->getGTS()
								
								// (Optional ?) RECEIVE connection1: confirmation
							}
						}
						
						// SEND connection2: world->getObstacles().size();
						// (Optional ?) RECEIVE connection1: confirmation
						for(itObs = world->getObstacles().begin(); 
							itObs < world->getObstacles().end();
							itObs++) {
								if(((*it)->getPosY() + (*it)->getLongestGS() > 0) &&
								   ((*it)->getPosY() < COLS) &&
								   ((*it)->getPosX() + (*it)->getGTS() > 0) &&
								   ((*it)->getPosY() < world->getBottomRow())) {
										if(typeid(**it) == typeid(Seaweed))
										// SEND connection2: 1
										else if(typeid(**it) == typeid(Coral))
										// SEND connection2: 2
										else if(typeid(**it) == typeid(Shark))
										// SEND connection2: 3
										else if(typeid(**it) == typeid(Octopus))
										// SEND connection2: 4
										/* else if(typeid(**it) == typeid(Tree))
										// SEND connection2: 5
										else if(typeid(**it) == typeid(Rock))
										// SEND connection2: 6
										else if(typeid(**it) == typeid(Bird))
										// SEND connection2: 7
										else if(typeid(**it) == typeid(Bat))
										// SEND connection2: 8
										else if(typeid(**it) == typeid(Asteroid))
										// SEND connection2: 9
										else if(typeid(**it) == typeid(Planet))
										// SEND connection2: 10
										else if(typeid(**it) == typeid(Comet))
										// SEND connection2: 11
										else if(typeid(**it) == typeid(Spaceship))
										// SEND connection2: 12 */

										// SEND connection2: (*it)->getPosX(), 
										//					 (*it)->getPosY(),
										//					 (*it)->getGT(), 
										//					 (*it)->getGTS()

										// (Optional ?) RECEIVE connection2: confirmation
							  }
						}
						/**** END SEND ONSCREEN OBSTACLES  ****/
						
						/**** SEND MINICUBES  ****/
						set<pair<int, int>>::iterator miniCubes;
						
						// SEND connection1: world->getMiniCubes().size();
						// (Optional ?) RECEIVE connection1: confirmation
						for(miniCubes = world->getMiniCubes().begin(); 
							itObsCoords < world->getMiniCubes().end();
							itObsCoords++) {
								//SEND connection1: miniCubes->first,
								//					miniCubes->second
								
								// (Optional ?) RECEIVE connection1: confirmation
						}
						
						// SEND connection2: world->getMiniCubes().size();
						// (Optional ?) RECEIVE connection2: confirmation
						for(itObsCoords = world->getObstacles().begin(); 
							itObsCoords < world->getObstacles().end();
							itObsCoords++) {
								//SEND connection2: miniCubes->first,
								//					miniCubes->second
								
								// (Optional ?) RECEIVE connection2: confirmation
						}
						/**** END SEND MINICUBES  ****/
						
						/**** SEND TIME INFO  ****/
						//SEND connection1: hours
						// (Optional ?) RECEIVE connection1: confirmation
						//SEND connection1: minutes
						// (Optional ?) RECEIVE connection1: confirmation
						//SEND connection1: seconds
						// (Optional ?) RECEIVE connection1: confirmation
						
						//SEND connection2: hours
						// (Optional ?) RECEIVE connection1: confirmation
						//SEND connection2: minutes
						// (Optional ?) RECEIVE connection2: confirmation
						//SEND connection2: seconds
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
					
					if(omp_get_wtime() - lastMoveTime > moveRate) {
						lastMoveTime = omp_get_wtime();
						world->moveObs();
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