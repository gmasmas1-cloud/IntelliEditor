#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <windows.h>

// Identifiants des commandes Menus & Boutons
#define IDM_NOUVEAU       2001
#define IDM_OUVRIR        2002
#define IDM_SAUVEGARDER   2003
#define IDM_QUITTER       2004
#define IDM_COUPER        2005
#define IDM_COPIER        2006
#define IDM_COLLER        2007

#define IDB_TOOLBAR_CORRIGER 3001
#define IDB_TOOLBAR_REF      3002
#define IDB_STYLE_GRAS       3003
#define IDB_STYLE_ITALIQUE   3004
#define IDB_STYLE_SOULIGNE   3005

// Initialisations des sous-composants
HWND UI_InitEditor(HWND hwndParent);
HWND UI_InitToolbar(HWND hwndParent);
HWND UI_InitStatusbar(HWND hwndParent);
HWND UI_InitRulesPanel(HWND hwndParent);

// Redimensionnement et mise à jour
void UI_LayoutComponents(HWND hwndParent);
void UI_UpdateStatus(const char* txtMots, const char* txtLigneCol, const char* txtLLM);
void UI_AddRuleToPanel(const char* id, const char* desc, int status);

// Gestionnaires Scintilla & Thème (Tâche 3.1, 3.2)
void UI_ApplyTheme(int darkTheme);
void UI_HighlightNLPError(int position, int length);
void UI_ClearNLPErrors(void);

// Boîtes de dialogue (Tâche 2.4)
void UI_DialogOpenFile(HWND hwndOwner);
void UI_DialogSaveFile(HWND hwndOwner);

// Accesseurs Globaux
HWND UI_GetEditorHandle(void);
void UI_DestroyEditor(void);

#endif // UI_MANAGER_H