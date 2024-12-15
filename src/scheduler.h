#include "stdio.h"
#ifndef __SCHEDULER_H
#define __SCHEDULER_H
#define MAX_PROCESSES 10  // Máximo número de procesos
enum ProcessState { NEW, READY, RUNNING, BLOCKED, TERMINATED };

// Representación de un proceso
struct Process {
    int pid;
    ProcessState state;
    int remainingTime;
};

// Scheduler: Planificador simple
class Scheduler {
private:
    Process processQueue[MAX_PROCESSES];
    int processCount = 0;
    int timeSlice = 1; // Duración de un turno en ciclos
    int currentProcessIndex = -1;

public:
    void addProcess(Process p) {
        if (processCount < MAX_PROCESSES) {
            processQueue[processCount++] = p;
        }
    }

    void run(Screen s) {
        while (processCount > 0) {
            currentProcessIndex = (currentProcessIndex + 1) % processCount;
            Process &current = processQueue[currentProcessIndex];

            if (current.state == READY || current.state == RUNNING) {
                current.state = RUNNING;
                executeProcess(current, s);
                current.remainingTime -= timeSlice;

                if (current.remainingTime <= 0) {
                    current.state = TERMINATED;
                    // Eliminar el proceso (desplazar los procesos)
                    for (int i = currentProcessIndex; i < processCount - 1; i++) {
                        processQueue[i] = processQueue[i + 1];
                    }
                    processCount--;
                    currentProcessIndex--; // Ajusta el índice tras eliminar
                } else {
                    current.state = READY;
                }
            }
        }
    }

    void executeProcess(Process &process, Screen s) {
        for (int i = 0; i < 3; i++)
        {
            s.print("Executing process: ");
            s.print((char*)process.pid);
            s.print("\n");
            delay(timeSlice);
        }
    }
};

#endif
