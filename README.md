# GaMaTeT 3D

GaMaTeT 3D is a clone of the legendary BlockOut (1989).

It is a small project I originally created in just a few days as a homework assignment for a Computer Graphics course during my university studies. It was written back in 2007 and then left forgotten on a backup hard drive for nearly 19 years. At the time, it was developed for Windows only, since that was the only operating system I was familiar with. Textures were stored as BMP files, as loading formats such as JPG or PNG was not as straightforward back then.

## Modernization

The project has now been refreshed to the C++11 standard and can be built on Linux, macOS, and Windows using CMake. The executable requires the texture directory to be present in the working directory.

Aside from modernization and cross-platform support, the game behaves exactly as it did in 2007.

## Technical details

The game uses OpenGL 1.x (immediate mode via glBegin/glEnd). Unfortunately, support for this legacy API is gradually disappearing. On macOS, especially on newer Apple Silicon machines, the game may run somewhat sluggishly. On Linux and Windows, however, performance is generally smooth.

## License

Released under the GNU GPL v3 (or later) license.

## Enjoy!
