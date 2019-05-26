/*****************************************************
** Program name: highScore.hpp
** CS467 Capstone - 2D Runner - "Cube Runner"
** Team: NAVI
** Date: 5/24/2019
** Description: Header file for highScore functions.
*****************************************************/

#ifndef highScore_hpp
#define highScore_hpp

#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include <omp.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <list>
#include <set>
#include <utility>
#include <limits>
#include <typeinfo>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include "colorDefs.hpp"

int isHighScore (int score);
void addScoreSingle(int score, char* name);
void addScoreMulti(int score, char* firstName, char* secondName);

#endif
