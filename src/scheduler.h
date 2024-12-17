#include "stdio.h"
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
enum TipoProceso
{
    CPU,
    IO
}; // Tipos de ejecución
// Representación de un proceso
struct Proceso
{
    int pid;
    EstadosDeProceso estado;
    int tiempoRestante;
    int prioridad;
    int tiempos[3] = {0, 0, 0};            // Arreglo de tiempos: {CPU, IO, CPU}
    TipoProceso tipo[3] = {CPU, CPU, CPU}; // Default: proceso intensivo de CPU
    int etapa = 0;                         // Etapa actual del proceso (0 = primer CPU, 1 = IO, 2 = último CPU)
    // Sobrecarga del operador == para comparar procesos
    bool operator==(const Proceso &other) const
    {
        return this->pid == other.pid;
    }
    // Constructor por defecto, para evitar errores
    Proceso() : pid(0), estado(NEW), prioridad(0)
    {
        tiempos[0] = tiempos[1] = tiempos[2] = 0;
        tipo[0] = tipo[1] = tipo[2] = CPU;
    }
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
};

// Scheduler: Planificador simple
class Scheduler
{
private:
    Proceso processQueue[MAX_PROC];
    int cantPro = 0;
    int tiempoCiclo = 3; // Duración de un turno en ciclos, gestionado con Ron Robins
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
                        actual.estado = READY;
                }
                else
                { // aplica envejecimineto
                    actual.prioridad--;
                    for (int i = 0; i < cantPro; i++)
                    {
                        if (!(actual == processQueue[i]) && processQueue[i].prioridad < 10)
                        {
                            processQueue[i].prioridad++;
                        }
                    }
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
};
#endif
