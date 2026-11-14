#ifndef DRAW_CC
#define DRAW_CC 1

#include <esat/draw.h>

#include "../tools/asset-loader.cc"
#include "../entities/body.cc"
#include "cirno.cc"

const unsigned int kPixelScale = 3;

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

    esat::SpriteGetPixel(g_assets.ColorMask, x/8,y/8, RGBA);
    
}

void SetToBlack(unsigned char RGBA[4]) {
    RGBA[0] = 0;
    RGBA[1] = 0;
    RGBA[2] = 0;
    RGBA[3] = 0;
}

static bool ShouldPaint(Body_t* self,
  unsigned int x, unsigned y,
  unsigned char RGBA[4], bool ex_nihilo
) {

    if (!ReadBit(&self->mask, x,y)) return false; // No hay pixel

    unsigned char new_RGBA[4];
    GetPositionColor(new_RGBA, self->position.x + x, self->position.y + y);

    bool ret = ex_nihilo || (
      new_RGBA[0] == RGBA[0]
      && new_RGBA[1] == RGBA[1]
      && new_RGBA[2] == RGBA[2]
      && new_RGBA[3] == RGBA[3]
    );

    RGBA[0] = new_RGBA[0];
    RGBA[1] = new_RGBA[1];
    RGBA[2] = new_RGBA[2];
    RGBA[3] = new_RGBA[3];

    return ret;
}

void Draw(Body_t* self) {

    unsigned char RGBA[4] = {255,255,255,255};

    int ox = 0; // Origin x
    int length = 0;

    for (int y = 0; y < self->mask.size.y; y++) {

        // x == size.x es lo que cierra
        for (int x = 0; x <= self->mask.size.x; x++) {

            if (
              (x < self->mask.size.x) // Si x == size.x, estamos fuera, por lo que hay que cerrar y pintar si queda length
              && ShouldPaint(self, x,y, RGBA, length == 0)
            ) {

                if (length == 0) ox = x;
                length++;

            } else if (length > 0) {
                
                DrawRectangle(
                  { self->position.x + ox, self->position.y + y },
                  { length, 1 },
                  RGBA
                );
                length = 0;

            }

        }

    }

}

#endif // DRAW_CC