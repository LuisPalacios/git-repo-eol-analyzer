##########################################################################
## CMakeLists.txt
## CMake es una herramienta de código abierto y multiplataforma diseñada
## para construir, probar y empaquetar software. CMake trabaja con un
## sistema de compilación nativo para generar archivos de compilación
## y ejecutables de forma independiente del compilador.
##  - https://cmake.org/
##  - https://cmake.org/cmake/help/latest/guide/tutorial/index.html
##
##########################################################################

cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto
project(GitRepoEOLAnalyzer LANGUAGES CXX)

# Establece la versión de C++ a utilizar
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)  # Asegurarse de que no usa extensiones no estándar
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Detectar el sistema operativo y configurar el compilador para que
# utilice la versión concreta de Clang 17 que debe haberse instalado
# en el sistema
#
# MUY IMPORTANTE:
# 1) Asegurarse de que el compilador Clang 17 está instalado en el
# sistema. En caso contrario, el proceso de configuración fallará.
#
# 2) Asegurarse de que el compilador Clang 17 está en el PATH
#
# 3) Asegurarse de que Clang 17 está seleccionado en la extensión de
# CMake de VSCode, se hace desde el icono de CMake en la barra lateral
# de la izquierda, seleccionando la opción "Select a Kit" y eligiendo
# "Clang 17 ..."
#
if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")  # macOS
    message(STATUS "Configurando para MacOS")
    set(CMAKE_C_COMPILER "/opt/homebrew/opt/llvm@17/bin/clang")
    set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm@17/bin/clang++")
    set(LLVM_PATH "/opt/homebrew/opt/llvm@17")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I${LLVM_PATH}/include")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${LLVM_PATH}/include")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L${LLVM_PATH}/lib")
    # Evitar Warning: reexported library with install name
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-reexport_library,/opt/homebrew/opt/llvm@17/lib/libunwind.1.dylib")

elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")  # Linux
    message(STATUS "Configurando para Linux")
    set(CMAKE_C_COMPILER "/usr/bin/clang-17")
    set(CMAKE_CXX_COMPILER "/usr/bin/clang++-17")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I/usr/include/llvm-17")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/include/llvm-17")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/usr/lib/llvm-17/lib")

    # Forzar el compilador en Linux
    if (NOT DEFINED CMAKE_CXX_COMPILER_INITIALIZED_TO_DEFAULT)
        set(CMAKE_CXX_COMPILER "/usr/bin/clang++-17" CACHE FILEPATH "C++ compiler" FORCE)
    endif()
    if (NOT DEFINED CMAKE_C_COMPILER_INITIALIZED_TO_DEFAULT)
        set(CMAKE_C_COMPILER "/usr/bin/clang-17" CACHE FILEPATH "C compiler" FORCE)
    endif()

elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")  # Windows
    message(STATUS "Configurando para Windows")
    set(CMAKE_C_COMPILER "C:\\Program Files\\LLVM\\bin\\clang.exe")
    set(CMAKE_CXX_COMPILER "C:\\Program Files\\LLVM\\bin\\clang++.exe")
    set(LLVM_PATH "C:\\Program Files\\LLVM")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \"-I${LLVM_PATH}\\include\"")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \"-I${LLVM_PATH}\\include\"")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} \"-L${LLVM_PATH}\\lib\"")
else()
    message(FATAL_ERROR "Sistema operativo no soportado: ${CMAKE_SYSTEM_NAME}")
endif()

# Añadir el ejecutable principal
add_executable(git-repo-eol-analyzer
    src/main.cpp
    src/eol_analyzer.cpp
)

# Instalar el ejecutable según la plataforma
if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    install(TARGETS git-repo-eol-analyzer DESTINATION "C:/usr/local/bin")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    install(TARGETS git-repo-eol-analyzer DESTINATION "/usr/local/bin")
endif()

##########################################################################
## Unit Tests
## Utilizo GoogleTest, en concreto la modalidad de integrarme con
## CMake y descargaralo desde aquí, como un paso más del proceso de Build
##
##########################################################################

# GoogleTest desencadena que CMake busque pthread para saber si el sistema
# operativo soporta multihilo. En Windows, pthread no es parte del SO,
# ya que utiliza su propia API para manejar hilos, como WinAPI Threads.
# Por tanto, en Windows, no es necesario buscar pthread.
if (WIN32)
    set(gtest_disable_pthreads ON CACHE BOOL "Disable GoogleTest's use of pthreads" FORCE)
endif()
# Si mi proyecto requisiese pthread, tendria ademas que descomentar las
# tres líneas siguientes y ponerlas en la parte superior de CMakelists.txt
# if (NOT WIN32)
#    find_package(Threads REQUIRED)
# endif()

# Desactivar la instalación de GoogleTest
set(INSTALL_GTEST OFF CACHE BOOL "Disable installing GoogleTest" FORCE)
set(INSTALL_GMOCK OFF CACHE BOOL "Disable installing GoogleMock" FORCE)

# Añadir subdirectorio de GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.12.1.tar.gz
  # Esta línea evita la advertencia de que el archivo no tiene marca de tiempo
  # que provoca que CMake se resetee cada vez que se ejecuta
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# Evita que GoogleTest maneje los tests principales
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Desactivar la construcción de GoogleMock
set(BUILD_GMOCK OFF CACHE BOOL "Disable building GoogleMock" FORCE)

# Añadir GoogleTest
FetchContent_MakeAvailable(googletest)

# Habilitar los tests
enable_testing()

# Añadir el ejecutable de tests
add_executable(git-repo-eol-analyzer-tests
    tests/test_main.cpp
    src/eol_analyzer.cpp
)

# Enlazar GoogleTest con el ejecutable de tests
target_link_libraries(git-repo-eol-analyzer-tests gtest gtest_main)

# Asegura que el ejecutable de tests se compile correctamente
add_test(NAME GitRepoEOLAnalyzerTests COMMAND git-repo-eol-analyzer-tests WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
