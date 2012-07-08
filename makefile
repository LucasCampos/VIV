OPTFLAGS    = -fast
CC          = g++
MAKE        = make
SHELL       = /bin/sh

CFLAGS  = -std=c++0x
FOLDERS =
LIBS = -lglfw -lGL
EXECS = nine
SOURCES = NINE.cpp Circulo.cpp 
OBJECTS = $(SOURCES:.cpp=.o)

$(EXECS): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LIBS)

.cpp.o: obj/
	$(CC) -c $(SOURCES) $(LIBS)

clean:
	/bin/rm -f *.o *.mod $(EXECS) *.gnu *.sh *.gif

