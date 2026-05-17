#include "ui_manager.h"
#include <commctrl.h> // Nécessaire pour le contrôle natif STATUSCLASSNAME

static HWND g_hStatusWnd = NULL;

HWND UI_InitStatusbar(HWND hwndParent) {
    HINSTANCE hInst = (HINSTANCE)GetWindowLongPtr(hwndParent, GWLP_HINSTANCE);

    g_hStatusWnd = CreateWindowEx(
        0, STATUSCLASSNAME, NULL, 
        WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP, 
        0, 0, 0, 0, hwndParent, (HMENU)300, hInst, NULL
    );

    // Division de la barre d'état en 4 zones distinctes
    int sectionWidths[] = { 180, 340, 480, -1 }; 
    SendMessage(g_hStatusWnd, SB_SETPARTS, 4, (LPARAM)sectionWidths);

    // Définir les textes d'initialisation par défaut
    SendMessage(g_hStatusWnd, SB_SETTEXT, 0, (LPARAM)"Mots : 0");
    SendMessage(g_hStatusWnd, SB_SETTEXT, 1, (LPARAM)"Ligne : 1, Col : 1");
    SendMessage(g_hStatusWnd, SB_SETTEXT, 2, (LPARAM)"Format: UTF-8 (FR)");
    SendMessage(g_hStatusWnd, SB_SETTEXT, 3, (LPARAM)"LLM: Prêt (Mistral 7B)");

    return g_hStatusWnd;
}

// Fonction globale d'actualisation de l'affichage (Liaison avec DEV-A et DEV-C)
void UI_UpdateStatus(const char* txtMots, const char* txtLigneCol, const char* txtLLM) {
    if (!g_hStatusWnd) return;
    if (txtMots) SendMessage(g_hStatusWnd, SB_SETTEXT, 0, (LPARAM)txtMots);
    if (txtLigneCol) SendMessage(g_hStatusWnd, SB_SETTEXT, 1, (LPARAM)txtLigneCol);
    if (txtLLM) SendMessage(g_hStatusWnd, SB_SETTEXT, 3, (LPARAM)txtLLM);
}