CC = gcc
CFLAGS = -g -Wall -Wno-deprecated
OS = $(shell uname)
ifeq ($(OS), Darwin)
	LIBS = -framework OpenGL -framework GLUT
else ifeq ($(OS), Linux)
	LIBS = -lglut -lGLU -lGL
else
#	CC=i686-pc-mingw32-g++
#	CC=i686-w64-mingw32-g++
	CC=x86_64-w64-mingw32-g++
	LIBS = -lglut32 -lglu32 -lopengl32
#	LIBS = -lglut -lGLU -lGL
endif

gemgame.o: gemgame.cpp
	$(CC) $(CFLAGS) -c $<

gemgame: gemgame.o
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

clean: FORCE
	-rm -f *.o *~ gemgame *core*

FORCE: