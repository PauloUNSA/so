#include "stdio.h"
#include <cstddef>

void bot(char* name, char* message, Screen screen) {
    screen.print(name);
    for(int i = 0; message[i] != '\0'; ++i)
    {
        screen.print(message[i]);
        delay(1);
    }
}

extern "C" void kernel()
{
    Screen screen;
    screen.print("Scheduler 1.0v\n\n");
    bot("HUMAN: ", "Hola mundo", screen);
    screen.print('\n');
    screen.print("EXIT COMPUTER");
}
