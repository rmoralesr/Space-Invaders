#ifndef UI_CC
#define UI_CC 1

#include "cirno.cc"
#include "body.cc"
#include "asset-loader.cc"

// FUNCIONES PARA LA INTERFAZ DEL JUEGO

namespace character {
    constexpr unsigned int kNumRegistry = 100; // Profunidad del registro

    body::Type Registry[kNumRegistry] = {};
    // struct character (?)
    // o array (Registro) de characteres character::Registry

    /*
    ClearBody(body::Type *b){
        b->can_collide = false;
        b->position = {0,0};
        b->mask = {};
    }
    */

    void ClearRegistry(){
        for (int i = 0; i<kNumRegistry; i++){
            //ClearBody(&Registry[i]); //FUNCION SIN IMPLEMENTAR
        }
    }

    void Construct(char c, vector2::Type position) {
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

    void Destruct(body::Type *character) {
        int index = 0;
        do{
            if (Registry[index].position.x == character->position.x &&  
            Registry[index].position.y == character->position.y) {
                //ClearBody(character); //FUNCION SIN IMPLEMENTAR
                index = -1;
            } else {
                index++;
            }
        } while(index != -1 && index < kNumRegistry);
    }

    void GenerateNumber(int value, int digits, vector2::Type position) {
        if (value > pow(10, digits) - 1) 
            value = pow(10, digits) - 1;

        int number;
        for (int i = 0; i < digits; i++) {
            number = value / pow(10, digits - 1 - i);

            Construct('0' + number, position);

            position.x += assets::kCharacterWidth;

            value %= pow(10, digits - 1 - i);
        }
    }

}



#endif // UI_CC