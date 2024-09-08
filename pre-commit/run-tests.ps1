# Navegar a la raíz del repositorio
$REPO_ROOT = & git rev-parse --show-toplevel
Set-Location $REPO_ROOT

# Construir el proyecto con Clang
Write-Host "Building the project with Clang..."
& cmake -S . -B build -G "Ninja" -DCMAKE_C_COMPILER="C:/Program Files/LLVM/bin/clang.exe" -DCMAKE_CXX_COMPILER="C:/Program Files/LLVM/bin/clang++.exe"
& cmake --build build

# Ejecutar las pruebas
Write-Host "Running tests..."
Set-Location build
& ctest --output-on-failure
$TEST_RESULT = $LASTEXITCODE

# Si las pruebas fallan, abortar el commit
if ($TEST_RESULT -ne 0) {
    Write-Host "Tests failed. Aborting commit."
    exit 1
}

Write-Host "Tests passed. Proceeding with commit."
exit 0
