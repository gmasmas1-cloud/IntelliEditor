#include <stdio.h>
#include <stdlib.h>

struct LLMEngine;
struct LLMEngine* llm_init(const char* model_path, int n_threads, int n_ctx);
char* llm_ask(struct LLMEngine* engine, const char* prompt);
void llm_nettoyer_reponse(char* response);
void llm_free(struct LLMEngine* engine);

int main() {
    printf("--- Test DEV-C : LLM complet ---\n\n");

    struct LLMEngine* engine = llm_init(
        "data/models/qwen2.5-3b-instruct-q4_k_m.gguf",
        4, 2048
    );
    if (!engine) {
        printf("ECHEC : modèle non chargé.\n");
        return 1;
    }

    const char* prompt =
        "<|im_start|>system\n"
        "Tu es un correcteur de français. "
        "Réponds uniquement avec la phrase corrigée.<|im_end|>\n"
        "<|im_start|>user\n"
        "Corrige : Les enfant mange des pomme.<|im_end|>\n"
        "<|im_start|>assistant\n";

    printf("Prompt : Les enfant mange des pomme.\n");
    printf("Réponse : ");
    fflush(stdout);

    char* reponse = llm_ask(engine, prompt);
    llm_nettoyer_reponse(reponse);  /* supprime <|im_end|> */
    printf("%s\n", reponse ? reponse : "(vide)");
    free(reponse);

    llm_free(engine);
    return 0;
}
