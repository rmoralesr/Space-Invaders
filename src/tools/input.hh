#ifndef INPUT_HH
#define INPUT_HH 1

#include <esat/input.h>

struct Input {
    bool insert_credit;
    bool left_button;
    bool right_button;
    bool action_button;
    bool one_player_button;
    bool two_player_button;
};

void InputUpdate(Input *input);

#endif //INPUT_HH