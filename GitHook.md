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

A partir de ahora cuando haga un commit se ejecutarán los test.

## Instalo `pre-commit`

Voy a ir un paso más allá. He instalado [`pre-commit`](https://pre-commit.com/), de tal modo que el script de testing lo saco al repositorio. Los desarrolladores solo tienen que installar pre-commit y los tests que yo haya dejado se ejecutarán.

La primera vez, instalo la herramienta `pre-commit` (como es un Mac uso `brew install pre-commit`). Al ejecutarlo va a sobreescribir el fichero que había hecho en el paso anterior.

Creo el fichero `.pre-commit-config.yaml` en la raíz del proyecto

```yaml
repos:
  - repo: local
    hooks:
      - id: run-tests
        name: Run CTest before commit
        entry: /usr/bin/env bash ./pre-commit/run-tests.sh
        language: script
        stages: [commit]
```

Vuelvo a crear el fichero que hace el test pero esta vez creo el directorio y el fichero `./pre-commit/run-tests.sh`

```bash
#!/bin/bash

# Navigate to the root of the repository
REPO_ROOT=$(git rev-parse --show-toplevel)
cd "$REPO_ROOT"

# Build the project
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

Cuando hago un commit observo lo siguiente:

```bash
🍏 luis@asterix:git-repo-eol-analyzer (●● main +?x) % git add .
🍏 luis@asterix:git-repo-eol-analyzer (● main +x) % git commit -m "Añado pre-commit"
Run CTest before commit..................................................Passed
[main 00cba6b] Añado pre-commit
 3 files changed, 33 insertions(+), 8 deletions(-)
 create mode 100644 .pre-commit-config.yaml
 delete mode 100644 .pre-commit.yaml
 create mode 100755 pre-commit/run-tests.sh
 ```

 El resto de desarrolladores solo tiene que instalarse `git-commit` y ejecutar `git-commit install` en su clone del respositorio.

Windows

PENDIENTE

MacOS

```bash
🍏 luis@asterix:git-repo-eol-analyzer (● main ?) % brew install pre-commit
:
🍏 luis@asterix:git-repo-eol-analyzer (● main ?) % pre-commit install
```

Linux: Instalo `sudo apt install pre-commit` y ejecuto `git-commit install` desde la raiz del repo.

```bash
⚡ luis@wwwin:git-repo-eol-analyzer (main $) % sudo apt install pre-commit
:
⚡ luis@wwwin:git-repo-eol-analyzer (main $) % pre-commit install
pre-commit installed at .git/hooks/pre-commit
```
