# GameEngine

To see a preview visit my github page: https://domomod.github.io/GameEngine/

Engine encapsulates OpenGL in an object oriented interface, to keep RAII and initialize OpenGL structures properly, OpenGL encapsulated classes are stored using smart pointers. This way member classes can be restrained from initialization before OpenGL context state setup.

Engine implements:
- a sky box
- PBR material system
- IBL lightning
- HDR display
- free camera
- skeletal animation

It uses glm with some of it's experimental function for needed math calculations.

Requrements:
OpenGl 4.2 support (core profile),
gl3w
GLFW
ASSIMP
SOIL
BOOST
