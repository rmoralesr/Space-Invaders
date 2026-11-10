#ifndef INPUT_CC
#define INPUT_CC 1

#include <esat/input.h>

struct Input {
    bool add_credit;
    bool left_button;
    bool right_button;
    bool action_button;
};

void InputUpdate(Input *input) {
    input->add_credit = esat::IsSpecialKeyDown(esat::kSpecialKey_Enter);
    input->left_button = esat::IsSpecialKeyPressed(esat::kSpecialKey_Left);
    input->left_button = esat::IsKeyPressed('A');
    input->right_button = esat::IsSpecialKeyPressed(esat::kSpecialKey_Right);
    input->right_button = esat::IsKeyPressed('D');
    input->action_button = esat::IsSpecialKeyPressed(esat::kSpecialKey_Space);
}

#endif //INPUT_CC