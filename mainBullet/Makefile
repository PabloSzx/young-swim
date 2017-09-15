CC=g++
SRC=src/*.cpp
EXEC=bin/prog
OS := $(shell uname)
INCLUDE=
FRAMEWORK=
ifeq ($(OS),Darwin)
FLAGS = -D APPLE
LIBS=-lGLEW -lglfw -lassimp
INCLUDE=-I include -I/sw/include -I/usr/local/include
FRAMEWORK=-framework OpenGL -framework IOKit -framework CoreVideo -framework BulletDynamics -framework LinearMath -framework BulletCollision
else
LIBS=-lGL -lGLEW -lglfw -lassimp -lglut
endif


all:
	${CC} ${FLAGS} ${FRAMEWORK} ${SRC} ${LIBS} -o ${EXEC} ${INCLUDE}
	# @read -p "Enter Argument:" n; \
	# ./bin/prog $$n;
	./bin/prog
	# make clear

clear:
	rm bin/prog
	rm gl.log
	rm log/*
