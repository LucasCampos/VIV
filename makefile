OPTFLAGS    = -fast
CC          = g++
MAKE        = make
SHELL       = /bin/sh

CFLAGS  = -std=c++0x
FOLDERS =
LIBS = -lglfw -lGL -lrt -lXrandr  -lrt -lX11 -lGLU   -lGL -pthread -lm 

EXECS = nine
SOURCES = NINE.cpp
HEADERS = include/polygon.h include/precision.h include/vectorND.h
OBJECTS = $(SOURCES:.cpp=.o)

$(EXECS): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LIBS) -O3

.cpp.o: $(HEADERS)
	$(CC) -c $(SOURCES) $(LIBS) -O3

clean:
	/bin/rm -f *.o *.mod $(EXECS) *.gnu *.sh *.gif
run: $(EXECS)
	./$(EXECS) -c transannealingRusso.dat 384 1  -b 50 -e
