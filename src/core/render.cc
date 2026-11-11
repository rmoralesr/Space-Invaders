#ifndef RENDER_CC
#define RENDER_CC 1

#include <esat/draw.h>

// #include "types.cc"
#include "cirno.cc"

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

void GetPositionColor(unsigned char RGBA[4], unsigned int x, unsigned int y) {
    
    // TODO, cuando tengamos las color mask

    RGBA[0] = 255;
    RGBA[1] = 255;
    RGBA[2] = 255;
    RGBA[3] = 255;
}

void SetToBlack(unsigned char RGBA[4]) {
    RGBA[0] = 0;
    RGBA[1] = 0;
    RGBA[2] = 0;
    RGBA[3] = 0;
}


#endif // RENDER_CC