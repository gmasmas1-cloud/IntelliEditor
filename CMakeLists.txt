cmake_minimum_required(VERSION 3.10)
project(IntelliEditor)

# Standard C11 requis pour le projet [cite: 42]
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Inclusion du dossier des headers pour que #include "mon_fichier.h" fonctionne [cite: 333]
include_directories(include)
include_directories(llama.cpp/include)
include_directories(lib)


# Recherche récursive de TOUS les fichiers .c dans src et ses sous-dossiers (comme src/ui) [cite: 333]
file(GLOB_RECURSE SOURCES "src/*.c")
add_subdirectory(llama.cpp)
# Recherche des fichiers sources dans /src [cite: 13]
#file(GLOB SOURCES "src/*.c")

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


# Programme principal (DEV-A)
add_executable(IntelliEditor src/main.c)
target_link_libraries(IntelliEditor llama)
#add_executable(IntelliEditor ${SOURCES})

# Test DEV-C : module LLM
add_executable(test_llm
    src/llm/test_llm.c
    src/llm/llm_interface.c
)
target_link_libraries(test_llm llama)

# Test DEV-C : Hunspell
add_executable(test_hunspell
    src/nlp/test_hunspell.c
    src/nlp/hunspell_wrap.c
)
target_link_libraries(test_hunspell hunspell-1.7)

# Test DEV-C : Thread LLM
add_executable(test_thread
    src/llm/test_thread.c
    src/llm/llm_thread.c
    src/llm/llm_interface.c
)
target_link_libraries(test_thread llama pthread)

# Test DEV-C : NLP complet
add_executable(test_nlp
    src/nlp/test_nlp.c
    src/nlp/nlp_engine.c
    src/nlp/hunspell_wrap.c
    src/nlp/tokenizer.c
)
target_link_libraries(test_nlp hunspell-1.7)

add_executable(test_debug
    src/nlp/test_debug.c
    src/nlp/tokenizer.c
    src/nlp/hunspell_wrap.c
)
target_link_libraries(test_debug hunspell-1.7)

# Note (dev-d) : Les bibliothèques (Hunspell, Scintilla, cJSON) seront liées ici en Phase 2 [cite: 345, 346]
add_executable(test_cjson
    src/test_cjson.c
    lib/cJSON.c       # Fichier source de cJSON
)

# Indique où trouver cJSON.h
target_include_directories(test_cjson PRIVATE ${CMAKE_SOURCE_DIR}/lib)

# Exécutable pour tester le parseur de règles
add_executable(test_rule_parser
    src/test_rule_parser.c
    src/rules/rule_parser.c
    lib/cJSON.c
)

# Chemins pour les includes
target_include_directories(test_rule_parser PRIVATE
    ${CMAKE_SOURCE_DIR}/lib
    ${CMAKE_SOURCE_DIR}/include
)

# Exécutable pour tester le moteur de règles
add_executable(test_rule_checker
    src/test_rule_checker.c
    src/rules/rule_parser.c
    src/rules/rule_checker.c
    src/rules/rule_engine.c
    lib/cJSON.c
)

# Chemins pour les includes
target_include_directories(test_rule_checker PRIVATE
    ${CMAKE_SOURCE_DIR}/lib
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/rules
)

add_executable(test_demo
    src/test_demo.c
    src/rules/rule_parser.c
    src/rules/rule_checker.c
    src/rules/rule_engine.c
    lib/cJSON.c
)

target_include_directories(test_demo PRIVATE
    ${CMAKE_SOURCE_DIR}/lib
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/rules
)

