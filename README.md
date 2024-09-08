# Introducción

Este repositorio empieza como un ejercicio para tener proyecto C++ multiplataforma, de tal manera que puediese **clonarse, compilarse y ejecutarse en Windows, MacOS y Linux**. Para conseguirlo he ido hacieno modificaciones, hasta llegar a seleccionar y configurar lo siguiente:

- Compilador **Clang versión 17 con estándar C++20**
- Herramieta para hacer el Build: **CMake** y **Ninja**
- Herramienta para seguir una convención de Formato: **`clang-format`** basado en el estilo Chromium
- Como entorno IDE: **VSCode** con varias extensiones:
  - ClangD como intellisense, autocompletado, sugerencias, etc.
  - CMake y Cmake Tools
  - Debug con vscode-lldb.
  - Las Cpptools de VSCode.
  - Clang-format, prettier
  - Eliminar trailing spaces y markdownlint

## Objetivos

He marcado varios objetivos

- Crear un ejemplo de proyecto multiplataforma en C++ con CMake y VSCode, que funcione transparentemente en Windows 11, MacOS y Linux Ubuntu
- Documentar cómo clonar, compilar y ejecutar el proyecto.
- Que el propio proyecto sirva para algo. En desarrollo de software multiplataforma, uno de los aspectos más sutiles pero cruciales que debes tener en cuenta es la diferencia entre los finales de línea en archivos de texto entre Windows y Linux/MacOS. Por lo tanto, este proyecto genera un ejecutable que te ayuda a identificar cual es el EOL (End Of Line) o final de línea que se está usando en los ficheros de texto de un repositorio.
- Implementar `.gitattributes` para aprender a configurar bien el tema de CRLF vs LF.

## Preparar el Entorno

### Requisitos Previos

- **VSCode**: Asegúrate de tener instalado [VSCode](https://code.visualstudio.com/download) en tu sistema.
- **Extensiones de VSCode**: Instalo todas las extensiones del fichero [.vscode/extensions.json](./.vscode/extensions.json)

Importante:

- Me aseguro de que el compilador Clang 17 está instalado en el sistema.
- Me aseguro de que el compilador Clang 17 está en el PATH
- Me aseguro de que Clang 17 está seleccionado en la extensión de CMake de VSCode, se hace desde el icono de CMake en la barra lateral de la izquierda, seleccionando la opción "Select a Kit". Elijo el que empieza por "`Clang 17...`"

### Clonar

Clonar el repositorio en Windows, MacOS o Linux

```shell
git clone https://github.com/LuisPalacios/git-repo-eol-analyzer.git
```

***Detalles de la Parametrizacion***

- [.vscode/settings.json](./.vscode/settings.json) - Configuro múltiples parámetros y cómo trabajar con Clang
- [.clang-format](./.clang-format) - Utilizo clang-format como formato C++
- [.clangd](./.clangd) - Utilizo clangd como sistema de intellisense (deshabilito el nativo de VSCode)
- [.markdownlint.json](./.markdownlint.json) - Parametrizacion de la extension Markdown Lint
- [.gitattributes](./.gitattributes) - Opciones de CRLF / LF

### Instalacion del compilador Clang/LLVM

#### Windows 11 - CLang/CMake

- Descargo e instalo ***CLANG 17.0.1*** desde el sitio de [Releases oficial](https://github.com/llvm/llvm-project/releases). [link directo a LLVM 64bits 17.0.1 para Windows)](https://github.com/llvm/llvm-project/releases/download/llvmorg-17.0.1/LLVM-17.0.1-win64.exe).
- Instalo CMake desde el [sitio oficial](https://cmake.org/download/), me bajo el *Windows x64 Installer*
- Instalo Ninja (Generador) desde [su repositorio oficial](https://github.com/ninja-build/ninja/releases) y lo guardo en un diretorio que tengo en el PATH

#### MacOS - CLang/CMake

- CLANG ya viene preinstalado con Xcode Command Line Tools. Puedes instalarlo con el siguiente comando:

```bash
xcode-select --install
```

- Instalo `cmake/clang-format` utilizando Homebrew:

```bash
brew update
brew upgrade
brew install llvm@17
brew install cmake
brew install clang-format
brew install ninja
```

Puedes ver con `brew info llvm@17` dónde ha quedado instalado

#### Linux - CLang/CMake

- Instalo `cmake/clang-format` en Ubuntu:

```bash
sudo apt update && sudo apt upgrade -y && sudo apt full-upgrade -y
sudo apt install clang-17
sudo apt install clang-format
sudo apt install cmake
```

Para instalar ninja desde los fuentes en linux:

```shell
cd tmp
git clone git clone https://github.com/ninja-build/ninja.git
cd ninja && git checkout release
cmake -Bbuild-cmake && cmake --build build-cmake
sudo cp build-cmake/ninja /usr/local/bin/ninja
```

## Compilación del Proyecto

Estructura del proyecto

```bash
git-repo-eol-analyzer/
│
├── CMakeLists.txt
├── src/
│   └── main.cpp
└── README.md
```

Configuración con CMake (Línea de Comandos)

- Linux/MacOS: `cmake -S . -B build -G Ninja`
- Windows: `cmake -S . -B build -G Ninja -DCMAKE_C_COMPILER="C:/Program Files/LLVM/bin/clang.exe" -DCMAKE_CXX_COMPILER="C:/Program Files/LLVM/bin/clang++.exe"`

Compilación con CMake (Línea de Comandos). Una vez que el proyecto ha sido configurado correctamente,

- Linux/MacOS/Windows: `cmake --build build`

Configuración y Compilación con VSCode

- Abro VSCode > "CMake: Configure" (dentro del comando VSCode (Ctrl/Cmd + Shift + P)).
- Selecciono `Clang` como compilador y me aseguro de que `Ninja` esta seleccionado como Generador preferido
- Configurará o Generará de forma automática.
- Alternativamente puedes entrar en commando > Tasks: Run Tasks y seleccionar la que te interese.
- Tambien puedes pulsar `Ctrl|Cd + Shift + B` para que haga el build.

Ejecución

- Windows: `.\build\git-repo-eol-analyzer.exe`
- MacOS o Linux: `./build/git-repo-eol-analyzer`

### Instalación de los ejecutables

Se pueden instalar los ejecuables en un directorio del sistema, en este caso copia `git-repo-eol-analyzer` (o `git-repo-eol-analyzer.exe` en Windows)  a `/usr/local/bin` (o `c:\user\local\bin`).

- Linux/MacOS: `sudo cmake --install build`
- Windows: `cmake --install build`

## Unit Tests

He preparado [Unit Tests](./doc/UnitTests.md) para este repo.

Ejecución

- Windows: `.\build\git-repo-eol-analyzer-tests.exe`
- MacOS o Linux: `./build/git-repo-eol-analyzer-tests`

## Git Hooks

Consulta el documento [GitHook](./doc/GitHook.md) donde describo cómo configuro `pre-commit`

## Contribuciones

Si tienes sugerencias o mejoras, siéntete libre de hacer un fork de este repositorio y enviar un pull request.

## Licencia

Este proyecto está bajo la licencia MIT. Para más detalles, consulta el archivo LICENSE.
