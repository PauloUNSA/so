#include "scheduler.h"
#include <cstddef>

extern "C" void kernel()
{
    Scheduler scheduler;
    scheduler.screen.print("\nScheduler 1.0v\n\n");
    // Simulación de procesos
    scheduler.screen.print("Creando procesos...\n");
    /*
    scheduler.addProcess({1, READY, 10, 5}); // PID 1, listo, 10 ciclos
    scheduler.addProcess({2, READY, 15, 7}); // PID 2, listo, 15 ciclos
    scheduler.addProcess({3, READY, 25, 8}); // PID 2, listo, 27 ciclos*/
    scheduler.addProcess(Proceso(1, READY, 10, 5));            // Proceso CPU intensivo
    scheduler.addProcess(Proceso(2, READY, 5, 5, 10, 4));      // Proceso CPU-IO-CPU
    scheduler.run();
    delay(10);
}
