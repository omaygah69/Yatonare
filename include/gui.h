#pragma once
#include "terrain.h"
#include "raylib.h"

bool GuiSliderFloat(Rectangle bounds, const char *label, float *value, float min, float max);
bool GuiSliderInt(Rectangle bounds, const char *label, int *value, int min, int max);
