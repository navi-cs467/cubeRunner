############################################################
## Program name: Makefile
## CS467 Capstone - 2D Runner - "Cube Runner"
## Team: NAVI
## Date: 5/18/2019
## Description: Makefile for Capstone Project "Cube Runner".
############################################################

CXX = g++
CXXFLAGS = -std=c++0x -lncursesw -lm -fopenmp \
		   -w
		   # -g -Wall -Werror -Wfatal-errors \
		   # -Wno-error=sign-compare \
		   # -Wno-error=unused-but-set-variable \
		   # -Wno-error=unused-variable --pedantic-errors \
		   # -Wno-error=maybe-uninitialized -Wno-error=comment \
		   # -Wno-error=write-strings

IDIR = HEADER
SDIR = SOURCE
ODIR = OBJECT
ODIR_SERVER = OBJECT_SERVER
GDIR = GRAPHICS

PROGRAM_NAME = cubeRunner
PROGRAM_NAME_SERVER = cubeRunnerServer

#Objects (Client/Single-Player)
_OBJ = cubeRunner.o highlight.o hostPrompt.o transitionAnimation.o \
	   transitionAnimationInsideThread.o loadGraphic.o loadInstructions.o\
	   paintCubeGraphic.o paintGraphic.o printMenu.o validateWinSize.o \
	   initColors.o Game.o World.o Water.o Land.o Space.o Obstacle.o Seaweed.o \
	   Coral.o Shark.o Octopus.o Tree.o Rock.o Bird.o Bat.o \
	   Asteroid.o Planet.o Comet.o Spaceship.o \
	   Direction.o Cube.o client.o waitingForOtherPlayer.o userPrompt.o highScore.o \
	   earlyTermTransition.o noServerAvailable.o

OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

#Objects (Server)
_OBJ_SERVER = cubeRunnerServer.o server.o client.o World.o Water.o Land.o Space.o \
			  Obstacle.o Seaweed.o Coral.o Shark.o Octopus.o Tree.o \
			  Rock.o Bird.o Bat.o Asteroid.o Planet.o Comet.o Spaceship.o \
			  Direction.o Cube.o
OBJ_SERVER = $(patsubst %,$(ODIR_SERVER)/%,$(_OBJ_SERVER))

#Headers (Client/Single-Player)
_HDR = constants.hpp highlight.hpp hostPrompt.hpp include.hpp \
	   colorDefs.hpp transitionAnimation.hpp loadGraphic.hpp loadInstructions.hpp\
	   paintCubeGraphic.hpp paintGraphic.hpp printMenu.hpp using.hpp validateWinSize.hpp \
	   initColors.hpp Game.hpp World.hpp Water.hpp Land.hpp Space.hpp Obstacle.hpp \
	   Seaweed.hpp Coral.hpp Shark.hpp Octopus.hpp Tree.hpp Rock.hpp \
	   Bird.hpp Bat.hpp Asteroid.hpp Planet.hpp Comet.hpp Spaceship.hpp \
	   Direction.hpp Cube.hpp client.hpp waitingForOtherPlayer.hpp userPrompt.hpp \
	   highScore.hpp earlyTermTransition.hpp noServerAvailable.hpp

HDR = $(patsubst %,$(IDIR)/%,$(_HDR))

#Headers (Server)
_HDR_SERVER = constants.hpp include.hpp using.hpp initColors.hpp Game.hpp World.hpp \
	   Water.hpp Land.hpp Space.hpp Obstacle.hpp Seaweed.hpp Coral.hpp Shark.hpp Octopus.hpp \
	   Tree.hpp Rock.hpp Bird.hpp Bat.hpp Asteroid.hpp Planet.hpp Comet.hpp Spaceship.hpp \
	   Direction.hpp Cube.hpp server.hpp client.hpp userPrompt.hpp

HDR_SERVER = $(patsubst %,$(IDIR)/%,$(_HDR_SERVER))

#Sources
_SRC = cubeRunner.cpp cubeRunnerServer.cpp highlight.cpp hostPrompt.cpp \
	   transitionAnimation.cpp transitionAnimationInsideThread.cpp \
	   loadGraphic.cpp loadInstructions.cpp paintCubeGraphic.cpp \
	   paintGraphic.cpp printMenu.cpp validateWinSize.cpp initColors.cpp \
	   server.cpp client.cpp Game.cpp World.cpp Water.cpp Land.cpp Space.cpp \
	   Obstacle.cpp Seaweed.cpp Coral.cpp Shark.cpp Octopus.cpp Tree.cpp \
	   Rock.cpp Bird.cpp Bat.cpp Asteroid.o Planet.o Comet.o Spaceship.o \
	   Direction.cpp Cube.cpp waitingForOtherPlayer.cpp userPrompt.cpp highScore.cpp \
	   earlyTermTransition.cpp noServerAvailable.cpp

SRC = $(patsubst %,$(SDIR)/%,$(_SRC))

$(PROGRAM_NAME): $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

$(PROGRAM_NAME_SERVER): $(OBJ_SERVER)
	$(CXX) -o $@ $^ $(CXXFLAGS)

$(ODIR)/%.o: $(SDIR)/%.cpp $(HDR)
	#@chmod +x makeOBJECTDir
	@./makeOBJECTDir
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(ODIR_SERVER)/%.o: $(SDIR)/%.cpp $(HDR_SERVER)
	#@chmod +x makeOBJECT_SOURCEDir
	@./makeOBJECT_SOURCEDir
	$(CXX) -c -o $@ $< $(CXXFLAGS)

# Alternate Syntax
# $(OBJ): $(SRC) $(HDR)
	# $(CXX) -c -o $@ $< $(CXXFLAGS)

all: $(PROGRAM_NAME) $(PROGRAM_NAME_SERVER)


run:
	./$(PROGRAM_NAME)

clean:
	rm -f -r $(OBJ) $(PROGRAM_NAME) $(ODIR)
	rm -f -r $(OBJ_SERVER) $(PROGRAM_NAME_SERVER) $(ODIR_SERVER)
	rm -f -r gameHighScores.txt

zip:
	zip $(PROGRAM_NAME).zip $(SDIR) $(HDIR) $(GDIR) Makefile \
		makeOBJECTDir makeOBJECT_SOURCEDir

# REFERENCES
# http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
# https://stackoverflow.com/questions/19475037/function-and-difference-between-and-in-makefile
# https://stackoverflow.com/questions/3220277/what-do-the-makefile-symbols-and-mean
