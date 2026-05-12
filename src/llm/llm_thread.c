#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

/* On a besoin de llm_ask() définie dans llm_interface.c */
struct LLMEngine;
char* llm_ask(struct LLMEngine* engine, const char* prompt);
void  llm_nettoyer_reponse(char* response);

/*
 * LLMTask représente une tâche envoyée au thread LLM.
 * C'est comme un ticket de commande dans un restaurant :
 * - le prompt  = la commande
 * - la réponse = le plat préparé
 * - done       = le plat est prêt ou pas
 */
typedef struct {
    char*  prompt;    /* la question posée au LLM        */
    char*  response;  /* la réponse du LLM               */
    int    done;      /* 1 = terminé, 0 = en cours       */
    int    error;     /* 1 = erreur survenue             */
    pthread_mutex_t mutex; /* verrou pour cette tâche    */
    pthread_cond_t  cond;  /* sonnette pour cette tâche  */
} LLMTask;

/*
 * LLMThreadPool est notre gestionnaire de thread.
 * Il contient :
 * - le thread worker
 * - la file d'attente des tâches (max 32)
 * - les outils de synchronisation
 */
typedef struct {
    struct LLMEngine* engine;     /* le moteur LLM            */
    LLMTask*  queue[32];          /* file d'attente circulaire */
    int       head;               /* indice de lecture         */
    int       tail;               /* indice d'écriture         */
    int       count;              /* nombre de tâches en attente */
    pthread_t       thread;       /* le thread worker          */
    pthread_mutex_t mutex;        /* verrou du pool            */
    pthread_cond_t  cond;         /* sonnette du pool          */
    int       running;            /* 1 = actif, 0 = arrêté    */
} LLMThreadPool;

/* Variable globale — un seul pool dans tout le programme */
static LLMThreadPool* g_pool = NULL;

/*
 * llm_worker() — la fonction exécutée par le thread en arrière-plan.
 *
 * Cette fonction tourne en boucle infinie :
 * 1. Elle attend qu'une tâche arrive dans la file
 * 2. Elle prend la tâche
 * 3. Elle appelle llm_ask()
 * 4. Elle marque la tâche comme terminée
 * 5. Elle recommence
 */
static void* llm_worker(void* arg) {
    LLMThreadPool* pool = (LLMThreadPool*)arg;

    while (1) {
        /* Verrouiller pour regarder la file */
        pthread_mutex_lock(&pool->mutex);

        /* Attendre tant que la file est vide ET qu'on tourne encore
         * pthread_cond_wait déverrouille le mutex et attend la sonnette
         * Quand la sonnette sonne, il reverrouille et continue */
        while (pool->count == 0 && pool->running)
            pthread_cond_wait(&pool->cond, &pool->mutex);

        /* Si on doit s'arrêter et qu'il n'y a plus de tâches → sortir */
        if (!pool->running && pool->count == 0) {
            pthread_mutex_unlock(&pool->mutex);
            break;
        }

        /* Prendre la prochaine tâche dans la file */
        LLMTask* task = pool->queue[pool->head];
        pool->head  = (pool->head + 1) % 32; /* avancer circulairement */
        pool->count--;

        pthread_mutex_unlock(&pool->mutex);

        /* Traiter la tâche — llm_ask() peut prendre 30 secondes */
        char* result = llm_ask(pool->engine, task->prompt);
        if (result) llm_nettoyer_reponse(result);

        /* Marquer la tâche comme terminée
         * On verrouille la tâche elle-même pour protéger ses données */
        pthread_mutex_lock(&task->mutex);
        task->response = result;
        task->error    = (result == NULL) ? 1 : 0;
        task->done     = 1;
        /* Sonner la sonnette : "la tâche est prête !" */
        pthread_cond_signal(&task->cond);
        pthread_mutex_unlock(&task->mutex);

        printf("[LLM Thread] Tâche traitée.\n");
    }

    return NULL;
}

/*
 * llm_thread_start() — démarre le thread worker.
 * À appeler une seule fois au démarrage du programme.
 */
