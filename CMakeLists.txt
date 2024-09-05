cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto
project(GitRepoEOLAnalyzer LANGUAGES CXX)

# Establece la versión de C++ a utilizar
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)  # Asegurarse de que no usa extensiones no estándar
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Añade el ejecutable
add_executable(git-repo-eol-analyzer src/main.cpp)