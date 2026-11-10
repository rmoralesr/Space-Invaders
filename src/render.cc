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

/* En desuso
void DrawPixel(Vector2_t position, int pixel_size){
    esat::DrawSetFillColor(255,255,255);
    esat::DrawSetStrokeColor(255,255,255);
    //TODO : cambiar color dependiendo de la mascara de color de la pantalla
    DrawRectangle(position, {pixel_size-1,pixel_size-1});
}*/

void DrawBody(Body_t *b){
    int c = 0; // Contamos cuantos bits de una row podemos dibujar en una sola call (parecido a greedy meshing)
    int ox = 0;

    unsigned char RGBA[4] = {255, 255, 255, 255};

    for (int y = 0; y < b->mask.size.y; y++){
        for (int x = 0; x < b->mask.size.x; x++){

            if ( ReadBit(&b->mask, x, y) ) {
                if (c == 0)
                    ox = x;
                c++;
            } else {
                if (c > 0) DrawRectangle(
                    {
                        b->position.x + ox,
                        b->position.y + y
                    },
                    { c, 1 },
                    RGBA
                );
                c = 0;
            }

        }

        if (c > 0) DrawRectangle(
            {
                b->position.x + ox,
                b->position.y + y
            },
            { c, 1 },
            RGBA
        );
        c = 0;
    }
}

#endif // RENDER_CC