CC=g++
SRC=*.cpp
EXEC=prog
OS := $(shell uname)
INCLUDE=
FRAMEWORK=
ifeq ($(OS),Darwin)
FLAGS = -D APPLE
LIBS=-lGLEW -lglfw -lassimp
INCLUDE=-I include -I/sw/include -I/usr/local/include
FRAMEWORK=-framework BulletDynamics -framework LinearMath -framework BulletCollision -framework OpenGL -framework IOKit -framework CoreVideo
else
LIBS=-lGL -lGLEW -lglfw -lassimp -lglut
endif


all:
	${CC} ${FLAGS} ${FRAMEWORK} ${SRC} ${LIBS} -o ${EXEC} ${INCLUDE}
	# @read -p "Enter Argument:" n; \
	./prog $$n;
	./prog
	# make clear

clear:
	rm bin/prog
	rm gl.log
	rm log/*
