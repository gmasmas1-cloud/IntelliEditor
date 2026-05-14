#include <stdio.h>
#include <stdlib.h>

typedef struct { char** tokens; int count; } TokenList;
TokenList tokenizer_mots(const char* texte);
void      tokenizer_free(TokenList* liste);

struct HunspellEngine;
struct HunspellEngine* hunspell_init(void);
int    hunspell_verifier(struct HunspellEngine* hs, const char* mot);
void   hunspell_free(struct HunspellEngine* hs);

int main() {
    struct HunspellEngine* hs = hunspell_init();

    /* Texte avec vraies fautes d'orthographe */
    const char* texte = "Je mange une pomme. Il a un probleme de langague.";
    TokenList mots = tokenizer_mots(texte);

    printf("Mots trouvés : %d\n", mots.count);
    for (int i = 0; i < mots.count; i++) {
        int ok = hunspell_verifier(hs, mots.tokens[i]);
        printf("  '%s' → %s\n", mots.tokens[i],
               ok ? "✅ correct" : "❌ INCORRECT");
    }

    tokenizer_free(&mots);
    hunspell_free(hs);
    return 0;
}
