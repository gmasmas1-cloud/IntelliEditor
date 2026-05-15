cmake_minimum_required(VERSION 3.10)
project(IntelliEditor)

# Standard C11 requis pour le projet [cite: 42]
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Définition des dossiers
include_directories(include)
include_directories(lib)


# Recherche des fichiers sources dans /src [cite: 13]
#file(GLOB SOURCES "src/*.c")

# Création de l'exécutable dans le dossier /bin [cite: 13]
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#add_executable(IntelliEditor ${SOURCES})

# Note : Les bibliothèques (Hunspell, Scintilla, cJSON) seront liées ici en Phase 2 [cite: 345, 346]
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

