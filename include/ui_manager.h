/**
 * @file ui_manager.h
 * @brief UI Manager - Module de gestion de l'interface utilisateur (DEV-B)
 * 
 * Ce module expose uniquement les fonctions publiques pour initialiser
 * et gérer l'éditeur Scintilla. Masangu (DEV-A) et David (DEV-C)
 * peuvent l'utiliser via ces interfaces publiques.
 * 
 * Périmètre DEV-B : src/ui/
 */

#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialise l'éditeur Scintilla dans une fenêtre parente
 * 
 * @param hwndParent Handle de la fenêtre parente (fournie par DEV-A)
 * @return HWND Handle de la fenêtre Scintilla créée, ou NULL en cas d'erreur
 * 
 * Utilisation recommandée :
 *   - Appelée depuis main() après la création de la fenêtre principale
 *   - Charge automatiquement Scintilla.dll
 *   - Crée la fenêtre enfant avec les styles appropriés
 */
HWND UI_InitEditor(HWND hwndParent);

/**
 * @brief Redimensionne la fenêtre de l'éditeur
 * 
 * @param width Nouvelle largeur en pixels
 * @param height Nouvelle hauteur en pixels
 * 
 * Utilisation recommandée :
 *   - Appelée lors de WM_SIZE dans la fenêtre principale
 *   - Maintient l'éditeur en synchronisation avec la taille de la fenêtre
 */
void UI_ResizeEditor(int width, int height);

/**
 * @brief Obtient le handle de la fenêtre Scintilla
 * 
 * @return HWND Handle de l'éditeur (peut être NULL si non initialisé)
 * 
 * Utilisation recommandée :
 *   - DEV-C peut l'utiliser pour envoyer du texte NLP à l'éditeur
 *   - Permet de communiquer avec Scintilla via SendMessage()
 */
HWND UI_GetEditorHandle(void);

/**
 * @brief Ajoute du texte à l'éditeur
 * 
 * @param text Texte à ajouter (terminé par \0)
 * @return int Nombre de caractères ajoutés
 * 
 * Utilisation recommandée :
 *   - DEV-C appelle cette fonction pour afficher les résultats NLP
 *   - Thread-safe grâce à l'encapsulation
 */
int UI_AppendText(const char *text);

/**
 * @brief Nettoie et détruit la fenêtre de l'éditeur
 * 
 * Utilisation recommandée :
 *   - Appelée lors de la fermeture de l'application (WM_DESTROY)
 *   - Libère les ressources Scintilla
 */
void UI_DestroyEditor(void);


/* ============================================================================
 * UTILITAIRES POUR DEV-A (Masangu) - Intégration avec la fenêtre principale
 * ============================================================================ */

/**
 * @brief Obtient la procédure fenêtre pour la fenêtre principale
 * 
 * @return WNDPROC Pointeur vers la procédure fenêtre UI_WndProc
 * 
 * Utilisation recommandée (dans main.c de DEV-A):
 *   WNDCLASSA wc = {0};
 *   wc.lpfnWndProc = UI_GetWndProc();  // Au lieu de WndProc personnalisé
 *   RegisterClassA(&wc);
 * 
 * Cette procédure gère automatiquement :
 *   - WM_SIZE: redimensionne l'éditeur
 *   - WM_DESTROY: nettoie les ressources
 */
WNDPROC UI_GetWndProc(void);

/**
 * @brief Procédure fenêtre pour la fenêtre principale
 * 
 * @param hwnd Handle de la fenêtre
 * @param msg Identificateur du message
 * @param wp Paramètre wParam du message
 * @param lp Paramètre lParam du message
 * @return LRESULT Résultat du traitement du message
 * 
 * Gère automatiquement les messages liés à l'interface utilisateur.
 * Peut être utilisée directement dans RegisterClass().
 */
LRESULT CALLBACK UI_WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

#ifdef __cplusplus
}
#endif

#endif // UI_MANAGER_H
