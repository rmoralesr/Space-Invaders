#ifndef UI_ELEMENT_CC
#define UI_ELEMENT_CC 1

#include "cirno.cc"
#include "body.cc"
#include "asset-loader.cc"

// FUNCIONES PARA LA INTERFAZ DEL JUEGO

struct Character_t {
    bool active = false;
    Body_t body = { false };
};

constexpr unsigned int Character_kRegistryCapacity = 20;
unsigned int Character_registry_active_count = 0;
Character_t Character_registry[kNumRegistry] = {0};

// Prototipaje

Character_t* Character_Construct();
void Destruct(Character_t* self);

void MoveTo(Character_t* self, Vector2_t pos);
void Move(Character_t* self, Vector2_t pos);

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

Character_t* Character_Construct(char c) {
    int n = 0;
    bool free_space = false;
    while(n < Character_kRegistryCapacity && !free_space){
        if (!Character_registry[n].is_active) free_space = true;
        else n++;
    }

    Character_t *self = nullptr;

    if (free_space) { 
        Character_registry[n] = {};
        self = &Character_registry[n];
        self->active = true;

        Character_registry_active_count++;
    }

    return self;
}

void Destruct(Character_t* self) {
  self->active = false;

  Character_registry_active_count--;
}

void Destruct(Character_t *character, unsigned int *registry_active_count) {
    character->active = false;
    (*registry_active_count)--;
}

void GenerateNumber(int value, int digits, Vector2_t position) {
    if (value > pow(10, digits) - 1) 
        value = pow(10, digits) - 1;

    int number;
    for (int i = 0; i < digits; i++) {
        number = value / pow(10, digits - 1 - i);
        Character_t *new_c = Character_Construct('0' + number);
        new_c->position.y = position.y;
        new_c->position.x = position.x;
        position.x += 8; //Acceder a la constante de CharacterWidth
        value %= pow(10, digits - 1 - i);
    }
}

void GenerateText(const *char text, int length, Vector2_t position){
    for (int i = 0; i < length; i++){
        Character_t *new_c = Character_Construct(text[i]);
        new_c->position.y = position.y;
        new_c->position.x = position.x;
        position.x += 8; //Acceder a la constante de CharacterWidth
    }
}


#endif // UI_ELEMENT_CC