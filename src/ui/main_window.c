#include <windows.h>

// La Window Procedure (WndProc) gère les messages envoyés à la fenêtre
LRESULT CALLBACK WindowProcess(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProc(hwnd, msg, wp, lp);
    }
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR args, int ncmd) {
    const char CLASS_NAME[] = "IntelliEditorWindowClass";

    WNDCLASS wc = {0};
    wc.lpfnWndProc   = WindowProcess;
    wc.hInstance     = hInst;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    if (!RegisterClass(&wc)) {
        return -1;
    }

    // Création de la fenêtre avec le titre spécifié pour le Jour 1 
    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        "IntelliEditor v0.1", // Titre requis pour le lancement
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL, NULL, hInst, NULL
    );

    if (hwnd == NULL) {
        return -1;
    }

    // Boucle de messages standard
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}