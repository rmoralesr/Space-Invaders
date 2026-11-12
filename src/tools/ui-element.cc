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


const unsigned int UIElement_kRegistryCapacity = 100;
unsigned int UIElement_registry_active_count = 0;
UIElement_t UIElement_registry[UIElement_kRegistryCapacity] = {0};

// Prototipaje

UIElement_t* UIElement_Construct(bool flashing = false);
void Destruct(UIElement_t* self);

char CharToMaskIndex(char c);
Mask_t GetCharMask(char c);

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

	while(n < UIElement_kRegistryCapacity && !free_space){
		if (!UIElement_registry[n].active) free_space = true;
		else n++;
	}

	UIElement_t *self = nullptr;
	
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
UIElement_t* UIElement_Construct(long long int reveal_tick, bool flashing){
	UIElement_t *self = UIElement_Construct(flashing);

	self->reveal_tick = reveal_tick;

	return self;
}

void Destruct(UIElement_t* self) {
	if (self->active) {
		self->active = false;
  	UIElement_registry_active_count--;
	}
}

// Texto

void GenerateText(const char *text, Vector2_t position, unsigned int delay, bool flashing, UIElement_t **outElements) {
	GenerateProgressiveText(text, position, delay, 0, flashing, outElements);
}

void GenerateProgressiveText(const char *text, Vector2_t position, unsigned int delay, unsigned int interval, bool flashing, UIElement_t **outElements) {
  int c = 0;
	
	for (int i = 0; text[i] != '\0'; i++) { // \0 es el string terminator

		printf("%c", text[i]);

		if (text[i] != ' ') {
			
			UIElement_t *elem = UIElement_Construct(g_tick + delay + i*interval, flashing);
			elem->body.position = position;
			elem->body.mask = GetCharMask(text[i]);

			if (outElements != nullptr)
				outElements[c++] = elem; // c++... say that again?
		}

		position.x += kCharacterWidth + 3;
	}
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

#endif // UI_ELEMENT_CC