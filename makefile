OPTFLAGS    = -fast
CC          = g++
MAKE        = make
SHELL       = /bin/sh

CFLAGS  = -std=c++0x
FOLDERS =

LIBS = -lglfw -lGL 
EXECS = viv
SOURCES = VIV.cpp
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

install: $(EXECS)
	mv $(EXECS) /usr/bin
