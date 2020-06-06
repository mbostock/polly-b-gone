LIBS = -lGL -lGLEW -lGLU -lglut -ltinyxml -lSDL -lSDL_mixer -lSDL_image
INCLUDES=-I /usr/include

SRC = ./src/portal.cpp \
./src/fan.cpp \
./src/physics/constraint.cpp \
./src/physics/shape.cpp \
./src/physics/vector.cpp \
./src/physics/transform.cpp \
./src/physics/force.cpp \
./src/physics/particle.cpp \
./src/physics/translation.cpp \
./src/physics/rotation.cpp \
./src/switch.cpp \
./src/main.cpp \
./src/room_force.cpp \
./src/model.cpp \
./src/resource.cpp \
./src/wall.cpp \
./src/world.cpp \
./src/lighting.cpp \
./src/ball.cpp \
./src/worlds.cpp \
./src/translating.cpp \
./src/material.cpp \
./src/transforming.cpp \
./src/sound.cpp \
./src/trail.cpp \
./src/rotating.cpp \
./src/player.cpp \
./src/simulation.cpp \
./src/tube.cpp \
./src/shader.cpp \
./src/ramp.cpp \
./src/seesaw.cpp \
./src/escalator.cpp \
./src/room.cpp \
./src/block.cpp \
./src/texture.cpp \
./src/room_object.cpp

OBJ = ./bin/portal.o \
./bin/fan.o \
./bin/constraint.o \
./bin/shape.o \
./bin/vector.o \
./bin/transform.o \
./bin/force.o \
./bin/particle.o \
./bin/translation.o \
./bin/rotation.o \
./bin/switch.o \
./bin/main.o \
./bin/room_force.o \
./bin/model.o \
./bin/resource.o \
./bin/wall.o \
./bin/world.o \
./bin/lighting.o \
./bin/ball.o \
./bin/worlds.o \
./bin/translating.o \
./bin/material.o \
./bin/transforming.o \
./bin/sound.o \
./bin/trail.o \
./bin/rotating.o \
./bin/player.o \
./bin/simulation.o \
./bin/tube.o \
./bin/shader.o \
./bin/ramp.o \
./bin/seesaw.o \
./bin/escalator.o \
./bin/room.o \
./bin/block.o \
./bin/texture.o \
./bin/room_object.o

WARNINGS=-w

FLAGS=-std=c++11

all:
	g++ -c -g $(SRC) $(INCLUDES) $(WARNINGS) $(FLAGS)
	ls bin>/dev/null||mkdir bin
	mv *.o ./bin
	g++ -g $(OBJ) $(LIBS) $(INCLUDES) -o bin/main $(WARNINGS) $(FLAGS)
