# Tests Unitarios para git-repo-eol-analyzer

Explico los tests unitarios creados para el proyecto, objetivos y cómo integrarlos y ejecutarlos usando Visual Studio Code.

Voy a utilizar [GoogleTest](https://github.com/google/googletest), un marco de testing y mocking para C++ que ha hecho Goole. Funciona en Linux, Windows o Mac, admite cualquier tipo de pruebas, no sólo pruebas unitarias.

 Desde hace un tiempo se fusionó el proyecto [GoogleMocks](https://google.github.io/googletest/gmock_for_dummies.html) dentro de GoogleTest; por lo que ahora te permite crear “mocks” o objetos simulados en C++. Un mock es un objeto “falso” que imita el comportamiento de otro objeto real, pero de manera controlada y personalizada, lo que es útil para hacer pruebas unitarias.

## Objetivo

Los tests unitarios tienen como objetivo garantizar que la funcionalidad de detección de fin de línea (EOL) en varios tipos de archivos funcione como se espera. Los tests se enfocan en diferentes combinaciones de terminadores de línea: LF, CR y CRLF.

### Tests

1. **Solo LF:**

   - Archivo: `test_files/only_lf.txt`
   - El test verifica que el archivo contiene solo terminaciones de línea LF.

2. **Solo CRLF:**

   - Archivo: `test_files/only_crlf.txt`
   - El test asegura que el archivo contiene solo terminaciones de línea CRLF.

3. **EOL mixtos (LF, CR, CRLF):**

   - Archivo: `test_files/mixed_eol.txt`
   - Este test valida que el archivo contiene terminaciones de línea LF, CR y CRLF.

## Archivos

- `tests/test_main.cpp`: Contiene los Test Cases para el proyecto utilizando GoogleTest.
- `test_files/`: Carpeta que contiene los archivos de ejemplo utilizados en los tests unitarios.

## Integración con VSCode

**Instalo GoogleTest:** como referencia, las versiones están en [releases](https://github.com/google/googletest/releases) y la documetación [aquí](https://github.com/google/googletest/tree/main/googletest). En mi caso, como utilizo **CMake**, el método de instalación que utilizo es apoyarme en él, para que descargue GoogleTest desde GitHub, como un paso más del proceso de Build. He añadido una sección dentro del [./CMakeLists.txt](./CMakeLists.txt)

**Actualicé `tasks.json`** y añadí comandos de compilación y test para facilitar la ejecución.

**Ejecuto los Tests:** siguiendo estos pasos

- Con `Ctrl|Cmd + Shift + B` para compilar los tests.
- Ejecuto los tests usando la tarea `Tasks: Run Test` (accesible mediante `Ctrl+Shift+P` o desde la paleta de comandos).

**Depurar Tests:**: Usa la configuración `Debug Tests (codelldb)` desde la pestaña de `Run and Debug` en Visual Studio Code para depurar los tests.

### Ejecución de Tests
Puedes ejecutar los tests seleccionando la tarea `Run Tests` o usando el terminal:
```bash
ctest --output-on-failure