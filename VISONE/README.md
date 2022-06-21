# VISONE

Is a Virtual Space Observation Engine written in OpenGL in C++

## File list

* observatory.hpp - Class for management an observatory
* sun.hpp              - Class for management the sun
* satellite.hpp       - Class for management a satellite
* earth.hpp           - Class for management the earth
* renderable.hpp  - Class inherited by all the objects that define the rendering objects
* renderer.hpp      - Class that performe the actual rendering
* visone.hpp         - Header file to include all the library
* main.cpp           - Example file

## Dependencies

* ASTRO   - https://github.com/CobbsLab/ASTRO
* MPL       - https://github.com/thewoz/MPL
* SOIL2 - https://github.com/SpartanJ/soil2
* Assimp - https://github.com/assimp/assimp
* GFLW - https://github.com/glfw/glfw
* GLAD - https://glad.dav1d.de
* OpenCV - https://github.com/opencv/opencv
* OpenGL

## Todo

* Make the class satellite thread safe

## FIXME

* Controllare tutta la parte di quando usi un satellite con i pannelli solari
