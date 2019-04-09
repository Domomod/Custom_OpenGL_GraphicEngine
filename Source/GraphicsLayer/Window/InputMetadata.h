//
// Created by dominik on 07.04.19.
//

#ifndef GAMEENGINE_INPUTMETADATA_H
#define GAMEENGINE_INPUTMETADATA_H

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

#endif //GAMEENGINE_INPUTMETADATA_H
