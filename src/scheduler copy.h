/*#include "stdio.h"
#ifndef __SCHEDULER_H
#define __SCHEDULER_H
#define MAX_PROC 10      // Máximo número de procesos
#define PRIORIDAD_MAX 10 // Prioridad máxima
#define PRIORIDAD_MIN 1  // Prioridad mínima
enum EstadosDeProceso
{
    NEW,
    READY,
    RUNNING,
    BLOCKED,
    TERMINATED
};

// Representación de un proceso
/*struct Proceso {
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
    int cantPro = 0;
    int tiempoCiclo = 3; // Duración de un turno en ciclos, gestionado con Ron Robins
    int indexProcActual = -1;

public:
    Screen screen;
    void addProcess(Proceso p) {
        if (cantPro < MAX_PROC) {
            processQueue[cantPro++] = p;
        }
    }

    void run() {
        while (cantPro > 0) {
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
                    for (int i = indexProcActual; i < cantPro - 1; i++) {
                        processQueue[i] = processQueue[i + 1];
                    }
                    cantPro--;
                    indexProcActual--;  // Ajusta el índice tras eliminar
                } else {
                    for (int i = 0; i < cantPro - 1; i++) {
                        if (!(actual == processQueue[i]) && processQueue[i].prioridad < 10) {
                            processQueue[i].prioridad++;
                    }
                    }

                }
            }
            // Evitar la hambruna: si no se ejecutó ningún proceso, ajustamos las prioridades
            if (cantPro > 0 && indexProcActual == -1) {
                ajustarPrioridad();
            }
        }
    }
enum TipoProceso
{
    CPU,
    IO
}; // Tipos de ejecución

struct Proceso
{
    int pid;
    EstadosDeProceso estado;
    int tiempos[3] = {0, 0, 0}; // Arreglo de tiempos: {CPU, IO, CPU}
    int prioridad;
    TipoProceso tipo[3] = {CPU, CPU, CPU}; // Default: proceso intensivo de CPU
    int etapa = 0;                         // Etapa actual del proceso (0 = primer CPU, 1 = IO, 2 = último CPU)

    // Constructor para procesos de CPU intensivo
    Proceso(int pid, EstadosDeProceso estado, int tiempo, int prioridad)
        : pid(pid), estado(estado), prioridad(prioridad)
    {
        tiempos[0] = tiempo;
        tipo[0] = CPU;
    }

    // Constructor para procesos mixtos CPU-IO-CPU
    Proceso(int pid, EstadosDeProceso estado, int t1, int t2, int t3, int prioridad)
        : pid(pid), estado(estado), prioridad(prioridad)
    {
        tiempos[0] = t1;
        tiempos[1] = t2;
        tiempos[2] = t3;
        tipo[1] = IO;
    }

    // Sobrecarga del operador == para comparar procesos
    bool operator==(const Proceso &other) const
    {
        return this->pid == other.pid;
    }
};

class Scheduler
{
private:
    Proceso processQueue[MAX_PROC];
    int cantPro = 0;
    int tiempoCiclo = 3; // Duración de un turno en ciclos
    int indexProcActual = -1;

public:
    Screen screen;    
    void addProcess(Proceso p)
    {
        if (cantPro < MAX_PROC)
        {
            processQueue[cantPro++] = p;
        }
    }

    void run()
    {
        while (cantPro > 0)
        {
            indexProcActual = findMayorPrio();
            Proceso &actual = processQueue[indexProcActual];

            if (actual.estado == READY || actual.estado == RUNNING)
            {
                actual.estado = RUNNING;
                ejecutarProceso(actual);
                actual.tiempos[actual.etapa] -= tiempoCiclo;

                // Si la etapa actual termina
                if (actual.tiempos[actual.etapa] <= 0)
                {
                    actual.etapa++;

                    // Si completó todas las etapas
                    if (actual.etapa >= 3 || (actual.etapa == 1 && actual.tiempos[1] == 0))
                    {
                        actual.estado = TERMINATED;
                        eliminarProceso(indexProcActual);
                    }
                    else
                    {
                        actual.estado = READY;
                        for (int i = 0; i < cantPro - 1; i++)
                        {
                            if (!(actual == processQueue[i]) && processQueue[i].prioridad < 10)
                            {
                                processQueue[i].prioridad++;
                            }
                        }
                    }
                    // ajustarPrioridad(actual);
                }
            }
        }
    }
    void eliminarProceso(int index)
    {
        for (int i = index; i < cantPro - 1; i++)
        {
            processQueue[i] = processQueue[i + 1];
        }
        cantPro--;
        indexProcActual--;
    }

    void ejecutarProceso(Proceso &proceso)
    {
        screen.print("Ejecutando proceso: ");
        screen.print((char)proceso.pid + '0');
        screen.print(" - Tipo: ");
        if (proceso.tipo[proceso.etapa] == CPU)
        {
            screen.print("CPU\n");
        }
        else
        {
            screen.print("I/O\n");
        }
        delay(tiempoCiclo);
    }

    // Método para encontrar el proceso con mayor prioridad
    int findMayorPrio()
    {
        int indiceMayorPrio = -1;
        int mayorPrio = PRIORIDAD_MIN - 1; // Menor prioridad posible

        // Buscar el proceso con mayor prioridad
        for (int i = 0; i < cantPro; i++)
        {
            if (processQueue[i].estado == READY || processQueue[i].estado == RUNNING)
            {
                if (processQueue[i].prioridad > mayorPrio)
                {
                    mayorPrio = processQueue[i].prioridad;
                    indiceMayorPrio = i;
                }
            }
        }
        return indiceMayorPrio;
    }

    // Método para ajustar las prioridades de los procesos
    void ajustarPrioridad()
    {
        for (int i = 0; i < cantPro; i++)
        {
            if (processQueue[i].prioridad > PRIORIDAD_MIN)
            {
                processQueue[i].prioridad--; // Reducir prioridad para evitar la hambruna
            }
        }
    }

    /*void ejecutarProceso(Proceso &proceso) {
        screen.print("Ejecutando proceso: ");
        screen.print((char)proceso.pid+'0');  // Imprime el pid como cadena
        screen.print("\n");  // Salto de línea después de cada impresión
        delay(tiempoCiclo);
        }
};
#endif*/
