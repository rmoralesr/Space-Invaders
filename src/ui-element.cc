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

constexpr unsigned int Character_kRegistryCapacity = 100; // Profunidad del registro
unsigned int Character_registry_active_count = 0;
Character_t Character_registry[kNumRegistry] = {0};

// Deberiamos seguir este prototipaje/template?
Character_t* Construct(char c, Vector2_t position);
void Destruct(char c, Vector2_t position);
void Update(); // Llama el update de body


Character_t* Construct(char c, Vector2_t position) {
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
    

    // body = GenerateBody(TextSprites[c], position) //FUNCION SIN IMPLEMENTAR
    // Añadir body al registro de "character"
}

void Destruct(Character_t *character) {
    character->alive = false;
    registry_alive_count--;
}





// -------------------------------------------------------------------- //

// Esto deberia cambiarse
void GenerateNumber(int value, int digits, Vector2_t position) {
    if (value > pow(10, digits) - 1) 
        value = pow(10, digits) - 1;

    int number;
    for (int i = 0; i < digits; i++) {
        number = value / pow(10, digits - 1 - i);

        Construct('0' + number, position);

        position.x += kCharacterWidth;

        value %= pow(10, digits - 1 - i);
    }
}




#endif // UI_ELEMENT_CC