# Polly-B-Gone

**Polly-B-Gone** is a 3D physics platform game that tells the story of a plucky wheeled robot named Polly, who has been imprisoned by the nefarious Dr. Nurbs in his laboratory. Polly must overcome a series of increasingly-elaborate obstacles to escape and regain her freedom. Polly was my entry in the 2008 [CS 248](http://graphics.stanford.edu/courses/cs248-08/) video game competition, and she won the grand prize!

## Download

Polly-B-Gone is currently available as a binary only for Mac OS X:

* [polly-1.0.dmg.zip](http://cs.stanford.edu/people/mbostock/polly/polly-1.0.dmg.zip)

It has only been tested on 10.5 (Leopard), but it probably works on other versions. For other platforms, you’ll need to build from source and make modifications as necessary.

## Screenshots

<img src="http://cs.stanford.edu/people/mbostock/polly/intro.jpg" width="640" height="400" border="2">
<img src="http://cs.stanford.edu/people/mbostock/polly/a-breakthrough.jpg" width="640" height="400" border="2">
<img src="http://cs.stanford.edu/people/mbostock/polly/living-on-the-edge.jpg" width="640" height="400" border="2">
<img src="http://cs.stanford.edu/people/mbostock/polly/i-saw-this-on-tv.jpg" width="640" height="400" border="2">
<img src="http://cs.stanford.edu/people/mbostock/polly/airborne.jpg" width="640" height="400" border="2">
<img src="http://cs.stanford.edu/people/mbostock/polly/a-balancing-act.jpg" width="640" height="400" border="2">

## Documentation

The entire game world for Polly-B-Gone is specified as an XML file. You can edit world.xml to create new levels, new puzzles, and even change the music, textures and lighting! See the [wiki](/mbostock/polly-b-gone/wiki) for details.

## Third-Party Libraries

Polly-B-Gone uses the [Simple DirectMedia Layer](http://www.libsdl.org/) Library version 1.2.13, which is distributed under the GNU Lesser General Public License version 2.1 or newer. Polly-B-Gone also uses [SDL_image](http://www.libsdl.org/projects/SDL_image/) 1.2.7 and [SDL_mixer](http://www.libsdl.org/projects/SDL_mixer/) 1.2.8 which are distributed under the same license. Polly-B-Gone uses [TinyXML](http://www.grinninglizard.com/tinyxml/) version 2.5.3, which is distributed under the ZLib license, and is copyright 2006 Lee Thomason.

## Third-Party Content

[Texturama](http://texturama.com/) provided the textures for the ceramic, concrete, and drain materials. These images are copyright XY3D, Texturama, and Eric Brian Smith and may not be redistributed for any other purpose without the permission of the copyright holders. The clover and ivy textures are from the [Blender for Architecture](http://blender-archi.tuxfamily.org/) website and are distributed via the Creative Commons Attribution License version 2.5. The MIDI files for the background music come from the "Very Best of GUS MIDI" collection, which is available from the SDL_mixer website (see above). According to the compilation author, "all of these MIDI files are freely distributable, but most of them are copyrighted."
