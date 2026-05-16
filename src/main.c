#include <stdio.h>
#include <stdlib.h>
#include "buffer.h"
#include "worker.h"
#include "config.h"

// Note : On inclut les headers des collègues ici
// #include "interface.h" (Le code de Mbayo)
// #include "ai_engine.h" (Le code de David)

int main(int argc, char* argv[]) {
    printf("--- Lancement de %s ---\n", APP_NAME);

    // 1. Initialisation du Gap Buffer (Ton noyau)
    GapBuffer* mainBuffer = gb_create(DEFAULT_BUFFER_SIZE);
    if (!mainBuffer) return EXIT_FAILURE;

    // 2. Chargement automatique si l'utilisateur glisse un fichier sur l'exe
    if (argc > 1) {
        if (gb_load_from_file(mainBuffer, argv[1]) == 0) {
            printf("Fichier %s charge avec succes.\n", argv[1]);
        }
    }

    // 3. Point d'entrée de l'interface (Code de Mbayo)
    // C'est ici que la boucle Win32 prend le relais
    // Mbayo_InitUI(mainBuffer); 

    printf("Fermeture de l'editeur...\n");

    // 4. Nettoyage final (Crucial pour l'oral !)
    gb_free(mainBuffer);
    
    return EXIT_SUCCESS;
}