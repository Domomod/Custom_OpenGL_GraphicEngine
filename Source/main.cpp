#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>

#include "SampleApps/OpenGLinitializer.h"
#include "SampleApps/SinWaveApp/SinWaveApp.h"
#include "MyExceptions.h"

int main(int argc, char* argv[]) {
    //Application code
    SinWaveApp sinWaveApp;
    sinWaveApp.run();

    return 0;
}