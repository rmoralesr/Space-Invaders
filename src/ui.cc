#ifndef UI_CC
#define UI_CC 1

#include "cirno.cc"
#include "body.cc"

// FUNCIONES PARA LA INTERFAZ DEL JUEGO

namespace character {
    constexpr unsigned int kNumRegistry = 100; // Profunidad del registro
    constexpr unsigned int kCharacterWidth = 5; // Leer del sprite
    constexpr unsigned int kCharacterHeight = 8; // Leer del sprite

    // struct character (?)
    // o array (Registro) de characteres character::Registry

    void Construct(unsigned char c, vector2::Type position) {
        if (c >= '0' && c <= '9') {
            c -= '0';
        } else {
            if (c >= 'a' && c <= 'z') c -= 'a';
        }
        // body = GenerateBody(TextSprites[c], position) //FUNCION SIN IMPLEMENTAR
        // Añadir body al registro de "character"
    }

    void Destruct() {
        // Quita el body a del registro
    }

    void GenerateNumber(int value, int digits, vector2::Type position) {
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

}



#endif // UI_CC