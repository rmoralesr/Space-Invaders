#include "input.hh"

void InputUpdate(Input *input) {
    input->insert_credit = esat::IsSpecialKeyDown(esat::kSpecialKey_Enter);
    input->left_button = esat::IsSpecialKeyPressed(esat::kSpecialKey_Left) || esat::IsKeyPressed('A');
    input->right_button = esat::IsSpecialKeyPressed(esat::kSpecialKey_Right) || esat::IsKeyPressed('D');
    input->action_button = esat::IsSpecialKeyDown(esat::kSpecialKey_Space);
    input->one_player_button = esat::IsKeyDown('1');
    input->two_player_button = esat::IsKeyDown('2');
}