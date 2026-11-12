#ifndef UI_ELEMENT_CC
#define UI_ELEMENT_CC 1

#include <stdio.h>

#include "../core/cirno.cc"
#include "../entities/body.cc"
#include "asset-loader.cc"

// FUNCIONES PARA LA INTERFAZ DEL JUEGO

struct UIElement_t {
    bool active = false, flashing = false;
    
    long long int reveal_tick = 0;

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
/*
UIElement_t* UIElement_Construct(bool flashing = false);
void Destruct(UIElement_t* self);

char CharToMaskIndex(char c);
Mask_t GetCharMask(char c);
*/
// Texto

/** @brief elimina todo lo que sea texto de la pantalla */
void ClearText();

void GenerateText(
	const char *text, 
	Vector2_t position, 
	unsigned int delay = 0, 
	bool flashing = false,

	UIElement_t **outElements = nullptr // pointer a un array de pointers
);

void GenerateProgressiveText(
	const char *text, 
	Vector2_t position, 
	unsigned int delay = 0, 
	unsigned int interval = 6, 
	bool flashing = false,

	UIElement_t **outElements = nullptr
);

void GenerateNumber(
	unsigned int value,
	unsigned int digits,
	Vector2_t position, 
	unsigned int delay = 0, 
	bool flashing = false,

	UIElement_t **outElements = nullptr
);

// Definiciones

UIElement_t* UIElement_Construct(bool flashing) {
	int n = 0;
	bool free_space = false;
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

QueuedUIElement_t QueueUIElement(){
	QueuedUIElement_t *self = nullptr;
	
	if (free_space) { 
		UIElement_registry_active_count++;

		UIElement_registry[n] = {};
		self = &UIElement_registry[n];
		
		self->active = true;
		self->flashing = flashing;
	}

	return self;
}

// Overloaded
/*UIElement_t* UIElement_Construct(long long int reveal_tick, bool flashing){
	UIElement_t *self = UIElement_Construct(flashing);

	self->reveal_tick = reveal_tick;

	return self;
}

void Destruct(UIElement_t* self) {
	if (self->active) {
		self->active = false;
  	UIElement_registry_active_count--;
	}
}*/

// Clear
void ClearUIContent(){
    for (int i = 0; i < UIElement_kRegistryCapacity; i++){
        if (UIElement_registry[i].body.position.y > 33 &&
        UIElement_registry[i].body.position.y < 33+207) Destruct(&UIElement_registry[i]);
    }
}

// Texto

void GenerateText(const char *text, Vector2_t position, unsigned int delay, bool flashing, UIElement_t **outElements) {
	GenerateProgressiveText(text, position, delay, 0, flashing, outElements);
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

void GenerateNumber(unsigned int value, unsigned int digits, Vector2_t position, unsigned int delay, bool flashing, UIElement_t **outElements) {
	if (digits == 0)
		digits = max(1, log(value, 10));

	if (value > pow(10, digits) - 1)
		  value = pow(10, digits) - 1;

	int number;

	for (int i = 0; i < digits; i++) {
		number = value / pow(10, digits - 1 - i);
		value %= pow(10, digits - 1 - i);

		UIElement_t *elem = UIElement_Construct(g_tick + delay, flashing);
		elem->body.position = position;
		elem->body.mask = GetCharMask('0' + number);

		if (outElements != nullptr)
			outElements[i] = elem;

		position.x += kCharacterWidth + 3;
	}
}


void ClearText() {
  for (int i = 0; i < UIElement_kRegistryCapacity; i++) {
		Destruct(&UIElement_registry[i]);
  }
}

char CharToMaskIndex(char c){
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

Mask_t GetCharMask(char c){
	return g_assets.CharacterMasks[CharToMaskIndex(c)];
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