#include <windows.h>

// Fonction stub pour satisfaire les exports de Scintilla
HWND WINAPI CreateScintillaWindow(HWND hParent, LPVOID pInfo) {
    // Crée une fenêtre vide qui ressemble à Scintilla
    HWND hwnd = CreateWindow(
        "STATIC", "",
        WS_CHILD | WS_VISIBLE,
        0, 0, 100, 100,
        hParent, NULL,
        GetModuleHandle(NULL), NULL
    );
    return hwnd;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason) {
        case DLL_PROCESS_ATTACH:
            // Enregistre la classe Scintilla pour que CreateWindow fonctionne
            {
                WNDCLASS wc = {0};
                wc.lpfnWndProc = DefWindowProc;
                wc.lpszClassName = "Scintilla";
                wc.hInstance = hinstDLL;
                RegisterClass(&wc);
            }
            break;
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}
