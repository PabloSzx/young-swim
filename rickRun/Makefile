CC=g++ -std=c++11
SRC=src/main.cpp src/components/camera/camera.cpp src/components/model/model.cpp src/components/physics/physics.cpp src/util/shader/shader.cpp src/util/log/log.cpp src/util/window/window.cpp src/components/input/input.cpp src/components/parameters/parameters.cpp src/containers/world/world.cpp
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
LIBS=-lGL -lGLEW -lglfw -lassimp -I /usr/include/bullet -lBulletDynamics -lBulletCollision -lLinearMath
endif



all:
	${CC} ${FLAGS} ${FRAMEWORK} ${SRC} ${LIBS} -o ${EXEC}
	#
	#
	#     TYPE "make run" TO PLAY
	#     TYPE "make makeandrun" TO COMPILE AND PLAY
	#     TYPE "make clear" TO CLEAR TEMP FILES
	#
	#
makeandrun:
	make && make run
run:
	./bin/prog

clear:
	rm log/gl.log bin/prog
