//
// Created by dominik on 07.04.19.
//

#include "InputSystem.h"

#include "Window.h"
#include "InputMetadata.h"

void WindowInputSystem::connectToWindow(Window &window)
{
    auto windowHandle = window.getWindowHandle();
    glfwSetWindowUserPointer(windowHandle, this);
    keyStateGetter.setWindow(windowHandle);

    glfwSetKeyCallback(windowHandle, onKeyAction);
    glfwSetCursorPosCallback(windowHandle, onMouseMovementAction);
    glfwSetMouseButtonCallback(windowHandle, onMousePressedAction);
    glfwSetWindowFocusCallback(windowHandle, onWindowFocusRecheckKeyStates);
}


void WindowInputSystem::connectToKeyPressedListener(OnChangeListener<KeyInfo> &onChangeListener)
{
    OnKeyPressedNotifier.addListener(&onChangeListener);
}


void WindowInputSystem::connectToKeyboardStateListener(OnChangeListener<KeyStateGetter *> &onChangeListener)
{
    KeyboardStateNotifier.addListener(&onChangeListener);
}


void WindowInputSystem::connectToMouseMovedListener(OnChangeListener<MouseMovementInfo> &onChangeListener)
{
    OnMoseMovedNotifier.addListener(&onChangeListener);
}


void WindowInputSystem::connectToMouseClickedListener(OnChangeListener<MouseClickInfo> &onChangeListener)
{
    OnMouseClickedNotifier.addListener(&onChangeListener);
}


void WindowInputSystem::keyboardStateNotify()
{
    /* Notifying the client about current keyboard state is not caused by arrival of an input event, but
     * it's performed manually. This way we can accumulate multiple eventes (pressed keys) into one notify.
     * The client does not ask the Input System about the state because I wanted it to only know that
     * it owns a Listener, and the System to only know about having a Notifier.
     * */
    KeyboardStateNotifier.notifyListeners(&keyStateGetter);
}


void WindowInputSystem::onKeyAction(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    // In contrast to mouse handling, the key action does not only notify the listeners, it keeps track which keys
    // are pressed atm. This is caused by the manner in which keyboard may  be used. If user needs to know which
    // keys are pressed at the same time he will have to ask the Input System. If user is only interested in the
    // fact the button was pressed he needs to sing on the listeners list.
    auto *windowsInputSystem = reinterpret_cast<WindowInputSystem *>(glfwGetWindowUserPointer(window));

    /* System works in two modes, with hidden cursor (fe. for camera movements)
     * and with free cursor (fe. for clicking in menu)
     * */
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        setMouseStateUnfocused(window, windowsInputSystem);
    }
    if (key == GLFW_KEY_ESCAPE && action == GLFW_REPEAT)
    {
        setMouseStateFocused(window, windowsInputSystem);
    }

    windowsInputSystem->OnKeyPressedNotifier.notifyListeners(KeyInfo(key, scancode, action, mods));
}


void WindowInputSystem::onMouseMovementAction(GLFWwindow *window, double xPos, double yPos)
{
    auto *windowsInputSystem = reinterpret_cast<WindowInputSystem *>(glfwGetWindowUserPointer(window));

    /* Mose movement notification is considered needful only if the mouse is focused on window
     * */
    if (windowsInputSystem->mouseFocused)
    {
        double xMovement = xPos - windowsInputSystem->xPreviousMosePosition;
        double yMovement = yPos - windowsInputSystem->yPreviousMosePosition;

        windowsInputSystem->xPreviousMosePosition = xPos;
        windowsInputSystem->yPreviousMosePosition = yPos;

        windowsInputSystem->OnMoseMovedNotifier.notifyListeners(MouseMovementInfo(xPos, yPos, xMovement, yMovement));
    }
}


void WindowInputSystem::onMousePressedAction(GLFWwindow *window, int button, int action, int mods)
{
    auto *windowsInputSystem = reinterpret_cast<WindowInputSystem *>(glfwGetWindowUserPointer(window));
    double xPosition;
    double yPosition;
    glfwGetCursorPos(window, &xPosition, &yPosition);

    windowsInputSystem->OnMouseClickedNotifier.notifyListeners(
            MouseClickInfo(button, action, mods, xPosition, yPosition));
}


void WindowInputSystem::onWindowFocusRecheckKeyStates(GLFWwindow *window, int focused)
{
    auto *windowsInputSystem = reinterpret_cast<WindowInputSystem *>(glfwGetWindowUserPointer(window));

    setMouseStateFocused(window, windowsInputSystem);
}


void WindowInputSystem::setMouseStateFocused(GLFWwindow *window, WindowInputSystem *windowsInputSystem)
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    windowsInputSystem->mouseFocused = true;
    glfwGetCursorPos(window, &windowsInputSystem->xPreviousMosePosition, &windowsInputSystem->yPreviousMosePosition);
}


void WindowInputSystem::setMouseStateUnfocused(GLFWwindow *window, WindowInputSystem *windowsInputSystem)
{
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    windowsInputSystem->mouseFocused = false;
}

void WindowInputSystem::pollEvents()
{
    glfwPollEvents();
    keyboardStateNotify();
}
