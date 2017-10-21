CC=g++ -std=c++11
SRC=src/main.cpp src/components/camera/camera.cpp src/components/model/model.cpp src/components/physics/physics.cpp src/util/stb/stb_image.cpp src/util/shader/shader.cpp src/util/log/log.cpp src/util/window/window.cpp src/components/input/input.cpp src/components/parameters/parameters.cpp src/containers/world/world.cpp src/components/time/time.cpp src/components/sound/sound.cpp
EXEC=bin/prog
LIBS=-lGL -lGLEW -lglfw -lassimp -I /usr/include/bullet -lBulletDynamics -lBulletCollision -lLinearMath `pkg-config --libs freealut` -lm

all:
	${CC} ${FLAGS} ${SRC} ${LIBS} -o ${EXEC}
	#
	#
	#     TYPE "make run" TO PLAY
	#     TYPE "make makeandrun" TO COMPILE AND PLAY
	#     TYPE "make clear" TO CLEAR TEMP FILES
	#
	#
	#		PLAY WITH "I, J, K, L" TO MOVE
	#		AND PRESS "SPACE" TO JUMP
	#
	#
	#
	#
	#		EL JUEGO ESTA PROBADO Y FUNCIONANDO CON LA LIBRERIA BULLET
	#		Y CON 60 FPS
	#
	#
	#
	#
	#
	#


makeandrun:
	make && make run
run:
	./bin/prog

clear:
	rm log/gl.log bin/prog
