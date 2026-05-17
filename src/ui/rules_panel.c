#include "ui_manager.h"

static HWND g_hRulesList = NULL;

HWND UI_InitRulesPanel(HWND hwndParent) {
    HINSTANCE hInst = (HINSTANCE)GetWindowLongPtr(hwndParent, GWLP_HINSTANCE);

    // Conteneur de fond pour la barre latérale
   HWND hPanel = CreateWindowEx(
        0, "STATIC", "", 
        WS_CHILD | WS_VISIBLE | SS_BLACKFRAME, 
        0, 0, 0, 0, hwndParent, (HMENU)400, hInst, NULL
    );

    // Titre indicatif au sommet de la barre latérale
    CreateWindow("STATIC", " MODULE DE CONFORMITÉ", WS_CHILD | WS_VISIBLE | SS_LEFTNOWORDWRAP, 
                 2, 5, 250, 20, hPanel, (HMENU)0, hInst, NULL);

    // Contrôle ListBox natif Windows pour empiler proprement les règles
    g_hRulesList = CreateWindowEx(
        WS_EX_CLIENTEDGE, "LISTBOX", NULL, 
        WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY | LBS_HASSTRINGS, 
        5, 30, 250, 550, hPanel, (HMENU)401, hInst, NULL
    );

    // Insertion d'un jeu de règles de démonstration initial (Liaison future DEV-D)
    UI_AddRuleToPanel("R001", "Introduction obligatoire", 1);  // OK
    UI_AddRuleToPanel("R002", "Pas de première personne", 0);  // Violation
    UI_AddRuleToPanel("R003", "Bibliographie manquante", 2); // Avertissement

    return hPanel;
}

// Fonction d'injection et de mise à jour des règles métier dans l'interface
void UI_AddRuleToPanel(const char* id, const char* desc, int status) {
    if (!g_hRulesList) return;

    char fullRowText[256];
    const char* statusIcon = "??"; // En cours par défaut

    if (status == 1) statusIcon = "[OK]";       // Règle respectée
    else if (status == 0) statusIcon = "[X]";   // Non conforme
    else if (status == 2) statusIcon = "[!]";   // Avertissement

    wsprintf(fullRowText, "%s %s: %s", statusIcon, id, desc);
    SendMessage(g_hRulesList, LB_ADDSTRING, 0, (LPARAM)fullRowText);
}