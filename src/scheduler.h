#include "stdio.h"
#ifndef __SCHEDULER_H
#define __SCHEDULER_H
#define MAX_PROC 10  // Máximo número de procesos
enum EstadosDeProceso { NEW, READY, RUNNING, BLOCKED, TERMINATED };

// Representación de un proceso
struct Proceso {
    int pid;
    EstadosDeProceso estado;
    int tiempoRestante;
};

// Scheduler: Planificador simple
class Scheduler {
private:
    Proceso processQueue[MAX_PROC];
    int contPro = 0;
    int tiempoCiclo = 5; // Duración de un turno en ciclos, gestionado con Ron Robins
    int indexProcActual = -1;
    

public:
    Screen screen;
    void addProcess(Proceso p) {
        if (contPro < MAX_PROC) {
            processQueue[contPro++] = p;
        }
    }

    void run() {
        while (contPro > 0) {
            indexProcActual = (indexProcActual + 1) % contPro;
            Proceso &actual = processQueue[indexProcActual];

            if (actual.estado == READY || actual.estado == RUNNING) {
                actual.estado = RUNNING;
                ejecutarProceso(actual);
                actual.tiempoRestante -= tiempoCiclo;

                if (actual.tiempoRestante <= 0) {
                    actual.estado = TERMINATED;
                    // Eliminar el proceso (desplazar los procesos)
                    for (int i = indexProcActual; i < contPro - 1; i++) {
                        processQueue[i] = processQueue[i + 1];
                    }
                    contPro--;
                    indexProcActual--; // Ajusta el índice tras eliminar
                } else {
                    actual.estado = READY;
                }
            }
        }
    }

    void ejecutarProceso(Proceso &proceso) {
        screen.print("Ejecutando proceso: ");
        screen.print((char)proceso.pid+'0');  // Imprime el pid como cadena
        screen.print("\n");  // Salto de línea después de cada impresión
        delay(tiempoCiclo);
        }
};

#endif
