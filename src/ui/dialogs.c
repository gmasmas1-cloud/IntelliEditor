#include "ui_manager.h"
#include <commdlg.h>

void UI_DialogOpenFile(HWND hwndOwner) {
    OPENFILENAME ofn;
    char szFile[MAX_PATH] = "";

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwndOwner;
    // Filtres : Fichiers textes ou configurations de règles JSON
    ofn.lpstrFilter = "Fichiers Texte (*.txt)\0*.txt\0Fichiers de règles (*.json)\0*.json\0Tous (*.*)\0*.*\0";
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

    if (GetOpenFileName(&ofn)) {
        HWND hEditor = UI_GetEditorHandle();
        HANDLE hFile = CreateFile(szFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile != INVALID_HANDLE_VALUE) {
            DWORD dwSize = GetFileSize(hFile, NULL);
            char* buf = (char*)GlobalAlloc(GPTR, dwSize + 1);
            DWORD dwRead;
            if (ReadFile(hFile, buf, dwSize, &dwRead, NULL)) {
                buf[dwSize] = '\0';
                
                SendMessage(hEditor, 2181, 0, (LPARAM)"");       // 2181 = SCI_SETTEXT (vide l'éditeur)
                SendMessage(hEditor, 2001, dwSize, (LPARAM)buf); // 2001 = SCI_ADDTEXT (insère le texte lu)
            }
            GlobalFree(buf);
            CloseHandle(hFile);
        }
    }
}

void UI_DialogSaveFile(HWND hwndOwner) {
    OPENFILENAME ofn;
    char szFile[MAX_PATH] = "";

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwndOwner;
    ofn.lpstrFilter = "Fichiers Texte (*.txt)\0*.txt\0";
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT; // <-- CORRIGÉ ICI !

    if (GetSaveFileName(&ofn)) {
        HWND hEditor = UI_GetEditorHandle();
        
        int len = (int)SendMessage(hEditor, 2006, 0, 0); // 2006 = SCI_GETLENGTH
        char* buf = (char*)GlobalAlloc(GPTR, len + 1);
        
        SendMessage(hEditor, 2182, len + 1, (LPARAM)buf); // 2182 = SCI_GETTEXT
        
        HANDLE hFile = CreateFile(szFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile != INVALID_HANDLE_VALUE) {
            DWORD dwWritten;
            WriteFile(hFile, buf, len, &dwWritten, NULL);
            CloseHandle(hFile);
            MessageBox(hwndOwner, "Fichier sauvegardé avec succès !", "Succès", MB_OK | MB_ICONINFORMATION);
        }
        GlobalFree(buf);
    }
}