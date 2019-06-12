//
// Created by dominik on 07.04.19.
//

#include "FreeCamera.h"
#include "Platform/OpenGL/Window/KeyStateGetter.h"

void FreeCamera::keyboardReaction(KeyStateGetter *keyStateGetter)
{
    if (keyStateGetter->getKey(GLFW_KEY_W))
    {
        position += movementSpeed * forward;
    }
    if (keyStateGetter->getKey(GLFW_KEY_S))
    {
        position -= movementSpeed * forward;
    }
    if (keyStateGetter->getKey(GLFW_KEY_A))
    {
        position -= movementSpeed * cross(forward, up);
    }
    if (keyStateGetter->getKey(GLFW_KEY_D))
    {
        position += movementSpeed * cross(forward, up);
    }
}

void FreeCamera::mouseMovementReaction(MouseMovementInfo &mouseMovementInfo)
{
    /* Mouse coordinates seem inverted in comparison to OpenGL coordinates.
     * This x and y should not be associated with wolrd x and y axis.
     * */
    auto x = -static_cast<float>(mouseMovementInfo.xMovement);
    auto y = -static_cast<float>(mouseMovementInfo.yMovement);

    /* Not ignoring 0 length mouse movement would result in dividing by 0. On the other hand losing few camera
     * rotations is not a catastrophe, so I decided not to throw an exception but send an error message instead.
     * */
    if (x == 0 && y == 0)
    {
        std::cerr << "Ignored notification about a mouse movement with 0 length.\n";
        return;
    }

    /* By calculating ratio of mouse x and y movement we can calculate the axis around which we will rotate
     * this allows as to rotate around camera's up (looking around) axis and cameras left axis (looking up
     * and down) using one quaternion;
     * */
    float xScale = x / (abs(x) + abs(y));
    float yScale = y / (abs(x) + abs(y));

    float movementLength = std::sqrt(x * x + y * y);

    glm::vec3 left = cross(forward, up);

    glm::vec3 rotationAxis = yScale * left + xScale * up;

    glm::vec3 normalizedRotationAxis = glm::normalize(rotationAxis);

    glm::fquat quaternion = glm::angleAxis<float>(glm::radians(0.05f * movementLength), normalizedRotationAxis);

    glm::mat4 rotationMatrix = glm::toMat4(quaternion);

    /* Matrix rotation requires a homogenous positions while the rest of the system is easier to calculate with
     * a 3 values system.
     * */
    glm::vec4 vec4Forward(
            forward.x,
            forward.y,
            forward.z,
            0);

    glm::vec4 vec4Up(
            up.x,
            up.y,
            up.z,
            0);

    vec4Forward = rotationMatrix * vec4Forward;
    vec4Up = rotationMatrix * vec4Up;

    forward = glm::vec3(vec4Forward);
    up = glm::vec3(vec4Up);
}

glm::mat4 FreeCamera::calculateViewMatrix()
{
    return glm::lookAt(
            position,
            position + forward,
            up);
}

OnChangeListener<KeyStateGetter *> &FreeCamera::getKeyboardStateListener()
{
    return keyboardStateListener;
}

OnChangeListener<MouseMovementInfo> &FreeCamera::getMouseMovementListener()
{
    return mouseMovementListener;
}

FreeCamera::FreeCamera(const glm::vec3 &position, const glm::vec3 &forward, const glm::vec3 &up, float speed)
        : movementSpeed(speed), position(position),
          forward(forward), up(up)
{
    keyboardStateListener.setReactionFuncPtr(
            [&](KeyStateGetter *keyStateGetter)
            { this->keyboardReaction(keyStateGetter); }
    );

    mouseMovementListener.setReactionFuncPtr(
            [&](MouseMovementInfo mouseMovementInfo)
            { this->mouseMovementReaction(mouseMovementInfo); }
    );
}

const glm::vec3 &FreeCamera::getPosition() const
{
    return position;
}
