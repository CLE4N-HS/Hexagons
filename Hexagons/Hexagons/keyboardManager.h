#pragma once
#include "windowManager.h"

void initKeyboard();
void updateKeyboard(Window* _window);
sfBool hasPressed(sfKeyCode _key);
sfBool isPressing(sfKeyCode _key);
