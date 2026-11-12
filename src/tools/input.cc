#ifndef INPUT_CC
#define INPUT_CC 1

#include <esat/input.h>

struct Input {
    bool insert_credit;
    bool left_button;
    bool right_button;
    bool action_button;
    bool one_player_button;
    bool two_player_button;
};

void InputUpdate(Input *input) {
    input->insert_credit = esat::IsSpecialKeyDown(esat::kSpecialKey_Enter);
    input->left_button = esat::IsSpecialKeyPressed(esat::kSpecialKey_Left) || esat::IsKeyPressed('A');
    input->right_button = esat::IsSpecialKeyPressed(esat::kSpecialKey_Right) || esat::IsKeyPressed('D');
    input->action_button = esat::IsSpecialKeyDown(esat::kSpecialKey_Space);
    input->one_player_button = esat::IsKeyDown('1');
    input->two_player_button = esat::IsKeyDown('2');
}

#endif //INPUT_CC