CC=g++ -std=c++11
SRC=src/main.cpp src/components/camera/camera.cpp src/components/model/model.cpp src/components/physics/physics.cpp src/utils/shader/shader.cpp src/utils/log/log.cpp src/utils/window/window.cpp src/components/input/input.cpp src/utils/gaming/gaming.cpp
EXEC=bin/prog
OS := $(shell uname)
INCLUDE=
FRAMEWORK=
ifeq ($(OS),Darwin)
FLAGS = -D APPLE
LIBS=-lGLEW -lglfw -lassimp
INCLUDE=-I include -I/sw/include -I/usr/local/include/bullet
FRAMEWORK=-framework OpenGL -framework IOKit -framework CoreVideo -framework BulletDynamics -framework LinearMath -framework BulletCollision
else
LIBS=-lGL -lGLEW -lglfw -lassimp -I /usr/local/include/bullet
endif


all:
	${CC} ${FLAGS} ${FRAMEWORK} ${SRC} ${LIBS} -o ${EXEC}
	./bin/prog
	make clear

clear:
	rm bin/prog
	rm gl.log
	rm log/*
