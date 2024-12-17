#include "types.h"

#ifndef __STDIO_H
#define __STDIO_H

// Define colores para la salida en pantalla, usando combinaciones de bits para especificar el color del texto y el fondo.
enum Color
{
    STANDARD_WHITE = 0x1F00,
    STANDARD_GREY = 0x0700,
    STANDARD_GREEN = 0x2a00
};

// Simula una pausa en el sistema iterando en un bucle vacío. Esto no es eficiente,
// pero es una solución básica para introducir retardos.
void delay(int seconds)
{
    int actual = 0;
    while (actual <= seconds * 100000000)
    {
        actual++;
    }
}

// Maneja la posición del cursor en la pantalla.
class Cursor
{
private:
    uint8_t x = 0, y = 0; // Coordenadas del cursor en la pantalla.
public:
    void setAction(bool isLineBreakCharacter)
    {
        if (isLineBreakCharacter || this->x >= 80)
        { // Decide si mover el cursor o iniciar una nueva línea.
            this->lineBreak();
        }
        else
        {
            this->moveCursor();
        }
    }

    int getCursor()
    { // calcula la posición en la memoria de video (0xb8000) basada en las coordenadas del cursor.
        return 80 * this->y + this->x;
    }

    void lineBreak()
    { // reinicia x y avanza a la siguiente línea (y++).
        this->x = 0;
        this->y++;
    }

    void moveCursor(int step = 1)
    {
        this->x += step; // Mueve el cursor horizontalmente
    }
};

class Screen
{
private:
    uint16_t *videoMemory = (uint16_t *)0xb8000;      // Dirección de memoria de video.
    uint16_t color = (uint16_t)Color::STANDARD_GREEN; // Color por defecto.
    Cursor cursor;                                    // Objeto `Cursor` para manejar posiciones.
    char finalTextCharacter = '\0';                   // Indica el fin de una cadena.
    char lineBreakCharacter = '\n';                   // Indica un salto de línea.

public:
    // Imprime una cadena caracter por caracter.
    void print(const char *str)
    {
        int i = 0;
        while (str[i] != this->finalTextCharacter)
        {
            this->print(str[i]);
            i++;
        }
    }

    // Simplifica la impresión de cadenas llamando al método Screen::print.
    void print(char str)
    {
        bool isLineBreak = (str == this->lineBreakCharacter); // Detectar salto de línea
        this->cursor.setAction(isLineBreak);                  // Actualiza la posición del cursor según el carácter

        if (!isLineBreak)
        {
            this->videoMemory[this->cursor.getCursor()] = (this->videoMemory[this->cursor.getCursor()] & this->color) | str; // Imprime el carácter
        }
    }
};

void print(char *str)
{
    Screen screen;
    screen.print(str);
}

#endif
