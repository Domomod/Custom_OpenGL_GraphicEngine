//
// Created by dominik on 07.04.19.
//

#ifndef GAMEENGINE_INPUTSYSTEM_H
#define GAMEENGINE_INPUTSYSTEM_H

#include "Window.h"
#include "OnChangeNotifier.h"

struct KeyInfo{
    int key;
    int scancode;
    int action;
    int mods;

    KeyInfo(int key, int scancode, int action, int mods) : key(key), scancode(scancode), action(action), mods(mods) {}
};

struct MouseMovementInfo{
    double xPosition;
    double yPosition;
    double xMovement;
    double yMovement;

    MouseMovementInfo(double xPosition, double yPosition, double xMovement, double yMovement) : xPosition(xPosition),
                                                                                                yPosition(yPosition),
                                                                                                xMovement(xMovement),
                                                                                                yMovement(yMovement) {}
};

struct MouseClickInfo{
    int button;
    int acction;
    int mods;
    double xPosition;
    double yPosition;

    MouseClickInfo(int button, int acction, int mods, double xPosition, double yPosition) : button(button),
                                                                                              acction(acction),
                                                                                              mods(mods),
                                                                                              xPosition(xPosition),
                                                                                              yPosition(yPosition) {}
};

class WindowInputSystem {
public:
    static void onKeyAction(GLFWwindow* window, int key, int scancode, int action, int mods){
        auto * windowsInputSystem = reinterpret_cast<WindowInputSystem*>(glfwGetWindowUserPointer(window));
        windowsInputSystem->OnKeyPressedNotifier.notifyListeners( KeyInfo(key, scancode, action, mods) );
    }

    static void onMouseMovementAction(GLFWwindow* window, double xPos, double yPos){
        auto * windowsInputSystem = reinterpret_cast<WindowInputSystem*>(glfwGetWindowUserPointer(window));
        double xMovement = 0;
        double yMovement = 0;
        if(windowsInputSystem->xPreviousMosePosition != -1 && windowsInputSystem->yPreviousMosePosition != -1){
            xMovement = xPos - windowsInputSystem->xPreviousMosePosition;
            yMovement = yPos - windowsInputSystem->yPreviousMosePosition;
        }
        windowsInputSystem->xPreviousMosePosition = xPos;
        windowsInputSystem->yPreviousMosePosition = yPos;

        windowsInputSystem->OnMoseMovedNotifier.notifyListeners( MouseMovementInfo(xPos, yPos, xMovement, yMovement) );
    }

    static void onMousePressedAction(GLFWwindow* window, int button, int action, int mods){
        auto * windowsInputSystem = reinterpret_cast<WindowInputSystem*>(glfwGetWindowUserPointer(window));
        double xPosition;
        double yPosition;
        glfwGetCursorPos(window, &xPosition, &yPosition);

        windowsInputSystem->OnMouseClickedNotifier.notifyListeners( MouseClickInfo(button, action, mods, xPosition, yPosition) );
    }


    void connectToWindow(Window& window){
        auto windowHandle = window.getWindowHandle();
        glfwSetWindowUserPointer(windowHandle, this);

        glfwSetKeyCallback        ( windowHandle, onKeyAction           );
        glfwSetCursorPosCallback  ( windowHandle, onMouseMovementAction );
        glfwSetMouseButtonCallback( windowHandle, onMousePressedAction  );
    }

private:
    OnChangeNotifier<KeyInfo>           OnKeyPressedNotifier;
    OnChangeNotifier<MouseMovementInfo> OnMoseMovedNotifier;
    OnChangeNotifier<MouseClickInfo> OnMouseClickedNotifier;

    double xPreviousMosePosition = -1;
    double yPreviousMosePosition = -1;
};


#endif //GAMEENGINE_INPUTSYSTEM_H
