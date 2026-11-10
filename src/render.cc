#ifndef RENDER_CC
#define RENDER_CC 1

#include <types.cc>

// FUNCIONES DE RENDERIZADO

void DrawRectangle(vector2::Type position, vector2::Type size){
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

void DrawPixel(vector2::Type position, int pixel_size){
    esat::DrawSetFillColor(255,255,255);
    esat::DrawSetStrokeColor(255,255,255);
    //FALTA : cambiar color dependiendo de la mascara de color de la pantalla
    DrawRectangle(position, {pixel_size-1,pixel_size-1});
}

void DrawBody(body::Type b){
    for (int i = 0; i < b.mask.size.x; i++){
        for (int j = 0; j < b.mask.size.y; j++){
            if ( ReadBit(&b.mask, i, j) ) DrawPixel({b.position.x+i, b.position.y+j});
        }
    }
}

/** @brief Dibuja el juego en la ventana */
void GameRender(Game *game) {
    // TODO
}

#endif // RENDER_CC