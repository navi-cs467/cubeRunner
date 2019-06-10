# cubeRunner
Capstone Project - CS467 - Spring 2019 - Group: NAVI - Oregon State University

## Description

Cube Runner is a terminal-based, 2D Endless Runner game written in the C++ programming language that supports 1 or 2 players. It utilizes the Sockets API for multiplayer functionality and the Ncurses library to display the game in a terminal window. The game also uses the OpenMP API for multithreading. In the Cube Runner 2D endless runner game, the player token is a ASCII cube graphic which the player(s) navigate through three unique worlds. The worlds include Water, Land, and Space, each of which has different player token movement behavior and a different set of stationary and moving obstacles. The goal of the game is to maximize point accumulation by collecting mini-cubes and destroying moving obstacles while avoiding obstacles. Collision with obstacles results in loss of life. The game continues until the player(s) loses all of their lives. 


## Installation Instructions
For a pdf copy of these instructions and a more detailed overview of the game, see the file CS467_Instructions_Document.pdf

1. Extract the submitted zip file to the OSU flip server.
2. To compile, enter the following command from the terminal line: ```make all```
3. OSU flip server connection requirements:  
   1. Single player mode requires one terminal window (of minimum size 135x32) connected to the OSU flip engineering servers (flip1, flip2, or flip3). Enter the following command from the terminal line: ```cubeRunner```  
   2. Two player mode requires three terminal windows, connected to the OSU flip engineering servers (flip1, flip2, or flip3). In one console, run command: ```cubeRunnerServer AAAAA BBBBB``` (where AAAAA and BBBBB are valid port numbers) and in the other two consoles, run command:```cubeRunner```. Besides this startup, no further actions will be taken in the console running cubeRunnerServer. The server requires a SIGINT signal (ctrl-c) to exit and will otherwise run continuously, restarting itself after each 2 player game.  
    ***NOTE: The multiplayer version of the game renders best when both player terminal windows are the same size.***
  4. To navigate the game menus, use the up/down arrow keys to change the selected option and press Enter to confirm selection. Follow instructions on the displayed menus for further menu navigation. For menus requesting text information, press enter to submit/continue or move to the next line.  
     1. For two player mode, you will be asked to enter the hostname/IP address of the server (flip1, flip2, or flip3) and data port number (AAAAA for both clients).  
     ***NOTE: Both players must enter this information in order to connect to the server.***  
     2. (recommended) Visit the Instructions page from the Main Menu for game objectives, game controls, and scoring.
  5. Game play will begin, command basic player token movement using the arrow keys or wasd keys (see the in game instructions or CS467_Instructions_Document.pdf for more info on game controls)
     1. For two player mode, by default, Player 1 controls only vertical movement and Player 2 controls only horizontal movement. All other game controls are available to both players. (To enable dual axis control for both players in multiplayer mode, set the constant MULTIPLAYER_DUAL_AXIS_CONTROL in constants.hpp to a non-zero value.)
     2. Follow instructions on any transitional menus that appear or at the very bottom of the console during game play, such as   “Press enter to continue.”
     3. Once the game has started, press any of the ESC or END or q or Q keys to end the game and return to the main game menu.
  6. When game play is over, the main game menu will be presented.
  7. (optional) To delete directories, executables, and textfiles
  created during compilation of the program run the following command from the command-line: ```make clean```
  
## Game Controls
***NOTE: THE GAME IS NOT COMPATIBLE WITH NUMPAD KEYS, AND THEY MAY CAUSE UNDEFINED BEHAVIOR***  

w or up arrow key - move up  
s or down arrow key- move down  
a or left arrow key- move left   
d or right arrow key- move right   
e - left up diagonal (single player only)  
r - right up diagonal (single player only)  
c - left down diagonal (single player only)  
v - right down diagonal (single player only)  
spacebar - fire shot  
l - lock orientation (Space world only)  
u - unlock orientation (Space world only)  
p - pause/unpause game (MULTIPLAYER ONLY: Cube will reset and moving Obstacles may advance beyond their current positions when resumed.)  
Q or q or END or ESC key- quit the game and return to main menu  
t - transition to next world (testing/grading purposes only)  

