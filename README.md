# Polly-B-Gone

**Polly-B-Gone** is a 3D physics platform game that tells the story of a plucky wheeled robot named Polly, who has been imprisoned by the nefarious Dr. Nurbs in his laboratory. Polly must overcome a series of increasingly-elaborate obstacles to escape and regain her freedom. Polly was Mike Bostock's entry in the 2008 [CS 248](http://graphics.stanford.edu/courses/cs248-08/) video game competition, and it won the grand prize!

[You can find more information about this project on its Stanford official page.](http://cs.stanford.edu/people/mbostock/polly)

## Screenshots

<img src="http://cs.stanford.edu/people/mbostock/polly/intro.jpg" width="640" height="400" border="2">
<img src="http://cs.stanford.edu/people/mbostock/polly/a-breakthrough.jpg" width="640" height="400" border="2">
<img src="http://cs.stanford.edu/people/mbostock/polly/living-on-the-edge.jpg" width="640" height="400" border="2">
<img src="http://cs.stanford.edu/people/mbostock/polly/i-saw-this-on-tv.jpg" width="640" height="400" border="2">
<img src="http://cs.stanford.edu/people/mbostock/polly/airborne.jpg" width="640" height="400" border="2">
<img src="http://cs.stanford.edu/people/mbostock/polly/a-balancing-act.jpg" width="640" height="400" border="2">

## Controls

* You can move Polly, using the W,A,S,D keys. 
* SPACE pauses the game.
* F9 Toggles on/off world textures.
* F10 Render helper vectors.
* F11 toggles the Full Screen mode.

## Documentation

The entire game world for Polly-B-Gone is specified as an XML file. You can edit world.xml to create new levels, new puzzles, and even change the music, textures and lighting! See the [wiki](http://cs.stanford.edu/people/mbostock/polly) for details.

## Third-Party Libraries

Polly-B-Gone uses the [Simple DirectMedia Layer](http://www.libsdl.org/) Library version 1.2.13, which is distributed under the GNU Lesser General Public License version 2.1 or newer. Polly-B-Gone also uses [SDL_image](http://www.libsdl.org/projects/SDL_image/) 1.2.7 and [SDL_mixer](http://www.libsdl.org/projects/SDL_mixer/) 1.2.8 which are distributed under the same license. Polly-B-Gone uses [TinyXML](http://www.grinninglizard.com/tinyxml/) version 2.5.3, which is distributed under the ZLib license, and is copyright 2006 Lee Thomason.

## Third-Party Content

[Texturama](http://texturama.com/) provided the textures for the ceramic, concrete, and drain materials. These images are copyright XY3D, Texturama, and Eric Brian Smith and may not be redistributed for any other purpose without the permission of the copyright holders. The clover and ivy textures are from the [Blender for Architecture](http://blender-archi.tuxfamily.org/) website and are distributed via the Creative Commons Attribution License version 2.5. The MIDI files for the background music come from the "Very Best of GUS MIDI" collection, which is available from the SDL_mixer website (see above). According to the compilation author, "all of these MIDI files are freely distributable, but most of them are copyrighted."

## Building on GNU/Linux

The GNU/Linux Makefile was created by Raydelto Hernandez (raydelto [at] gmail.com). It was tested un Ubuntu 20.04 on June 2020.

If you want to build this code on GNU/Linux you first need to install the following libraries:
* OpenGL
* GLEW
* GLU
* GLUT
* TinyXML
* SDL
* SDL Mixer
* SDL Image

If you are using Ubuntu you can install these libraries by typing the following command:
```
sudo apt-get install build-essential libgl1-mesa-dev libglu1-mesa-dev libglew-dev freeglut3-dev libsdl1.2-dev libsdl-mixer1.2-dev libsdl-image1.2-dev  libtinyxml-dev

```

Then , finally , build the code by running the following command
```
make
```

Finally, you can run the freshly built game running:
```
./bin/main
```
