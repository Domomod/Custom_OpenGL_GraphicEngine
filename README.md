# GameEngine

This project is not a graphical tool, it's a set of classes allowing fast writing of interactive visualisations.

Engine encapsulates OpenGL in an object oriented interface, to keep RAII and initialize OpenGL structures properly, OpenGL encapsulated classes are stored using smart pointers. This way member classes can be restrained from initialization before OpenGL context state setup.

It uses glm with some of it's experimental function for needed math calculations.

Requrements:
OpenGl 4.2 support (core profile),
gl3w
GLFW
ASSIMP
SOIL
BOOST
