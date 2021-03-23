#pragma once

#include "../resource.h"

#include "iStd.h"
#include "iWindow.h"

void goFullscreen();
void enforceResolution(int edge, RECT& rt, int win_border_width, int win_border_height);

extern iPoint cursor;
void loadCursor();
void freeCursor();
void drawCursor(float dt);
bool updateCursor(bool inClient);

