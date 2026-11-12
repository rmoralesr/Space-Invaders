#ifndef UI_ELEMENT_CC
#define UI_ELEMENT_CC 1

#include "../core/cirno.cc"
#include "../entities/body.cc"
#include "asset-loader.cc"

// FUNCIONES PARA LA INTERFAZ DEL JUEGO

struct UIElement_t {
    bool active = false, flashing = false;
    Body_t body = { false };
};
struct QueuedUIElement_t {
    bool active = false;
    UIElement_t element;
    long long int queued_tick;
};
struct WipeScreen {
    Vector2_t position;
    Vector2_t size;
    int step_size = 16, current_step = -1;
    long long int queued_tick;
};

const unsigned int UIElement_kRegistryCapacity = 200;
unsigned int UIElement_registry_active_count = 0;
UIElement_t UIElement_registry[UIElement_kRegistryCapacity] = {0};

const unsigned int UIElement_kGenerateQueueCapacity = 200;
QueuedUIElement_t UIElement_GenerateQueue[UIElement_kGenerateQueueCapacity] = {0};

const unsigned int ProgressiveTextTickRate = 2;

WipeScreen g_wipe_screen;

// Prototipaje

UIElement_t* UIElement_Construct();
void Destruct(UIElement_t* self);

void MoveTo(UIElement_t* self, Vector2_t pos);
void Move(UIElement_t* self, Vector2_t pos);

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

UIElement_t* UIElement_Construct() {
    int n = 0;
    bool free_space = false;
    while(n < UIElement_kRegistryCapacity && !free_space){
        if (!UIElement_registry[n].active) free_space = true;
        else n++;
    }
    UIElement_t *self = nullptr;
    if (free_space) { 
        UIElement_registry[n] = {};
        self = &UIElement_registry[n];
        self->active = true;
        UIElement_registry_active_count++;
    }
    return self;
}
void Destruct(UIElement_t* self) {
  self->active = false;
  UIElement_registry_active_count--;
}

void GenerateUIElement(Body_t b, bool flashing = false){
    for (int i = 0; i < UIElement_kRegistryCapacity; i++){
        if (UIElement_registry[i].body.position.x == b.position.x &&
        UIElement_registry[i].body.position.y == b.position.y) Destruct(&UIElement_registry[i]);
    }
    UIElement_t *new_c = UIElement_Construct();
    new_c->body = b;
    new_c->flashing = flashing;
}
void GenerateQueuedUIElement(QueuedUIElement_t *qc){
    GenerateUIElement(qc->element.body, qc->element.flashing);
    qc->active = false;
}
void ClearUIElementGenerateQueue(){
    for (int i = 0; i < UIElement_kGenerateQueueCapacity; i++){
        UIElement_GenerateQueue[i].active = false;
    }
}
void QueuedUIElementsUpdate(){
	for (int i = 0; i < UIElement_kGenerateQueueCapacity; i++){
		if ( UIElement_GenerateQueue[i].active && g_tick == UIElement_GenerateQueue[i].queued_tick ) 
            GenerateQueuedUIElement(&UIElement_GenerateQueue[i]);
	}
}
void QueueUIElement(Body_t b, long long int queue_tick = 0, bool flashing = false){
    int n = 0;
    bool free_space = false;
    while(n < UIElement_kGenerateQueueCapacity && !free_space){
        if (!UIElement_GenerateQueue[n].active) free_space = true;
        else n++;
    }
    if (free_space) {
        UIElement_GenerateQueue[n].active = true;
        UIElement_GenerateQueue[n].queued_tick = queue_tick;

        UIElement_GenerateQueue[n].element.active = true;
        UIElement_GenerateQueue[n].element.flashing = flashing;
        UIElement_GenerateQueue[n].element.body = b;
    }
}

// Clear
void ClearUIContent(){
    for (int i = 0; i < UIElement_kRegistryCapacity; i++){
        if (UIElement_registry[i].body.position.y > 33 &&
        UIElement_registry[i].body.position.y < 33+207) Destruct(&UIElement_registry[i]);
    }
}

// Texto
void GenerateText(const char *text, int length, Vector2_t position, int delay = 0, bool flashing = false){
    for (int i = 0; i < length; i++){
        if (text[i] != ' ') {

            Body_t b = {false, position, CharMask(text[i])};
            if (delay == 0) GenerateUIElement(b, flashing);
            else QueueUIElement(b, g_tick+delay, flashing);
        }
        position.x += kCharacterWidth + 3;
    }
}
void GenerateProgressiveText(const char *text, int length, Vector2_t position, 
    int initial_wait = 0, bool ignore_space = false, int wait_rate = ProgressiveTextTickRate, bool flashing = false){
    long long int queue_tick = g_tick + initial_wait;
    for (int i = 0; i < length; i++){
        if (text[i] != ' ') {
            Body_t b = {false, position, CharMask(text[i])};
            QueueUIElement(b, queue_tick, flashing);
        }
        if (text[i] != ' ' || !ignore_space) queue_tick += wait_rate;
        position.x += kCharacterWidth + 3;
    }
    if (initial_wait == 0) QueuedUIElementsUpdate();
}
void GenerateNumber(int value, int digits, Vector2_t position, int delay = 0, bool flashing = false) {
    if (value > pow(10, digits) - 1) 
        value = pow(10, digits) - 1;
    int number;
    for (int i = 0; i < digits; i++) {
        number = value / pow(10, digits - 1 - i);

        Body_t b = {false, position, CharMask('0'+number)};
        if (delay == 0) GenerateUIElement(b, flashing);
        else QueueUIElement(b, g_tick+delay, flashing);

        position.x += kCharacterWidth + 3;
        value %= pow(10, digits - 1 - i);
    }
}

//Wipe Screen

void AdvanceWipeScreen(WipeScreen *wp){
    if (wp->current_step != -1){
        wp->current_step += 1;
        if (wp->current_step*wp->step_size >= wp->size.x) wp->current_step = -1;
    }
}
void WipeScreenToContent(WipeScreen *wp){
    wp->position = {0,33};
    wp->size = {kWindowSize.x, 207};
}

#endif // UI_ELEMENT_CC