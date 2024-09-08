# Introducción

Este repositorio empezó como un ejercicio para crear un proyecto C++ multiplataforma, que puediese **clonar, compilar y ejecutar en Windows, MacOS y Linux**. Fui avanzando y he terminado con un proyecto que utiliza:

- El compilador **Clang versión 17 con estándar C++20**
- Herramienta para hacer el Build: **CMake** y **Ninja**
- Herramienta para seguir una convención de Formato: **`clang-format`** basado en el estilo Chromium
- Como entorno IDE: **VSCode** con varias extensiones:
  - ClangD como intellisense, autocompletado, sugerencias, etc.
  - CMake y Cmake Tools
  - Debug con vscode-lldb.
  - Las Cpptools de VSCode.
  - Clang-format, prettier
  - Eliminar trailing spaces y markdownlint

## Objetivos

He marcado varios objetivos, de más a menos importantes:

- Crear un ejemplo de proyecto multiplataforma en C++
- Documentar cómo clonar, compilar y ejecutar en todos los sistemas operativos.
- Implementar `.gitattributes` para aprender a configurar bien el tema de CRLF vs LF.
- Que el ejecutable generado pueda decirme que EOL (End Of Line) tienen los archivos del working directory de un repositorio git.

Es una versión super light de lo que hace el propio `git`, de hecho no pretende ni mucho menos acercarse. El comando siguiente es potentísimo y nos da una información muy valiosa

```bash
git ls-files --eol
:
i/lf    w/crlf  attr/text=auto eol=lf   file.txt
```

En este caso, `i/lf` significa que el fichero usa `LF` en el index, `w/crlf` significa ue usa `CRLF` en el working directory.

El ejecutable `git-repo-eol-analyzer` es una versión LIGHT, informará de qué tiene en realidad los ficheros de texto del working directory.

```bash
git-repo-eol-analyzer
:
Directory: ./src:
 eol_analyzer.cpp:                                          LF
 eol_analyzer.h:                                            LF
 main.cpp:                                                  LF

Directory: ./test_files:
 test_CRLF_CR.test:                                         CRLF CR
 test_CRLF_LF.test:                                         CRLF LF
 test_CRLF_LF_CR.test:                                      CRLF LF CR
 test_LF_CR.test:                                           LF CR
 test_only_CR.test:                                         CR
 test_only_CRLF.test:                                       CRLF
 test_only_LF.test:                                         LF
```


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

Una vez que hayas hecho el clone, es importante, si quieres aprovechar el hook que he preparado, instalar `pre-commit` y activarlo en el repositorio. Lo explico en el documento [GitHook pre-commit](./doc/GitHook.md). Muy resumido sería:

- MacOS: `brew install pre-commit`
- Linux: `sudo apt install pre-commit`
- Windows: `pip install pre-commit`

```shell
pre-commit install
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

Consulta el documento [GitHook](./doc/GitHook.md) donde describo cómo configuro `pre-commit` en este repositorio.

## Contribuciones

Si tienes sugerencias o mejoras, siéntete libre de hacer un fork de este repositorio y enviar un pull request.

## Licencia

Este proyecto está bajo la licencia MIT. Para más detalles, consulta el archivo LICENSE.
