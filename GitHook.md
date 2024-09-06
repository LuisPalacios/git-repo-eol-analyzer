# Git Hooks

## Git pre-commit Hook

Para asegurarme de que los tests se ejecuten automáticamente antes de cada commit, voy a configurar un pre-commit hook en Git. Esto asegura que los tests unitarios se ejecuten y tengan éxito antes de permitir un commit.

Creo el hook de pre-commit:

El hook de pre-commit es un script que se ejecuta automáticamente antes de que un commit se realice. Este script debe estar ubicado en el directorio .git/hooks/ y llamarse pre-commit.

```shell
mkdir -p .git/hooks
touch .git/hooks/pre-commit
```

Edito el script del hook `e .git/hooks/pre-commit`. Si no está ya compilado, el proyecto se compilará, ejecuta los tests con `ctest`, aborta el commit si los tests fallan y lo permite si pasan.

```shell
#!/bin/bash

# Build the project (optional)
echo "Building the project..."
cmake -S . -B build
cmake --build build

# Run the tests
echo "Running tests..."
cd build
ctest --output-on-failure
TEST_RESULT=$?

# If the tests fail, abort the commit
if [ $TEST_RESULT -ne 0 ]; then
  echo "Tests failed. Aborting commit."
  exit 1
fi

echo "Tests passed. Proceeding with commit."
exit 0
```

Hago que el fichero sea ejecutable

```bash
chmod +x .git/hooks/pre-commit
```

## Instalo `pre-commit`

Para que todos los desarrolladores usen este pre-commit, instalo la herramienta `pre-commit` y creo el fichero `.pre-commit-config.yaml` en la raíz de tu proyecto

```yaml
repos:
  - repo: local
    hooks:
      - id: run-tests
        name: Run CTest
        entry: bash .git/hooks/pre-commit
        language: system
        stages: [commit]
```
