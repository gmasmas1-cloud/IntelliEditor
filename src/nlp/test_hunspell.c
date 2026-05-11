#include <stdio.h>
#include <stdlib.h>

struct HunspellEngine;
struct HunspellEngine* hunspell_init(void);
int    hunspell_verifier(struct HunspellEngine* hs, const char* mot);
char** hunspell_suggestions(struct HunspellEngine* hs,
                             const char* mot, int* nb_sugg);
void   hunspell_liberer_suggestions(struct HunspellEngine* hs,
                                     char** liste, int nb_sugg);
void   hunspell_free(struct HunspellEngine* hs);

int main() {
    printf("--- Test DEV-C : Hunspell ---\n\n");

    struct HunspellEngine* hs = hunspell_init();
    if (!hs) return 1;

    /* Mots à tester */
    const char* mots[] = {
        "bonjour", "langage", "langague",
        "université", "tset", "mange"
    };
    int n = sizeof(mots) / sizeof(mots[0]);

    for (int i = 0; i < n; i++) {
        int ok = hunspell_verifier(hs, mots[i]);
        printf("'%s' → %s", mots[i], ok ? "✅ correct" : "❌ incorrect");

        if (!ok) {
            int nb = 0;
            char** sugg = hunspell_suggestions(hs, mots[i], &nb);
            if (sugg && nb > 0)
                printf("  (suggestion : %s)", sugg[0]);
            if (sugg)
                hunspell_liberer_suggestions(hs, sugg, nb);
        }
        printf("\n");
    }

    hunspell_free(hs);
    return 0;
}
