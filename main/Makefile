CC=g++
SRC=src/*.cpp src/utils/*.cpp
EXEC=bin/prog
OS := $(shell uname)
INCLUDE = ''
FRAMEWORK = ''
ifeq ($(OS),Darwin)
FLAGS = -D APPLE
LIBS=-lGLEW -lglfw -lassimp
INCLUDE=-I include -I/sw/include -I/usr/local/include
FRAMEWORK=-framework OpenGL -framework IOKit -framework CoreVideo
else
LIBS=-lGL -lGLEW -lglfw -lassimp -lglut
endif


all:
	${CC} ${FLAGS} ${FRAMEWORK} ${SRC} ${LIBS} -o ${EXEC} ${INCLUDE}
	./bin/prog
	make clear

clear:
	rm bin/*
	rm gl.log
	rm log/*
