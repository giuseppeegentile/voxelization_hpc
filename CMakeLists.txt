#cmake_minimum_required(VERSION 3.10)
#
#set(CMAKE_C_COMPILER "gcc")
#set(CMAKE_CXX_COMPILER "g++")
#
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
#
## set(HEADER_LIST "${PROJECT_SOURCE_DIR}/include/voxlib")
#
## add_library(voxlib voxlib.hpp ${PROJECT_SOURCE_DIR}/include/voxlib/voxlib.hpp)
#
##project name
#project(VoxelizationHPC CXX)
#
#add_compile_options(-std=c++17 -Wall -O3 -march=native -ffast-math)
#
##add executable
#add_executable(TestParser TestParser.cpp)
#
#
#target_compile_features(TestParser PRIVATE cxx_std_17)
#target_link_libraries(TestParser PRIVATE project_library)
#



############################################################################################

cmake_minimum_required(VERSION 3.5)
project(voxelization_hpc)                                            # nome del progetto

# Specifica la versione del C++ utilizzata nel tuo progetto
set(CMAKE_CXX_STANDARD 11)                                           # standard del linguaggio

# Specifica il percorso delle librerie hpp
include_directories(include/voxlib)                                  # ! directory degli header

# Aggiungi tutti i file sorgenti nella cartella src al tuo progetto
file(GLOB SOURCES "src/*.cpp")                                       # costruisce una lista di file partendo da src

# Aggiungi tutti i file sorgenti nella cartella test al tuo progetto # costruisce una lista di file partendo da test
file(GLOB TEST_SOURCES "tst/*.cpp")

# Crea l'eseguibile principale
# add_executable(tst ${TEST_SOURCES})               # crea gli eseguibili dai cpp dei test

# Crea gli eseguibili di test e linka le librerie necessarie
foreach(TEST_SOURCE ${TEST_SOURCES})                                # per ogni elemento ${TEST_SOURCE} nella lista TEST_SOURCE
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)        # crea una variabile ${TEST_NAME} uguale al nome del file cpp senza l'estensione
    add_executable(${TEST_NAME} ${TEST_SOURCE} ${SOURCES})                     # genera un eseguibile di nome ${TEST_NAME} e di sorgente ${TEST_SOURCE}
    #target_link_libraries(${TEST_NAME} include/voxlib)
endforeach()

add_compile_options(-std=c++17 -Wall -O3 -march=native -ffast-math)

# Specifica la cartella in cui creare il prodotto della compilazione
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)