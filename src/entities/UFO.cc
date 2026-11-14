#ifndef UFO_CC
#define UFO_CC 1

#include "body.cc"
#include "../core/types.cc"
#include "../tools/asset-loader.cc"

const int kUFOpoints[16] = {100, 50, 50,100,
                            150,100,100, 50,
                            300,100,100,100,
                            50,150,100, 50};

struct UFO_t {
    bool active;
    Body_t b;
    int anim_frame, points_index;
    long long int start_tick;
};

UFO_t g_UFO;

#endif // UFO_CC