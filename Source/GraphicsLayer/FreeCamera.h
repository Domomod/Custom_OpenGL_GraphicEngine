#include <cmath>

//
// Created by dominik on 07.04.19.
//

#define GLM_ENABLE_EXPERIMENTAL

#ifndef GAMEENGINE_FREECAMERA_H
#define GAMEENGINE_FREECAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <GLFW/glfw3.h>

#include "Platform/OpenGL/Window/InputMetadata.h"
#include "OnChangeListener.h"

#include <iostream>

class KeyStateGetter;

/* This class implements a free ("zero gravity") camera, with no up and down, you can
 * look around move and rotate your view in any direction you want. For this you have to link it
 * to a system that will notify the class about keyboard / mouse events, because camera does not directly
 * look for those events. You can do it by assigning this class listeners to notifier of a class responsible
 * for user input.
 * */
class FreeCamera
{
public:
    glm::mat4 calculateViewMatrix();

    OnChangeListener<KeyStateGetter *> &getKeyboardStateListener();

    OnChangeListener<MouseMovementInfo> &getMouseMovementListener();

    explicit FreeCamera(const glm::vec3 &position = glm::vec3(0, 0, 0), const glm::vec3 &forward = glm::vec3(0, 0, -1),
                        const glm::vec3 &up = glm::vec3(0, 1, 0), float speed = 0.2);

    const glm::vec3 &getPosition() const;

private:
    void keyboardReaction(KeyStateGetter *keyStateGetter);

    void mouseMovementReaction(MouseMovementInfo &mouseMovementInfo);

    float movementSpeed = 0.2;

    glm::vec3 position;
    glm::vec3 forward;
    glm::vec3 up;

    OnChangeListener<KeyStateGetter *> keyboardStateListener;
    OnChangeListener<MouseMovementInfo> mouseMovementListener;
};


#endif //GAMEENGINE_FREECAMERA_H
