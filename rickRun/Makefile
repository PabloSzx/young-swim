CC=g++ -std=c++11 -O2
SRC=src/main.cpp src/components/glerror/glerror.cpp src/components/menu/menu.cpp src/components/camera/camera.cpp src/components/model/model.cpp src/components/debugDrawer/DebugDrawer.cpp src/components/physics/physics.cpp src/util/stb/stb_image.cpp src/util/shader/shader.cpp src/util/log/log.cpp src/util/window/window.cpp src/components/input/input.cpp src/components/parameters/parameters.cpp src/containers/world/world.cpp src/components/time/time.cpp src/components/sound/sound.cpp
EXEC=bin/prog
LIBS=-fopenmp -lGL -lGLEW -lglfw -lassimp -I /usr/include/bullet -lBulletDynamics -lBulletCollision -lLinearMath `pkg-config --libs freealut` -lm

all:
	${CC} ${FLAGS} ${SRC} ${LIBS} -o ${EXEC}
	#
	#	  TYPE "make" TO COMPILE
	#     TYPE "make run" TO PLAY
	#     TYPE "make make" TO COMPILE AND PLAY
	#     TYPE "make clear" TO CLEAR TEMP FILES
	#
	#
	#		PLAY WITH "I, J, K, L" TO MOVE
	#		AND PRESS "SPACE" TO JUMP
	#		USE ARROWS AND "ENTER" TO USE MENU
	#
	#
	#
	#


make:
	make && make run
run:
	./bin/prog

clear:
	rm log/gl.log bin/prog