int llm_thread_start(struct LLMEngine* engine) {
    if (!engine) return -1;
    if (g_pool)  return 0; /* déjà démarré */

    g_pool = malloc(sizeof(LLMThreadPool));
    if (!g_pool) return -1;

    g_pool->engine  = engine;
    g_pool->head    = 0;
    g_pool->tail    = 0;
    g_pool->count   = 0;
    g_pool->running = 1;

    pthread_mutex_init(&g_pool->mutex, NULL);
    pthread_cond_init(&g_pool->cond,   NULL);

    /* Créer le thread — il commence à exécuter llm_worker() */
    if (pthread_create(&g_pool->thread, NULL, llm_worker, g_pool) != 0) {
        fprintf(stderr, "[LLM Thread] Erreur création thread\n");
        free(g_pool);
        g_pool = NULL;
        return -1;
    }

    printf("[LLM Thread] Thread démarré.\n");
    return 0;
}

/*
 * llm_thread_submit() — soumet une question au thread.
 * NON BLOQUANT : retourne immédiatement avec un ticket (LLMTask*).
 */
LLMTask* llm_thread_submit(const char* prompt) {
    if (!g_pool || !prompt) return NULL;

    /* Créer la tâche */
    LLMTask* task = malloc(sizeof(LLMTask));
    if (!task) return NULL;

    task->prompt   = strdup(prompt); /* copier le prompt */
    task->response = NULL;
    task->done     = 0;
    task->error    = 0;
    pthread_mutex_init(&task->mutex, NULL);
    pthread_cond_init(&task->cond,   NULL);

    /* Ajouter la tâche dans la file */
    pthread_mutex_lock(&g_pool->mutex);

    if (g_pool->count >= 32) {
        /* File pleine — rejeter la tâche */
        pthread_mutex_unlock(&g_pool->mutex);
        free(task->prompt);
        free(task);
        fprintf(stderr, "[LLM Thread] File pleine\n");
        return NULL;
    }

    g_pool->queue[g_pool->tail] = task;
    g_pool->tail  = (g_pool->tail + 1) % 32;
    g_pool->count++;

    /* Sonner la sonnette : "une tâche est arrivée !" */
    pthread_cond_signal(&g_pool->cond);
    pthread_mutex_unlock(&g_pool->mutex);

    return task; /* retourner le ticket immédiatement */
}

/*
 * llm_task_is_done() — vérifie si une tâche est terminée.
 * NON BLOQUANT : retourne 1 si terminée, 0 sinon.
 */
int llm_task_is_done(LLMTask* task) {
    if (!task) return 0;
    pthread_mutex_lock(&task->mutex);
    int done = task->done;
    pthread_mutex_unlock(&task->mutex);
    return done;
}

/*
 * llm_task_wait() — attend la fin d'une tâche et retourne la réponse.
 * BLOQUANT : attend jusqu'à ce que le thread ait fini.
 */
char* llm_task_wait(LLMTask* task) {
    if (!task) return NULL;
    pthread_mutex_lock(&task->mutex);
    /* Attendre la sonnette "tâche prête" */
    while (!task->done)
        pthread_cond_wait(&task->cond, &task->mutex);
    char* response = task->response;
    pthread_mutex_unlock(&task->mutex);
    return response;
}

/*
 * llm_task_free() — libère une tâche terminée.
 */
void llm_task_free(LLMTask* task) {
    if (!task) return;
    free(task->prompt);
    free(task->response);
    pthread_mutex_destroy(&task->mutex);
    pthread_cond_destroy(&task->cond);
    free(task);
}

/*
 * llm_thread_stop() — arrête le thread proprement.
 * À appeler avant de quitter le programme.
 */
void llm_thread_stop(void) {
    if (!g_pool) return;

    pthread_mutex_lock(&g_pool->mutex);
    g_pool->running = 0;
    pthread_cond_signal(&g_pool->cond); /* réveiller le thread */
    pthread_mutex_unlock(&g_pool->mutex);

    pthread_join(g_pool->thread, NULL); /* attendre sa fin */
    pthread_mutex_destroy(&g_pool->mutex);
    pthread_cond_destroy(&g_pool->cond);
    free(g_pool);
    g_pool = NULL;
    printf("[LLM Thread] Thread arrêté.\n");
}
