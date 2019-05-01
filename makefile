############################################################
## Program name: Makefile
## CS467 Capstone - 2D Runner - "Cube Runner"
## Team: NAVI
## Date: 4/23/2019
## Description: Makefile for Capstone Project "Cube Runner.
############################################################

CXX = g++
CXXFLAGS = -g -std=c++0x -Wall -Werror -Wfatal-errors \
		   -lcurses -lm -fopenmp -Wno-error=sign-compare \
		   -Wno-error=unused-but-set-variable \
		   -Wno-error=unused-variable --pedantic-errors
   
IDIR = HEADER
ODIR = OBJECT
SDIR = SOURCE

PROGRAM_NAME = cubeRunner

_OBJ = cubeRunner.o highlight.o hostPrompt.o intro.o loadGraphic.o \
	   paintCubeGraphic.o paintGraphic.o printMenu.o validateWinSize.o \
	   initColors.o server.o Game.o World.o Water.o Obstacle.o Seaweed.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_HDR = constants.hpp highlight.hpp hostPrompt.hpp include.hpp \
	   colorDefs.hpp intro.hpp loadGraphic.hpp paintCubeGraphic.hpp \
	   paintGraphic.hpp printMenu.hpp using.hpp validateWinSize.hpp \
	   initColors.hpp Game.hpp World.hpp Water.hpp Obstacle.hpp \
	   Seaweed.hpp   
HDR = $(patsubst %,$(IDIR)/%,$(_HDR))

_SRC = cubeRunner.cpp highlight.cpp hostPrompt.cpp intro.cpp \
	   loadGraphic.cpp paintCubeGraphic.cpp paintGraphic.cpp \
	   printMenu.cpp validateWinSize.cpp initColors.cpp \
	   server.cpp Game.cpp World.cpp Water.cpp Obstacle.cpp \
	   Seaweed.cpp    
SRC = $(patsubst %,$(SDIR)/%,$(_SRC))

$(PROGRAM_NAME): $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)
	
$(ODIR)/%.o: $(SDIR)/%.cpp $(HDR)
	#@chmod +x makeOBJECTDir
	@./makeOBJECTDir
	$(CXX) -c -o $@ $< $(CXXFLAGS)

# Alternate Syntax	
# $(OBJ): $(SRC) $(HDR)
	# $(CXX) -c -o $@ $< $(CXXFLAGS)
	
run:
	./$(PROGRAM_NAME)
	
clean:
	rm -f -r $(OBJ) $(PROGRAM_NAME) $(ODIR)
	
zip:
	zip $(PROGRAM_NAME).zip $(SRC) $(HDR) Makefile *.txt
	
# REFERENCES
# http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/
# https://stackoverflow.com/questions/19475037/function-and-difference-between-and-in-makefile
# https://stackoverflow.com/questions/3220277/what-do-the-makefile-symbols-and-mean