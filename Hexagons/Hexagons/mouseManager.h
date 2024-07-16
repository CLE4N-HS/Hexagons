#pragma once
#include "windowManager.h"

void initMouse();
void updateMouse(Window* _window);
sfBool leftClick();
sfBool leftClickUnreleased();
sfVector2f getMousePos();
