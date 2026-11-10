#ifndef RENDER_CC
#define RENDER_CC 1

#include <esat/draw.h>

#include "types.cc"

// FUNCIONES DE RENDERIZADO

void DrawRectangle(vector2::Type position, vector2::Type size, unsigned char RGBA[4]){
    esat::DrawSetFillColor(RGBA[0], RGBA[1], RGBA[2]);
    esat::DrawSetStrokeColor(RGBA[0], RGBA[1], RGBA[2]);

    float points[10];
    points[0] = position.x;
    points[1] = position.y;
    points[2] = position.x+size.x;
    points[3] = position.y;
    points[4] = position.x+size.x;
    points[5] = position.y+size.y;
    points[6] = position.x;
    points[7] = position.y+size.y;
    points[8] = position.x;
    points[9] = position.y;
    esat::DrawSolidPath(points, 5);
}

// En desuso
void DrawPixel(vector2::Type position, int pixel_size){
    esat::DrawSetFillColor(255,255,255);
    esat::DrawSetStrokeColor(255,255,255);
    //TODO : cambiar color dependiendo de la mascara de color de la pantalla
    DrawRectangle(position, {pixel_size-1,pixel_size-1});
}

void DrawBody(body::Type *b){
    int c = 0; // Contamos cuantos bits de una row podemos dibujar en una sola call (parecido a greedy meshing)
    int ox = 0;

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
                    { c, 1 }
                );
                c = 0;
            }

        }

        if (c > 0) DrawRectangle(
            {
                b->position.x + ox,
                b->position.y + y
            },
            { c, 1 }
        );
        c = 0;
    }
}

/** @brief Dibuja el juego en la ventana */
void GameRender(Game *game) {
    // TODO
}

#endif // RENDER_CC