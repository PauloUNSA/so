#include "types.h"

#ifndef __STDIO_H
#define __STDIO_H

enum Color {
    STANDARD_WHITE = 0x1F00,
    STANDARD_GREY = 0x0700,
    STANDARD_GREEN = 0x2a00
};

void delay(int seconds) {
    int actual = 0;
    while(actual <= seconds*100000000) {
        actual++;
    }
}

class Cursor {
private:
    uint8_t x = 0, y = 0;
public:
    void setAction(bool isLineBreakCharacter) {
        if (isLineBreakCharacter || this->x >= 80) {
            this->lineBreak();
        } else {
            this->moveCursor();
        }
    }

    int getCursor() {
        return 80 * this->y + this->x;
    }

    void lineBreak() {
        this->x = 0; // Reinicia la columna
        this->y++;   // Pasa a la siguiente fila
    }

    void moveCursor(int step = 1) {
        this->x += step;  // Mueve el cursor horizontalmente
    }
};


class Screen {
private:
    uint16_t *videoMemory = (uint16_t *)0xb8000;
    uint16_t color = (uint16_t)Color::STANDARD_GREEN;
    Cursor cursor;
    char finalTextCharacter = '\0';
    char lineBreakCharacter = '\n';
public:
    // Método para imprimir un string
    void print(const char *str) {
        int i = 0;
        while (str[i] != this->finalTextCharacter) {
            this->print(str[i]);
            i++;
        }
    }

    // Método para imprimir un solo carácter
    void print(char str) {
        bool isLineBreak = (str == this->lineBreakCharacter);  // Detectar salto de línea
        this->cursor.setAction(isLineBreak);  // Actualiza la posición del cursor según el carácter

        if (!isLineBreak) {
            this->videoMemory[this->cursor.getCursor()] = (this->videoMemory[this->cursor.getCursor()] & this->color) | str;  // Imprime el carácter
        }
    }
};


void print(char *str) {
   Screen screen;
   screen.print(str);
}

#endif

