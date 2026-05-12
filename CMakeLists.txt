cmake_minimum_required(VERSION 3.10)
project(IntelliEditor)

# Standard C11 requis pour le projet [cite: 42]
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Définition des dossiers
include_directories(include)

# Recherche des fichiers sources dans /src [cite: 13]
file(GLOB SOURCES "src/*.c")

# Création de l'exécutable dans le dossier /bin [cite: 13]
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
add_executable(IntelliEditor ${SOURCES})

# Note : Les bibliothèques (Hunspell, Scintilla, cJSON) seront liées ici en Phase 2 [cite: 345, 346]

# Ajout du support des threads
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)