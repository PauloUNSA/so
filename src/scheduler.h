#include "stdio.h"
#ifndef __SCHEDULER_H
#define __SCHEDULER_H
#define MAX_PROC 10  // Máximo número de procesos
#define PRIORIDAD_MAX 10  // Prioridad máxima
#define PRIORIDAD_MIN 1  // Prioridad mínima
enum EstadosDeProceso { NEW, READY, RUNNING, BLOCKED, TERMINATED };

// Representación de un proceso
struct Proceso {
    int pid;
    EstadosDeProceso estado;
    int tiempoRestante;
    int prioridad;  // Nueva propiedad de prioridad
    // Sobrecarga del operador == para comparar procesos
    bool operator==(const Proceso &other) const {
        return this->pid == other.pid &&
               this->estado == other.estado &&
               this->tiempoRestante == other.tiempoRestante &&
               this->prioridad == other.prioridad;
    }

};

// Scheduler: Planificador simple
class Scheduler {
private:
    Proceso processQueue[MAX_PROC];
    int contPro = 0;
    int tiempoCiclo = 3; // Duración de un turno en ciclos, gestionado con Ron Robins
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
            // Encontrar el proceso con mayor prioridad
            indexProcActual = findMayorPrio();
            Proceso &actual = processQueue[indexProcActual];

            if (actual.estado == READY || actual.estado == RUNNING) {
                actual.estado = RUNNING;
                ejecutarProceso(actual);
                actual.tiempoRestante -= tiempoCiclo;

                // Si el proceso terminó
                if (actual.tiempoRestante <= 0) {
                    actual.estado = TERMINATED;
                    // Eliminar el proceso (desplazar los procesos)
                    for (int i = indexProcActual; i < contPro - 1; i++) {
                        processQueue[i] = processQueue[i + 1];
                    }
                    contPro--;
                    indexProcActual--;  // Ajusta el índice tras eliminar
                } else {
                    for (int i = 0; i < contPro - 1; i++) {
                        if (!(actual == processQueue[i]) && processQueue[i].prioridad < 10) {
                            processQueue[i].prioridad++;
                    }
                    }
                    
                }
            }
            // Evitar la hambruna: si no se ejecutó ningún proceso, ajustamos las prioridades
            if (contPro > 0 && indexProcActual == -1) {
                ajustarPrioridad();
            }
        }
    }

    // Método para encontrar el proceso con mayor prioridad
    int findMayorPrio() {
        int highestPriorityIndex = -1;
        int highestPriority = PRIORIDAD_MIN - 1;  // Menor prioridad posible

        // Buscar el proceso con mayor prioridad
        for (int i = 0; i < contPro; i++) {
            if (processQueue[i].estado == READY || processQueue[i].estado == RUNNING) {
                if (processQueue[i].prioridad > highestPriority) {
                    highestPriority = processQueue[i].prioridad;
                    highestPriorityIndex = i;
                }
            }
        }
        return highestPriorityIndex;
    }

    // Método para ajustar las prioridades de los procesos
    void ajustarPrioridad() {
        for (int i = 0; i < contPro; i++) {
            if (processQueue[i].prioridad > PRIORIDAD_MIN) {
                processQueue[i].prioridad--;  // Reducir prioridad para evitar la hambruna
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
