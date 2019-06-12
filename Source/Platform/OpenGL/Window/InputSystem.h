//
// Created by dominik on 07.04.19.
//

#ifndef GAMEENGINE_INPUTSYSTEM_H
#define GAMEENGINE_INPUTSYSTEM_H


#include <GLFW/glfw3.h>

#include "KeyStateGetter.h"
#include "OnChangeNotifier.h"

class KeyInfo;

class MouseMovementInfo;

class MouseClickInfo;

class Window;


/* WindowInputSystem works as an input interface for GLFW window, one Input System may connect to
 * multiple GLFW windows. GLFW is a c API so only c style function pointers can be assigned to handle
 * input actions, WindowInputSystem object is accesed via a void* pointer stored in GLFWwindow (so called
 * window user).
 * System reacts to the input (fe. to change mouse state), and also notifies all interested listeners about
 * an event via a notifier-listener interface.
 * */

class WindowInputSystem
{
public:
    void connectToWindow(Window &window);

    void connectToKeyPressedListener(OnChangeListener<KeyInfo> &onChangeListener);

    void connectToKeyboardStateListener(OnChangeListener<KeyStateGetter *> &onChangeListener);

    void connectToMouseMovedListener(OnChangeListener<MouseMovementInfo> &onChangeListener);

    void connectToMouseClickedListener(OnChangeListener<MouseClickInfo> &onChangeListener);

    void pollEvents();

private:
    static void onKeyAction(GLFWwindow *window, int key, int scancode, int action, int mods);

    static void onMouseMovementAction(GLFWwindow *window, double xPos, double yPos);

    static void onMousePressedAction(GLFWwindow *window, int button, int action, int mods);

    static void onWindowFocusRecheckKeyStates(GLFWwindow *window, int focused);

    static void setMouseStateFocused(GLFWwindow *window, WindowInputSystem *windowsInputSystem);

    static void setMouseStateUnfocused(GLFWwindow *window, WindowInputSystem *windowsInputSystem);

    void keyboardStateNotify();

    OnChangeNotifier<KeyInfo> OnKeyPressedNotifier;
    OnChangeNotifier<KeyStateGetter *> KeyboardStateNotifier;
    OnChangeNotifier<MouseMovementInfo> OnMoseMovedNotifier;
    OnChangeNotifier<MouseClickInfo> OnMouseClickedNotifier;

    KeyStateGetter keyStateGetter;

    bool mouseFocused = false;

    double xPreviousMosePosition = 0;
    double yPreviousMosePosition = 0;
};


#endif //GAMEENGINE_INPUTSYSTEM_H
