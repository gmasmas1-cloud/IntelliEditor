#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "worker.h"

// Cette fonction tourne dans un thread séparé (Arrière-plan)
void* thread_routine(void* arg) {
    WorkerTask* task = (WorkerTask*)arg;

    // Simulation de l'analyse (Ici David/Ndala feront leur travail)
    // On simule une attente de 2 secondes
    printf("[Thread] Analyse en cours...\n");
    
    // On appelle le callback pour prévenir l'UI que c'est fini
    task->callback("Analyse terminée avec succès");

    free(task->text);
    free(task);
    return NULL;
}

void start_async_analysis(char* text, void (*callback)(const char*)) {
    pthread_t thread;
    WorkerTask* task = malloc(sizeof(WorkerTask));
    task->text = text; // On passe la copie du texte
    task->callback = callback;

    // Création du thread POSIX
    pthread_create(&thread, NULL, thread_routine, task);
    pthread_detach(thread); // Le thread se libère tout seul après exécution
}