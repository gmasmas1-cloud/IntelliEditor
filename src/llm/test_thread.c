#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  /* pour sleep() */

struct LLMEngine;
struct LLMEngine* llm_init(const char* path, int threads, int ctx);
void llm_free(struct LLMEngine* engine);

typedef struct {
    char* prompt;
    char* response;
    int   done;
    int   error;
    void* mutex;
    void* cond;
} LLMTask;

int      llm_thread_start(struct LLMEngine* engine);
LLMTask* llm_thread_submit(const char* prompt);
int      llm_task_is_done(LLMTask* task);
char*    llm_task_wait(LLMTask* task);
void     llm_task_free(LLMTask* task);
void     llm_thread_stop(void);

int main() {
    printf("--- Test DEV-C : Thread LLM ---\n\n");

    /* 1. Charger le modèle */
    struct LLMEngine* engine = llm_init(
        "data/models/qwen2.5-3b-instruct-q4_k_m.gguf",
        4, 2048
    );
    if (!engine) return 1;

    /* 2. Démarrer le thread */
    llm_thread_start(engine);

    /* 3. Soumettre une tâche — NON BLOQUANT */
    const char* prompt =
        "<|im_start|>system\n"
        "Tu es un correcteur de français. "
        "Réponds uniquement avec la phrase corrigée.<|im_end|>\n"
        "<|im_start|>user\n"
        "Corrige : Je vais au école.<|im_end|>\n"
        "<|im_start|>assistant\n";

    printf("Soumission de la tâche...\n");
    LLMTask* task = llm_thread_submit(prompt);
    printf("Tâche soumise ! L'interface peut continuer...\n\n");

    /* 4. Simuler l'interface qui continue à travailler */
    printf("Pendant que le LLM réfléchit :\n");
    for (int i = 0; i < 5; i++) {
        printf("  → L'interface est réactive... (%d)\n", i + 1);
        sleep(1);

        /* Vérifier si c'est terminé */
        if (llm_task_is_done(task)) {
            printf("  → Résultat prêt !\n");
            break;
        }
    }

    /* 5. Attendre et récupérer le résultat */
    printf("\nRécupération du résultat...\n");
    char* reponse = llm_task_wait(task);
    printf("Réponse : %s\n", reponse ? reponse : "(vide)");

    /* 6. Libérer */
    llm_task_free(task);
    llm_thread_stop();
    llm_free(engine);

    return 0;
}
