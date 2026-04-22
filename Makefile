# Homebrew prefix
HOMEBREW_PREFIX := $(shell brew --prefix 2>/dev/null || echo /opt/homebrew)

# Compiler flags
CXXFLAGS = \
	-O2 \
	-std=c++11 \
	-Wno-deprecated-declarations \
	-I$(HOMEBREW_PREFIX)/include \
	-I$(HOMEBREW_PREFIX)/include/SDL2 \
	-DTIXML_USE_STL

# Linker flags
LDFLAGS = \
	-framework Cocoa \
	-framework CoreFoundation \
	-framework OpenGL \
	-framework GLUT \
	-L$(HOMEBREW_PREFIX)/lib \
	-lSDL2 \
	-lSDL2_image \
	-lSDL2_mixer

RESOURCES = \
	resources/Polly.icns \
	resources/*.frag \
	resources/*.jpg \
	resources/*.ogg \
	resources/*.png \
	resources/*.vert \
	resources/world.xml

all : obj/Polly-B-Gone.app

obj/main.out : \
	obj/ball.o \
	obj/block.o \
	obj/escalator.o \
	obj/fan.o \
	obj/lighting.o \
	obj/material.o \
	obj/model.o \
	obj/physics/constraint.o \
	obj/physics/force.o \
	obj/physics/particle.o \
	obj/physics/rotation.o \
	obj/physics/shape.o \
	obj/physics/transform.o \
	obj/physics/translation.o \
	obj/physics/vector.o \
	obj/player.o \
	obj/portal.o \
	obj/ramp.o \
	obj/resource.o \
	obj/room.o \
	obj/room_force.o \
	obj/room_object.o \
	obj/rotating.o \
	obj/seesaw.o \
	obj/shader.o \
	obj/simulation.o \
	obj/sound.o \
	obj/switch.o \
	obj/texture.o \
	obj/trail.o \
	obj/transforming.o \
	obj/translating.o \
	obj/tube.o \
	obj/wall.o \
	obj/world.o \
	obj/worlds.o \
	obj/tinyxml/tinyxml.o \
	obj/tinyxml/tinyxmlerror.o \
	obj/tinyxml/tinyxmlparser.o \
	obj/tinyxml/tinystr.o \
	obj/main.o

obj/physics/particle_test.out : \
	obj/physics/force.o \
	obj/physics/particle.o \
	obj/physics/vector.o \
	obj/simulation.o

obj/physics/shape_test.out : \
	obj/physics/shape.o \
	obj/physics/vector.o

obj/physics/vector_test.out : \
	obj/physics/vector.o

obj/Polly-B-Gone.app : obj/main.out $(RESOURCES) resources/Info.plist Makefile
	rm -rf $@
	mkdir -p $@/Contents/MacOS
	cp $< $@/Contents/MacOS/Polly-B-Gone
	mkdir -p $@/Contents/Resources
	cp resources/Info.plist $@/Contents
	cp $(RESOURCES) $@/Contents/Resources

obj/%.out : obj/%.o
	$(CXX) $(LDFLAGS) -o $@ $^

obj/%.o : src/%.cpp
	mkdir -p $(@D)
	$(CXX) -c $(CXXFLAGS) -o $@ $<

.PRECIOUS : obj/%.o obj/physics/%.o obj/tinyxml/%.o

clean:
	rm -rf obj

run: obj/Polly-B-Gone.app
	./obj/Polly-B-Gone.app/Contents/MacOS/Polly-B-Gone

.PHONY: all clean run
