#ifndef UI_CC
#define UI_CC 1

// FUNCIONES PARA LA INTERFAZ DEL JUEGO

void GenerateCharacter(unsigned char c, vector2:Vector2_t position) {
    if (c >= '0' && c <= '9') {
        c -= '0';
    } else {
        if (c >= 'a' && c <= 'z') c -= 'a';
    }
    // body = GenerateBody(TextSprites[c], position) //FUNCION SIN IMPLEMENTAR
    // Añadir body a la array de objetos "character"
}

void GenerateNumberCharacters(int value, int digits, vector2:Vector2_t position) {
    if (value > pow(10, digits) - 1) value = pow(10, digits) - 1;
    for (int i = 0; i < digits; i++) {
        int number = value / pow(10, digits - 1 - i);
        GenerateCharacter('0' + number, position);
        position.x += kCharacterWidth;
        value %= pow(10, digits - 1 - i);
    }
}

#endif // UI_CC