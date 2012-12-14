CXXFLAGS = \
	-O2 \
	-I/System/Library/Frameworks/GLUT.framework/Headers \
	-I/System/Library/Frameworks/OpenGL.framework/Headers \
	-I/System/Library/Frameworks/SDL.framework/Headers \
	-I/System/Library/Frameworks/SDL_image.framework/Headers \
	-I/System/Library/Frameworks/SDL_mixer.framework/Headers \
	-I/System/Library/Frameworks/TinyXML.framework/Headers

LDFLAGS = \
	-framework Cocoa \
	-framework GLUT \
	-framework OpenGL \
	-framework SDL \
	-framework SDL_image \
	-framework SDL_mixer \
	-framework TinyXML

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
	src/SDLMain.m

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
	mkdir -p $@/Contents/Frameworks
	cp -R /System/Library/Frameworks/SDL.framework $@/Contents/Frameworks
	cp -R /System/Library/Frameworks/SDL_image.framework $@/Contents/Frameworks
	cp -R /System/Library/Frameworks/SDL_mixer.framework $@/Contents/Frameworks
	cp -R /System/Library/Frameworks/TinyXML.framework $@/Contents/Frameworks
	find $@/Contents/Frameworks -name Headers | xargs rm -r
#	ln -sf ../../../../resources/world.xml $@/Contents/Resources/world.xml

obj/%.out : obj/%.o
	$(CXX) $(LDFLAGS) -o $@ $^

obj/%.o : src/%.cpp
	mkdir -p $(@D)
	$(CXX) -c $(CXXFLAGS) -o $@ $<

.PRECIOUS : obj/%.o obj/physics/%.o

clean:
	rm -rf obj
