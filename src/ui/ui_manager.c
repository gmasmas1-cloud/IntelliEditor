#include "ui_manager.h"
#include <shellapi.h> // Requis pour WM_DROPFILES

// Déclaration de la procédure de fenêtre
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

// ---- Point d'entrée principal de l'application Windows ----
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR cmd, int show) {
    WNDCLASSEX wc = { 
        sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, WndProc, 0, 0, 
        hInst, NULL, LoadCursor(NULL, IDC_ARROW), 
        (HBRUSH)(COLOR_WINDOW + 1), NULL, "IntelliEditorClass", NULL 
    };
    
    if (!RegisterClassEx(&wc)) return 0;

    // Création de la fenêtre maîtresse (Note le titre corrigé ici en dur pour tester !)
    HWND hwnd = CreateWindowEx(
        WS_EX_ACCEPTFILES, "IntelliEditorClass", "IntelliEditor v1.0", 
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1024, 700, 
        NULL, NULL, hInst, NULL
    );

    if (!hwnd) return 0;

    ShowWindow(hwnd, show);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

// ---- Gestionnaire de messages et d'actions ----
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
        case WM_CREATE: {
            HMENU hMenu = CreateMenu();
            HMENU hMenuFichier = CreatePopupMenu();
            
            AppendMenuW(hMenuFichier, MF_STRING, IDM_NOUVEAU, L"Nouveau\tCtrl+N");
            AppendMenuW(hMenuFichier, MF_STRING, IDM_OUVRIR, L"Ouvrir...\tCtrl+O");
            AppendMenuW(hMenuFichier, MF_STRING, IDM_SAUVEGARDER, L"Enregistrer sous...\tCtrl+S");
            AppendMenuW(hMenuFichier, MF_SEPARATOR, 0, NULL);
            AppendMenuW(hMenuFichier, MF_STRING, IDM_QUITTER, L"Quitter");
            AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hMenuFichier, L"Fichier");

            HMENU hMenuEdition = CreatePopupMenu();
            AppendMenuW(hMenuEdition, MF_STRING, IDM_COUPER, L"Couper\tCtrl+X");
            AppendMenuW(hMenuEdition, MF_STRING, IDM_COPIER, L"Copier\tCtrl+C");
            AppendMenuW(hMenuEdition, MF_STRING, IDM_COLLER, L"Coller\tCtrl+V");
            AppendMenuW(hMenu, MF_POPUP, (UINT_PTR)hMenuEdition, L"Édition");
            
            SetMenu(hwnd, hMenu);

            UI_InitToolbar(hwnd);
            UI_InitEditor(hwnd);
            UI_InitRulesPanel(hwnd);
            UI_InitStatusbar(hwnd);
            
            UI_ApplyTheme(0); 
            break;
        }

        case WM_SIZE: {
            UI_LayoutComponents(hwnd);
            break;
        }

        case WM_COMMAND: {
            HWND hEditor = UI_GetEditorHandle();
            switch (LOWORD(wp)) {
                case IDM_NOUVEAU:
                    SendMessage(hEditor, 2181, 0, (LPARAM)""); 
                    break;
                case IDM_OUVRIR: 
                    UI_DialogOpenFile(hwnd); 
                    break;
                case IDM_SAUVEGARDER: 
                    UI_DialogSaveFile(hwnd); 
                    break;
                case IDM_QUITTER: 
                    DestroyWindow(hwnd); 
                    break;
                case IDM_COUPER:
                    SendMessage(hEditor, 2177, 0, 0); 
                    break;
                case IDM_COPIER:
                    SendMessage(hEditor, 2178, 0, 0); 
                    break;
                case IDM_COLLER:
                    SendMessage(hEditor, 2179, 0, 0); 
                    break;
            }
            break;
        }

        case WM_DROPFILES: {
            HDROP hDrop = (HDROP)wp;
            char szFilePath[MAX_PATH];
            if (DragQueryFile(hDrop, 0, szFilePath, MAX_PATH)) {
                if (strstr(szFilePath, ".json") != NULL) {
                    MessageBox(hwnd, szFilePath, "Fichier de règles détecté", MB_OK | MB_ICONINFORMATION);
                } else {
                    HWND hEditor = UI_GetEditorHandle();
                    HANDLE hFile = CreateFile(szFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
                    if (hFile != INVALID_HANDLE_VALUE) {
                        DWORD dwSize = GetFileSize(hFile, NULL);
                        char* buf = (char*)GlobalAlloc(GPTR, dwSize + 1);
                        DWORD dwRead;
                        if (ReadFile(hFile, buf, dwSize, &dwRead, NULL)) {
                            buf[dwSize] = '\0';
                            SendMessage(hEditor, 2181, 0, (LPARAM)""); 
                            SendMessage(hEditor, 2001, dwSize, (LPARAM)buf); 
                        }
                        GlobalFree(buf);
                        CloseHandle(hFile);
                    }
                }
            }
            DragFinish(hDrop);
            break;
        }

        case WM_DESTROY:
            UI_DestroyEditor();
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, msg, wp, lp);
    }
    return 0;
}

// ---- Gestion de la géométrie des composants ----
void UI_LayoutComponents(HWND hwndParent) {
    RECT rcClient;
    GetClientRect(hwndParent, &rcClient);

    int width = rcClient.right - rcClient.left;
    int height = rcClient.bottom - rcClient.top;

    int toolbarHeight = 35;
    int statusbarHeight = 25;
    int currentHeight = height - toolbarHeight - statusbarHeight;
    int rulesPanelWidth = 260; 
    int editorWidth = width - rulesPanelWidth;

    MoveWindow(GetDlgItem(hwndParent, 200), 0, 0, width, toolbarHeight, TRUE); 
    MoveWindow(UI_GetEditorHandle(), 0, toolbarHeight, editorWidth, currentHeight, TRUE); 
    MoveWindow(GetDlgItem(hwndParent, 400), editorWidth, toolbarHeight, rulesPanelWidth, currentHeight, TRUE); 
    MoveWindow(GetDlgItem(hwndParent, 300), 0, height - statusbarHeight, width, statusbarHeight, TRUE); 
}