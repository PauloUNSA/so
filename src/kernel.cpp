#include "scheduler.h"
#include <cstddef>

/*void bot(char* name, char* message, Screen screen) {
    screen.print(name);
    for(int i = 0; message[i] != '\0'; ++i)
    {
        screen.print(message[i]);
        delay(1);
    }
}*/

extern "C" void kernel()
{
    Scheduler scheduler;
    scheduler.screen.print("Scheduler 1.0v\n\n");
    //bot("HUMAN: ", "Hola mundo", screen);
    //screen.print('\n');
    // Simulación de procesos
    scheduler.screen.print("Creando procesos...\n");
    
    scheduler.addProcess({1, READY, 15}); // PID 1, listo, 10 ciclos
    //scheduler.addProcess({2, READY, 15}); // PID 2, listo, 15 ciclos
    scheduler.run();
    //demo(screen);
    delay(10);
    //screen.print("EXIT COMPUTER");
}
