#include "ui_manager.h"
#define ID_TOOLBAR        200

HWND UI_InitToolbar(HWND hwndParent) {
    HINSTANCE hInst = (HINSTANCE)GetWindowLongPtr(hwndParent, GWLP_HINSTANCE);

    // Conteneur de base pour la barre d'outils
    HWND hToolbar = CreateWindowEx(
        0, "STATIC", "", 
        WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 
        0, 0, 0, 0, hwndParent, (HMENU)200, hInst, NULL
    );

    // Injection des boutons de fichiers et styles sous forme de contrôles boutons natifs
    CreateWindow("BUTTON", "Nouveau", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 5, 5, 70, 25, hToolbar, (HMENU)IDM_NOUVEAU, hInst, NULL);
    CreateWindow("BUTTON", "Ouvrir", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 80, 5, 70, 25, hToolbar, (HMENU)IDM_OUVRIR, hInst, NULL);
    CreateWindow("BUTTON", "Sauv.", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 155, 5, 70, 25, hToolbar, (HMENU)IDM_SAUVEGARDER, hInst, NULL);

    // Séparateur visuel
    CreateWindow("STATIC", "|", WS_CHILD | WS_VISIBLE | SS_CENTER, 230, 8, 10, 20, hToolbar, (HMENU)0, hInst, NULL);

    // Boutons de style de texte
    CreateWindow("BUTTON", "G", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 245, 5, 30, 25, hToolbar, (HMENU)IDB_STYLE_GRAS, hInst, NULL);
    CreateWindow("BUTTON", "I", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 280, 5, 30, 25, hToolbar, (HMENU)IDB_STYLE_ITALIQUE, hInst, NULL);
    CreateWindow("BUTTON", "S", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 315, 5, 30, 25, hToolbar, (HMENU)IDB_STYLE_SOULIGNE, hInst, NULL);

    CreateWindow("STATIC", "|", WS_CHILD | WS_VISIBLE | SS_CENTER, 350, 8, 10, 20, hToolbar, (HMENU)0, hInst, NULL);

    // Boutons d'appel aux moteurs IA (Moteur NLP / LLM)
    CreateWindow("BUTTON", "Corriger texte", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 365, 5, 110, 25, hToolbar, (HMENU)IDB_TOOLBAR_CORRIGER, hInst, NULL);
    CreateWindow("BUTTON", "Reformuler", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 480, 5, 90, 25, hToolbar, (HMENU)IDB_TOOLBAR_REF, hInst, NULL);

    return hToolbar;
}