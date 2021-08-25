#include "surface.h"
#include "render.h"
#include "image.h"

#define W 700
#define H 100

static void sample(surface_t *base, float fps)
{
    surface_clear(base, (color_t){255, 255, 255, 255}, 0, 0, base->width, base->height);
	draw_text(base, 0, 0, "Hello World!", 6, RGB(0x000000));
}

#include "main.h"