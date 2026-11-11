#ifndef UI_ELEMENT_CC
#define UI_ELEMENT_CC 1

#include "../core/cirno.cc"
#include "../entities/body.cc"
#include "asset-loader.cc"

// FUNCIONES PARA LA INTERFAZ DEL JUEGO

struct Character_t {
    bool active = false, flashing = false;
    Body_t body = { false };
};
struct QueuedCharacter_t {
    bool active = false;
    char character;
    Vector2_t position;
    long long int queued_tick;
};

const unsigned int Character_kRegistryCapacity = 100;
unsigned int Character_registry_active_count = 0;
Character_t Character_registry[Character_kRegistryCapacity] = {0};

const unsigned int Character_kGenerateQueueCapacity = 50;
QueuedCharacter_t Character_GenerateQueue[Character_kGenerateQueueCapacity] = {0};

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
        if (!Character_registry[n].active) free_space = true;
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

void GenerateCharacter(char c, Vector2_t position, bool flashing = false){
    Character_t *new_c = Character_Construct(c);
    new_c->body.mask = g_assets.CharacterMasks[TranslateCharToIndex(c)];
    new_c->body.position.y = position.y;
    new_c->body.position.x = position.x;
    new_c->flashing = flashing;
}
void GenerateNumber(int value, int digits, Vector2_t position, bool flashing = false) {
    if (value > pow(10, digits) - 1) 
        value = pow(10, digits) - 1;
    int number;
    for (int i = 0; i < digits; i++) {
        number = value / pow(10, digits - 1 - i);
        GenerateCharacter('0' + number, position, flashing);
        position.x += kCharacterWidth + 3;
        value %= pow(10, digits - 1 - i);
    }
}
void GenerateText(const char *text, int length, Vector2_t position, bool flashing = false){
    for (int i = 0; i < length; i++){
        if (text[i] != ' ') GenerateCharacter(text[i], position, flashing);
        position.x += kCharacterWidth + 3;
    }
}

void GenerateQueuedCharacter(QueuedCharacter_t *qc){
    GenerateCharacter(qc->character, qc->position);
    qc->active = false;
}
void QueuedCharactersUpdate(){
	for (int i = 0; i < Character_kGenerateQueueCapacity; i++){
		if ( Character_GenerateQueue[i].active && g_tick == Character_GenerateQueue[i].queued_tick ) 
            GenerateQueuedCharacter(&Character_GenerateQueue[i]);
	}
}
void GenerateProgressiveText(const char *text, int length, Vector2_t position, int initial_wait = 0){
    double queue_tick = g_tick + 6*initial_wait;
    for (int i = 0; i < length; i++){
        if (text[i] != ' '){
            int n = 0;
            bool free_space = false;
            while(n < Character_kGenerateQueueCapacity && !free_space){
                if (!Character_GenerateQueue[n].active) free_space = true;
                else n++;
            }
            if (free_space) {
                QueuedCharacter_t new_qc;
                new_qc.active = true;
                new_qc.character = text[i];
                new_qc.position = position;
                new_qc.queued_tick = queue_tick;
                Character_GenerateQueue[n] = new_qc;
            }
        }
        queue_tick += 6;
        position.x += kCharacterWidth + 3;
    }
    QueuedCharactersUpdate();
}
void ClearCharacterGenerateQueue(){
    for (int i = 0; i < Character_kGenerateQueueCapacity; i++){
        Character_GenerateQueue[i].active = false;
    }
}

#endif // UI_ELEMENT_CC