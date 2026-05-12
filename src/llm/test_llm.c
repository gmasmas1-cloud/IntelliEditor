#include <stdio.h>

struct LLMEngine;
struct LLMEngine* llm_init(const char* model_path, int n_threads, int n_ctx);
void llm_free(struct LLMEngine* engine);

int main() {
    printf("--- Test DEV-C : chargement LLM ---\n");

    struct LLMEngine* engine = llm_init(
        "data/models/qwen2.5-3b-instruct-q4_k_m.gguf",
        4, 2048
    );

    if (!engine) {
        printf("ECHEC : modèle non chargé.\n");
        return 1;
    }

    printf("SUCCES : moteur LLM opérationnel.\n");
    llm_free(engine);
    return 0;
}
