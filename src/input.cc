#ifndef INPUT_CC
#define INPUT_CC 1

#include <esat/input.h>

struct Input {
    bool insert_credit;
    bool left_button;
    bool right_button;
    bool action_button;
};

void InputUpdate(Input *input) {
    input->insert_credit = esat::IsSpecialKeyDown(esat::kSpecialKey_Enter);

    input->left_button = esat::IsSpecialKeyPressed(esat::kSpecialKey_Left) || esat::IsKeyPressed('A');
    input->right_button = esat::IsSpecialKeyPressed(esat::kSpecialKey_Right) || esat::IsKeyPressed('D');

    input->action_button = esat::IsSpecialKeyPressed(esat::kSpecialKey_Space);
}

#endif //INPUT_CC