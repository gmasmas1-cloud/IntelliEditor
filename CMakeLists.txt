cmake_minimum_required(VERSION 3.10)
project(IntelliEditor)

# Standard C11 requis pour le projet [cite: 3, 40]
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Inclusion du dossier des headers pour que #include "mon_fichier.h" fonctionne [cite: 333]
include_directories(include)

# Recherche récursive de TOUS les fichiers .c dans src et ses sous-dossiers (comme src/ui) [cite: 333]
file(GLOB_RECURSE SOURCES "src/*.c")

# Définition du dossier de sortie pour l'exécutable [cite: 333]
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# Ajout de l'exécutable avec le flag WIN32 [cite: 106, 307]
# Le flag WIN32 indique qu'il s'agit d'une application graphique (utilise WinMain au lieu de main)
add_executable(IntelliEditor
    src/ui/ui_manager.c
    src/ui/scintilla_wrapper.c
    src/ui/toolbar.c
    src/ui/statusbar.c
    src/ui/rules_panel.c
    src/ui/dialogs.c
)


# Note : Les bibliothèques (Hunspell, Scintilla, cJSON) seront liées ici en Phase 2 [cite: 190, 301]