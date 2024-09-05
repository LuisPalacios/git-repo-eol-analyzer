# Analiza el EOL de los ficheros de un repositorio Git

Este proyecto se origina como una proyecto C++ MultiPlatforma, de tal manera que puede clonarse, compilarse y ejecutarse en Windows, MacOS y Linux.

- Compilador: Clang versión 17 con estándar C++20
- Build: CMake
- Formato: Clang-Format basado en el estilo Chromium
- IDE: VSCode con varias extensiones:
  - ClangD como intellisense, autocompletado, sugerencias, etc.
  - CMake y Cmake Tools

## Introducción

El objetivo de este proyecto es doble:

- Crear un ejemplo de proyecto multiplataforma en C++ con CMake y VSCode, que funcione transparentemente en Windows 11, MacOS y Linux Ubuntu
- Ayudar a identificar cual es el EOL (End Of Line) o final de línea que se está usando en los ficheros de texto de un repositorio.

Al trabajar en desarrollo de software multiplataforma, uno de los aspectos más sutiles pero cruciales que debes tener en cuenta es la diferencia entre los finales de línea en archivos de texto entre Windows y Linux/MacOS. Este pequeño detalle puede generar grandes problemas si no se maneja correctamente, especialmente cuando se trabaja en entornos mixtos, conflictos en el control de versiones incompatibilidades en scripts, problemas de compilación o ejecución.

## CLANG

Utilizo CLANG 17 y C++20 como el estándar por defecto. He configurado este repositorio de tal forma que VSCode utiliza la extension `clangd` para hacer el Intellisense, de hecho he deshabilitado el que trae por defecto.

## Preparar el Entorno

### Requisitos Previos

- **VSCode**: Asegúrate de tener instalado [VSCode](https://code.visualstudio.com/download) en tu sistema.
- **Extensiones de VSCode**: Instalo las extensiones del fichero [extensions.json](./.vscode/extensions.json):

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

El fichero [CMakeLists.txt](./CMakeLists.txt) está preparado para usar distintos PATHs y Variables de entorno según el sistema operativo.

- Abro VSCode > "CMake: Configure" (dentro del comando VSCode (Ctrl/Cmd + Shift + P)).
- Selecciono Clang como compilador.
- (En Windows solo) Asegúrate de que Ninja esta seleccionado como Generador preferido
- Después de la configuración, selecciona `CMake: Build` en la paleta de comandos.
- Una vez compilado, ejecuta el proyecto directamente desde VSCode o desde el terminal,

Ejecución

- Windows: `.\build\git-repo-eol-analyzer.exe`
- MacOS o Linux: `./build/git-repo-eol-analyzer`

## Contribuciones

Si tienes sugerencias o mejoras, siéntete libre de hacer un fork de este repositorio y enviar un pull request.

## Licencia

Este proyecto está bajo la licencia MIT. Para más detalles, consulta el archivo LICENSE.