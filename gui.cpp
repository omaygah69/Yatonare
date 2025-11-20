#define RAYGUI_IMPLEMENTATION
#include "terrain.h"
#include <math.h>
#include "raygui.h"

bool GuiSliderFloat(Rectangle bounds, const char *label, float *value, float min, float max)
{
    float old = *value;
    GuiSliderBar(bounds, label, TextFormat("%.2f", *value), value, min, max);
    return (*value != old);
}

bool GuiSliderInt(Rectangle bounds, const char *label, int *value, int min, int max)
{
    int old = *value;
    // GuiSliderBar expects a float pointer; cast via temporary float then write back
    float v = (float)*value;
    GuiSliderBar(bounds, label, TextFormat("%d", *value), &v, (float)min, (float)max);
    *value = (int)roundf(v);
    return (*value != old);
}
