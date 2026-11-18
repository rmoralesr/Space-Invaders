#ifndef DRAW_HH
#define DRAW_HH 1

#include <esat/draw.h>

#include "../tools/asset-loader.hh"
#include "../entities/body.hh"
#include "cirno.hh"

const unsigned int kPixelScale = 3;

void Draw(Body_t* self);
void DrawRectangle(
    Vector2_t position, 
    Vector2_t size, 
    unsigned char RGBA[4]
);

#endif // DRAW_HH