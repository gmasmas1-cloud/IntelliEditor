#include "ui_manager.h"
#include <string.h>

static HWND g_hSciWnd = NULL;
static HINSTANCE g_hSciLibrary = NULL;

// ---- Tâche 1.2 : Initialisation et Configuration de Scintilla ----
HWND UI_InitEditor(HWND hwndParent) {
    g_hSciLibrary = LoadLibrary("Scintilla.dll");
    if (!g_hSciLibrary) return NULL;

    HINSTANCE hInst = (HINSTANCE)GetWindowLongPtr(hwndParent, GWLP_HINSTANCE);

    // Création de la zone Scintilla
    g_hSciWnd = CreateWindowEx(
        0, "Scintilla", "", 
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN,
        0, 0, 0, 0, hwndParent, (HMENU)101, hInst, NULL
    );

    if (!g_hSciWnd) return NULL;

    // Configuration de la police par défaut (Consolas, Taille 11)
    SendMessage(g_hSciWnd, 2144, 32, (LPARAM)"Consolas"); // 2144 = SCI_STYLESETFONT, 32 = STYLE_DEFAULT
    // FORCER L'ENCODAGE UTF-8 POUR LE RENDU CORRECT DES LETTRES ET ACCENTS
    SendMessage(g_hSciWnd, 2054, 65001, 0); // 2054 = SCI_SETCODEPAGE, 65001 = CP_UTF8                // 2145 = SCI_STYLESETSIZE
    SendMessage(g_hSciWnd, 2050, 0, 0);                   // 2050 = SCI_STYLECLEARALL

    // Configuration de la marge gauche pour la numérotation automatique des lignes
    SendMessage(g_hSciWnd, 2240, 0, 1);                   // 2240 = SCI_SETMARGINTYPEN (1 = SC_MARGIN_NUMBER)
    SendMessage(g_hSciWnd, 2242, 0, 45);                  // 2242 = SCI_SETMARGINWIDTHN (45 pixels)

    // Configuration graphique du soulignement d'erreur NLP
    SendMessage(g_hSciWnd, 2122, 0, 0);                   // 2122 = SCI_INDICSETSTYLE (0 = INDIC_SQUIGGLE, ligne ondulée)
    SendMessage(g_hSciWnd, 2123, 0, RGB(255, 0, 0));       // 2123 = SCI_INDICSETFORE (Rouge)

    return g_hSciWnd;
}

// ---- Tâche 3.1 : Traçage dynamique du soulignement rouge ondulé (NLP) ----
void UI_HighlightNLPError(int position, int length) {
    if (!g_hSciWnd) return;
    SendMessage(g_hSciWnd, 2140, 0, 0);             // 2140 = SCI_SETINDICATORCURRENT (Indicateur 0)
    SendMessage(g_hSciWnd, 2500, position, length); // 2500 = SCI_INDICATORFILLRANGE
}

void UI_ClearNLPErrors(void) {
    if (!g_hSciWnd) return;
    int docLength = (int)SendMessage(g_hSciWnd, 2006, 0, 0); // 2006 = SCI_GETLENGTH
    SendMessage(g_hSciWnd, 2140, 0, 0);
    SendMessage(g_hSciWnd, 2501, 0, docLength);     // 2501 = SCI_INDICATORCLEARRANGE
}

// ---- Tâche 3.2 : Basculement Thématique Dynamique (Clair / Sombre) ----
void UI_ApplyTheme(int darkTheme) {
    if (!g_hSciWnd) return;

    COLORREF bgColor = darkTheme ? RGB(30, 30, 30) : RGB(255, 255, 255);
    COLORREF fgColor = darkTheme ? RGB(220, 220, 220) : RGB(0, 0, 0);
    COLORREF caretLineColor = darkTheme ? RGB(45, 45, 45) : RGB(240, 240, 240);

    // Application globale sur le style principal (32)
    SendMessage(g_hSciWnd, 2142, 32, fgColor); // 2142 = SCI_STYLESETFORE
    SendMessage(g_hSciWnd, 2143, 32, bgColor); // 2143 = SCI_STYLESETBACK
    SendMessage(g_hSciWnd, 2050, 0, 0);        // 2050 = SCI_STYLECLEARALL

    // Surlignage de la ligne active (curseur)
    SendMessage(g_hSciWnd, 2098, 1, 0);               // 2098 = SCI_SETCARETLINEVISIBLE
    SendMessage(g_hSciWnd, 2099, caretLineColor, 0);  // 2099 = SCI_SETCARETLINEBACK

    // Ajustement des couleurs de la marge numérique des lignes (Style 33)
    SendMessage(g_hSciWnd, 2143, 33, darkTheme ? RGB(40, 40, 40) : RGB(245, 245, 245));
    SendMessage(g_hSciWnd, 2142, 33, darkTheme ? RGB(120, 120, 120) : RGB(100, 100, 100));
}

// ---- Accesseurs et Utilitaires Communs ----
HWND UI_GetEditorHandle(void) { 
    return g_hSciWnd; 
}

int UI_AppendText(const char *text) {
    if (g_hSciWnd && text) {
        return (int)SendMessage(g_hSciWnd, 2001, strlen(text), (LPARAM)text); // 2001 = SCI_ADDTEXT
    }
    return 0;
}

void UI_DestroyEditor(void) {
    if (g_hSciLibrary) { 
        FreeLibrary(g_hSciLibrary); 
        g_hSciLibrary = NULL; 
    }
}