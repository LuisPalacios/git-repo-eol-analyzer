cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto
project(GitRepoEOLAnalyzer LANGUAGES CXX)

# Establece la versión de C++ a utilizar
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)  # Asegurarse de que no usa extensiones no estándar
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Detectar el sistema operativo
if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")  # macOS
    message(STATUS "Configuring for macOS")
    set(CMAKE_C_COMPILER "/opt/homebrew/opt/llvm@17/bin/clang")
    set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm@17/bin/clang++")
    set(LLVM_PATH "/opt/homebrew/opt/llvm@17")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I${LLVM_PATH}/include")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${LLVM_PATH}/include")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L${LLVM_PATH}/lib")

elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")  # Linux
    message(STATUS "Configuring for Linux")
    set(CMAKE_C_COMPILER "/usr/bin/clang-17")
    set(CMAKE_CXX_COMPILER "/usr/bin/clang++-17")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I/usr/include/llvm-17")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/include/llvm-17")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/usr/lib/llvm-17/lib")

elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")  # Windows
    message(STATUS "Configuring for Windows")
    set(CMAKE_C_COMPILER "C:/Program Files/LLVM/bin/clang.exe")
    set(CMAKE_CXX_COMPILER "C:/Program Files/LLVM/bin/clang++.exe")
    set(LLVM_PATH "C:/Program Files/LLVM")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I${LLVM_PATH}/include")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${LLVM_PATH}/include")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L${LLVM_PATH}/lib")

else()
    message(FATAL_ERROR "Unsupported operating system: ${CMAKE_SYSTEM_NAME}")
endif()

# Añade el ejecutable
add_executable(git-repo-eol-analyzer src/main.cpp)