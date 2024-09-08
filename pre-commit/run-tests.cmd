@echo off

REM Navegar a la raíz del repositorio
for /f "delims=" %%i in ('git rev-parse --show-toplevel') do set REPO_ROOT=%%i
cd /d "%REPO_ROOT%"

REM Construir el proyecto con Clang
echo Building the project with Clang...
cmake -S . -B build -G "Ninja" -DCMAKE_C_COMPILER="C:\Program Files\LLVM\bin\clang.exe" -DCMAKE_CXX_COMPILER="C:\Program Files\LLVM\bin\clang++.exe"
if %errorlevel% neq 0 (
    echo Failed to configure the project.
    exit /b 1
)

cmake --build build
if %errorlevel% neq 0 (
    echo Build failed.
    exit /b 1
)

REM Ejecutar las pruebas
echo Running tests...
cd build
ctest --output-on-failure
set TEST_RESULT=%errorlevel%

REM Si las pruebas fallan, abortar el commit
if %TEST_RESULT% neq 0 (
    echo Tests failed. Aborting commit.
    exit /b 1
)

echo Tests passed. Proceeding with commit.
exit /b 0
