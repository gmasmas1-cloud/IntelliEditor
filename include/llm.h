#ifndef LLM_H
#define LLM_H

/*
 * llm.h — Interface publique du module LLM (DEV-C)
 *
 * DEV-B inclut ce fichier pour envoyer du texte au LLM
 * et récupérer une correction sans bloquer l'interface.
 */

/* --- Moteur LLM (opaque) --- */
typedef struct LLMEngine LLMEngine;

/* --- Tâche asynchrone (opaque) --- */
typedef struct LLMTask LLMTask;

/* --- Chargement du modèle --- */
LLMEngine* llm_init(const char* model_path, int n_threads, int n_ctx);
void       llm_free(LLMEngine* engine);

/* --- Thread asynchrone --- */
int      llm_thread_start(LLMEngine* engine);
void     llm_thread_stop(void);

/* --- Soumettre une tâche (non bloquant) --- */
LLMTask* llm_thread_submit(const char* prompt);

/* --- Récupérer le résultat --- */
int      llm_task_is_done(LLMTask* task);
char*    llm_task_wait(LLMTask* task);
void     llm_task_free(LLMTask* task);

#endif /* LLM_H */
