#ifndef RENDER_CC
#define RENDER_CC 1

#include <esat/draw.h>

#include "types.cc"
#include "cirno.cc"
#include "body.cc"
#include "mask.cc"

// FUNCIONES DE RENDERIZADO

const unsigned int kPixelScale = 4;

void DrawRectangle(Vector2_t position, Vector2_t size, unsigned char RGBA[4]){
    esat::DrawSetFillColor(RGBA[0], RGBA[1], RGBA[2]);

    float points[10] = {

        float(position.x) * kPixelScale, 
        float(position.y) * kPixelScale,

        float(position.x + size.x) * kPixelScale,
        float(position.y) * kPixelScale,

        float(position.x + size.x) * kPixelScale,
        float(position.y + size.y) * kPixelScale,

        float(position.x) * kPixelScale,
        float(position.y + size.y) * kPixelScale,

        float(position.x) * kPixelScale, 
        float(position.y) * kPixelScale,

    };

    esat::DrawSolidPath(points, 5);
}




#endif // RENDER_CC