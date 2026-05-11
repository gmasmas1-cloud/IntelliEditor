#include "ui_manager.h"
#include "Scintilla.h" // Assure-toi que ce fichier est dans /include

static HWND g_hSciWnd = NULL;
static HINSTANCE g_hSciLibrary = NULL;

HWND UI_InitEditor(HWND hwndParent) {
    // 1. Charger la DLL
    g_hSciLibrary = LoadLibrary("Scintilla.dll");
    if (!g_hSciLibrary) return NULL;

    // 2. Récupérer l'instance
    HINSTANCE hInst = (HINSTANCE)GetWindowLongPtr(hwndParent, GWLP_HINSTANCE);

    // 3. Créer la fenêtre Scintilla
    g_hSciWnd = CreateWindowEx(
        0, "Scintilla", "", 
        WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN,
        0, 0, 0, 0, // Taille gérée par UI_ResizeEditor plus tard
        hwndParent, 
        (HMENU)100, // ID de la fenêtre enfant
        hInst, 
        NULL
    );

    return g_hSciWnd;
}

void UI_ResizeEditor(int width, int height) {
    if (g_hSciWnd) {
        MoveWindow(g_hSciWnd, 0, 0, width, height, TRUE);
    }
}

HWND UI_GetEditorHandle(void) {
    return g_hSciWnd;
}

int UI_AppendText(const char *text) {
    if (g_hSciWnd && text) {
        // SCI_APPENDTEXT est une commande spécifique à Scintilla
        return (int)SendMessage(g_hSciWnd, 2001, strlen(text), (LPARAM)text);
    }
    return 0;
}

void UI_DestroyEditor(void) {
    if (g_hSciLibrary) {
        FreeLibrary(g_hSciLibrary);
        g_hSciLibrary = NULL;
    }
}