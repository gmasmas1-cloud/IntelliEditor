/**
 * @file ui_manager.h
 * @brief UI Manager - Module de gestion de l'interface utilisateur (DEV-B)
 *
 * Ce module expose les fonctions publiques pour initialiser et gérer
 * l'éditeur Scintilla et tous les composants de l'interface.
 *
 * Périmètre DEV-B : src/ui/
 */

#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <windows.h>

/* Redéfinition des constantes Scintilla nécessaires */
#define SCI_GETLENGTH 2006
#define SCI_GETTEXT 2182
#define SCI_ADDTEXT 2001
#define SCI_CLEARALL 2004
#define SCI_SETZOOM 2373
#define SCI_SETCARETLINEVISIBLE 2098
#define SCI_SETCARETLINEBACK 2099
#define SCI_STYLESETFONT 2056
#define SCI_STYLESETSIZE 2055
#define SCI_STYLECLEARALL 2050
#define STYLE_DEFAULT 32

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * IDs des commandes de menu et des composants
 * ============================================================================ */
#define IDM_NOUVEAU       1001
#define IDM_OUVRIR        1002
#define IDM_SAUVEGARDER   1003
#define IDM_QUITTER       1004
#define IDM_COUPER        1005
#define IDM_COPIER        1006
#define IDM_COLLER        1007

/* IDs des boutons de la barre d'outils */
#define IDB_TOOLBAR_CORRIGER 3001
#define IDB_TOOLBAR_REF      3002
#define IDB_STYLE_GRAS       3003
#define IDB_STYLE_ITALIQUE   3004
#define IDB_STYLE_SOULIGNE   3005

/* IDs des contrôles enfants */
#define ID_TOOLBAR        200
#define ID_STATUSBAR      300
#define ID_RULES_PANEL    400
#define ID_EDITOR         100

/* ============================================================================
 * Initialisation et gestion de l'éditeur Scintilla
 * ============================================================================ */

/**
 * @brief Initialise l'éditeur Scintilla dans une fenêtre parente.
 * @param hwndParent Handle de la fenêtre parente
 * @return HWND Handle de la fenêtre Scintilla, ou NULL en cas d'erreur
 */
HWND UI_InitEditor(HWND hwndParent);

/**
 * @brief Retourne le handle de la fenêtre Scintilla.
 */
HWND UI_GetEditorHandle(void);

/**
 * @brief Ajoute du texte à la fin de l'éditeur.
 * @param text Texte à ajouter (terminé par \\0)
 * @return Nombre de caractères ajoutés
 */
int UI_AppendText(const char *text);

/**
 * @brief Nettoie et détruit les ressources de l'éditeur.
 */
void UI_DestroyEditor(void);

/* ============================================================================
 * Initialisation des composants de l'interface
 * ============================================================================ */

/**
 * @brief Initialise la barre d'outils.
 */
void UI_InitToolbar(HWND hwndParent);

/**
 * @brief Initialise le panneau des règles (barre latérale).
 */
void UI_InitRulesPanel(HWND hwndParent);

/**
 * @brief Initialise la barre d'état.
 */
void UI_InitStatusbar(HWND hwndParent);

/* ============================================================================
 * Layout et thème
 * ============================================================================ */

/**
 * @brief Repositionne tous les composants selon la taille de la fenêtre.
 * @param hwndParent Handle de la fenêtre principale
 */
void UI_LayoutComponents(HWND hwndParent);

/**
 * @brief Applique un thème à l'interface.
 * @param themeId 0 = thème clair, 1 = thème sombre
 */
void UI_ApplyTheme(int themeId);

/* ============================================================================
 * Boîtes de dialogue
 * ============================================================================ */

/**
 * @brief Ouvre une boîte de dialogue pour ouvrir un fichier.
 */
void UI_DialogOpenFile(HWND hwndParent);

/**
 * @brief Ouvre une boîte de dialogue pour enregistrer un fichier.
 */
void UI_DialogSaveFile(HWND hwndParent);

/* ============================================================================
 * Procédure fenêtre (pour usage interne et dans main.c)
 * ============================================================================ */

/**
 * @brief Procédure fenêtre principale de l'application.
 */
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

#ifdef __cplusplus
}
#endif

#endif /* UI_MANAGER_H */
