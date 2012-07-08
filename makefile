OPTFLAGS    = -fast
CC          = g++
MAKE        = make
SHELL       = /bin/sh

CFLAGS  = -std=c++0x
FOLDERS =
LIBS = -lglfw -lGL
EXECS = nine
SOURCES = NINE.cpp
HEADERS = include/polygon.h include/precision.h include/vectorND.h
OBJECTS = $(SOURCES:.cpp=.o)

$(EXECS): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LIBS)

.cpp.o: $(HEADERS)
	$(CC) -c $(SOURCES) $(LIBS)

clean:
	/bin/rm -f *.o *.mod $(EXECS) *.gnu *.sh *.gif

