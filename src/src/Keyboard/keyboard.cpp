/*****************************/
/*  FILE NAME: keyboard.cpp  */
/*****************************/
#include "keyboard.h"

// constructor
Keyboard::Keyboard()
{
    for (int i = 0; i < KEY_LAST; ++i)
        key[i] = false;
}

// copy constructor 
Keyboard::Keyboard(const Keyboard& other)
{
    for (int i = 0; i < KEY_LAST; ++i)
        key[i] = other.key[i];
}

// this function changes the key corresponding to the key code to the released state.
void Keyboard::keyUp(KeyCode code)
{
    key[code] = false;
}

// this function changes the key corresponding to the key code to the pressed state.
void Keyboard::keyDown(KeyCode code)
{
    key[code] = true;
}

// this function returns the pressed state of the key corresponding to the key code
bool Keyboard::isKeyDown(KeyCode code) const
{
    return key[code];
}