# Git Hooks

## Introducción

Para asegurarme de que mi repo cumpla una serie de estándares de formato, que pasa los tests antes del commit, etc. voy a emplear Git Hooks.

Una forma de hacerlo es creando scripts en `.git/hooks`, pero el problema que esto plantea es que hay que hacerlo cada vez que se clone por primera vez el repositorios.

Para resolverlo, existe un proyecto llamado precisamente [`pre-commit`](https://pre-commit.com/) que está desarrollado en Python y se apoya en un fichero llamado `.pre-commit-config.yaml` en la raiz del repositorio. Lo único que hay que hacer con el primer clone es ejecutar `pip install pre-commit` para que cree el fichero `.git/hooks/pre-commit`

### Prueba de concepto

Antes de instalar [`pre-commit`](https://pre-commit.com/), veamos cómo se haría sin dicha herramienta.

```shell
mkdir -p .git/hooks
touch .git/hooks/pre-commit
```

Edito un script `e .git/hooks/pre-commit`, que básicamente mira a ver si el proyecto está compilado, en su defecto lo compilará, ejecutará los tests con `ctest` y aborta el commit si los tests fallan, en caso conctrario lo permitirá.

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

Hago que el fichero sea ejecutable, modifico mi proyecto y hago un commit, si todo va bien verás cómo pasa el commit, porque los tests pasan.

```bash
chmod +x .git/hooks/pre-commit

touch poc.txt
git add .
git commit -m "Prueba pre-commit"
:
```

Ya puedo borrar el fichero de prueba y el pre-commit.

```bash
rm poc.txt ./git/hooks/pre-commit
```

## Instalo `pre-commit`

Prefiero usar [`pre-commit`](https://pre-commit.com/), de tal modo que el script de testing lo saco al repositorio. Los desarrolladores solo tienen que installar pre-commit y los tests que yo haya dejado se ejecutarán.

Instalo la herramienta `pre-commit`

* MacOS: `brew install pre-commit`
* Linux: `sudo apt install pre-commit`
* Windows: `pip install pre-commit` desde CMD o PowerShell. Nota: Es muy importante que tengas Python perfectamente instalado en Windows (echa un ojo a este [apunte](https://www.luispa.com/desarrollo/2024/08/25/win-desarrollo.html#python-pip-y-pipenv)).

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

Creo el script que ejecuta el test, [`./pre-commit/run-tests.sh`](./pre-commit/run-tests.sh)

Ejecuto `pre-commit install` para que me cree el script git hook

```bash
pre-commit install
```

La próxima vez que haga un commit, observo lo siguiente:

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

El resto de desarrolladores solo tiene que instalarse el programa y ejecutar `pre-commit install` en su clone del respositorio.
