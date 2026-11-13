#ifndef UI_ELEMENT_CC
#define UI_ELEMENT_CC 1

#include "../core/cirno.cc"
#include "../core/types.cc"
#include "../entities/body.cc"
#include "asset-loader.cc"

// FUNCIONES PARA LA INTERFAZ DEL JUEGO

struct UIElement_t {
    bool active = false, flashing = false;
    Body_t body = { false };
    long long int queued_tick;
};

struct AnimatedUIElement_t {
    bool active;
    Vector2_t delta;
    UIElement_t element;
    Mask_t alt_mask;
    long long int end_tick;
};

struct WipeScreen {
    Vector2_t position;
    Vector2_t size;
    int step_size = 16, current_step = -1;
    long long int queued_tick = 0;
};

const unsigned int UIElement_kRegistryCapacity = 200;
unsigned int UIElement_registry_active_count = 0;
UIElement_t UIElement_registry[UIElement_kRegistryCapacity] = {0};

const unsigned int AnimatedUIElement_kRegistryCapacity = 5;
unsigned int AnimatedUIElement_registry_active_count = 0;
AnimatedUIElement_t AnimatedUIElement_registry[AnimatedUIElement_kRegistryCapacity] = {0};

const unsigned int ProgressiveTextTickRate = 6;

const Vector2_t kCreditsNumberPosition = {200,240};
const Vector2_t kScore1NumberPosition = {10 + 8*2 , 9 + 8*2};
const Vector2_t kScore2NumberPosition = {10 + 8*20, 9 + 8*2};
const Vector2_t kHiScoreNumberPosition = {10 + 8*10, 9 + 8*2};

WipeScreen g_wipe_screen;
bool g_alt_title_anim = true;

// Prototipaje

UIElement_t* UIElement_Construct();
void Destruct(UIElement_t* self);

// Definiciones

char TranslateCharToIndex(char c){
    if (c >= 'a' && c <= 'z') {
        c -= 'a';
    } else {
        if (c >= '0' && c <= '9') c -= '0' - 26;
        else switch(c){
            case '<' : c = 10+26 + 0; break;
            case '>' : c = 10+26 + 1; break;
            case '=' : c = 10+26 + 2; break;
            case '*' : c = 10+26 + 3; break;
            case '?' : c = 10+26 + 4; break;
            case '-' : c = 10+26 + 5; break;
        }
    }
    return c;
}
Mask_t CharMask(char c){
    return g_assets.CharacterMasks[TranslateCharToIndex(c)];
}

void Destruct(UIElement_t* self) {
  self->active = false;
  UIElement_registry_active_count--;
}
void Destruct(AnimatedUIElement_t* self) {
  self->active = false;
  AnimatedUIElement_registry_active_count--;
}
void EraseUIElement(Vector2_t position, int exluded_index = -1){
    for (int j = 0; j < UIElement_kRegistryCapacity; j++){
        if (UIElement_registry[j].body.position.x == position.x &&
            UIElement_registry[j].body.position.y == position.y &&
            UIElement_registry[j].queued_tick <= g_tick && j != exluded_index) Destruct(&UIElement_registry[j]);
    }
}
void EraseText(Vector2_t position, int length = 1){
    for (int i = 0; i < length; i++){
        EraseUIElement(position);
        position.x += kCharactersWidth + 3;
    }
}

void GenerateUIElement(Body_t b, long long int tick = 0, bool flashing = false){
    int n = 0;
    bool free_space = false;
    while(n < UIElement_kRegistryCapacity && !free_space){
        if (!UIElement_registry[n].active) free_space = true;
        else n++;
    }
    if (free_space) { 
        UIElement_registry[n].active = true;
        UIElement_registry[n].body = b;
        UIElement_registry[n].flashing = flashing;
        UIElement_registry[n].queued_tick = tick;
        UIElement_registry_active_count++;
    }
}
void GenerateAnimatedUIElement(Body_t b, long long int start_tick, long long int end_tick, Vector2_t delta, Mask_t alt_mask, bool flashing = false){
    int n = 0;
    bool free_space = false;
    while(n < AnimatedUIElement_kRegistryCapacity && !free_space){
        if (!AnimatedUIElement_registry[n].active) free_space = true;
        else n++;
    }
    if (free_space) { 
        AnimatedUIElement_registry[n].element.active = true;
        AnimatedUIElement_registry[n].element.body = b;
        AnimatedUIElement_registry[n].element.flashing = flashing;
        AnimatedUIElement_registry[n].element.queued_tick = start_tick;

        AnimatedUIElement_registry[n].active = true;
        AnimatedUIElement_registry[n].end_tick = end_tick;
        AnimatedUIElement_registry[n].delta = delta;
        AnimatedUIElement_registry[n].alt_mask = alt_mask;

        AnimatedUIElement_registry_active_count++;
    }
}

// Clear
void ClearUIContent(){
    for (int i = 0; i < UIElement_kRegistryCapacity; i++){
        if (UIElement_registry[i].body.position.y > 33 &&
        UIElement_registry[i].body.position.y < 33+207) Destruct(&UIElement_registry[i]);
    }
    for (int i = 0; i < AnimatedUIElement_kRegistryCapacity; i++){
        Destruct(&AnimatedUIElement_registry[i]);
    }
}

// Texto
void GenerateText(const char *text, Vector2_t position, 
    int start_tick = 0, int wait_rate = 0, bool flashing = false, bool ignore_space = false){
    long long int queue_tick = start_tick;
    if (queue_tick == 0) queue_tick = g_tick;
    int i = 0;
    while(text[i] != '\0'){
        if (text[i] != ' ') {
            Body_t b = {false, position, CharMask(text[i])};
            GenerateUIElement(b, queue_tick, flashing);
        }
        if (text[i] != ' ' || !ignore_space) queue_tick += wait_rate;
        position.x += kCharactersWidth + 3;
        i++;
    }
}
void GenerateNumber(int value, int digits, Vector2_t position, int start_tick = 0, bool flashing = false) {
    if (start_tick == 0) start_tick = g_tick;
    if (value > pow(10, digits) - 1) 
        value = pow(10, digits) - 1;
    int number;
    for (int i = 0; i < digits; i++) {
        number = value / pow(10, digits - 1 - i);

        Body_t b = {false, position, CharMask('0'+number)};
        GenerateUIElement(b, start_tick, flashing);

        position.x += kCharactersWidth + 3;
        value %= pow(10, digits - 1 - i);
    }
}

//Wipe Screen

void AdvanceWipeScreen(WipeScreen *wp){
    if (wp->current_step != -1){
        wp->current_step += 1;
    }
}
void WipeScreenToContent(WipeScreen *wp){
    wp->position = {0,33};
    wp->size = {kWindowSize.x, 207};
}
void WipeScreenToFull(WipeScreen *wp){
    wp->position = {0,0};
    wp->size = {kWindowSize.x, kWindowSize.y};
}

#endif // UI_ELEMENT_CC