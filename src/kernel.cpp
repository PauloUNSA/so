#include "scheduler.h"
#include <cstddef>

extern "C" void kernel()
{
    Scheduler scheduler;
    scheduler.screen.print("\nScheduler 1.0v\n\n");
    // Simulación de procesos
    scheduler.screen.print("Creando procesos...\n");
    scheduler.addProcess(Proceso(1, READY, 9, 5));            // Proceso CPU intensivo
    scheduler.addProcess(Proceso(2, READY, 6, 5, 10, 4));      // Proceso CPU-IO-CPU
    scheduler.addProcess(Proceso(3, READY, 15, 10));      // Proceso CPU intensivo
    scheduler.addProcess(Proceso(4, READY, 12, 9));      // Proceso CPU intensivo
    scheduler.run();
    delay(10);
}
