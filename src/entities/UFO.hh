#ifndef UFO_HH
#define UFO_HH 1

#include "body.hh"
#include "../core/score.hh"
#include "../tools/ui-element.hh"
#include "../tools/asset-loader.hh"

const int kUFOpoints[16] = {100, 50, 50,100,
                            150,100,100, 50,
                            300,100,100,100,
                            50,150,100, 50};

struct UFO_t {
    bool active;
    Body_t body;
    int points_index = 0;
    bool dir = true;
    long long int start_tick;
    int anim_tick_progress = 0;
};

extern UFO_t g_UFO;

void UFO_Init();

void GenerateUFO();
void EscapeUFO();

void DestroyUFO();

void TickUFO();
void AdvanceUFOPointsIndex();

#endif // UFO_HH