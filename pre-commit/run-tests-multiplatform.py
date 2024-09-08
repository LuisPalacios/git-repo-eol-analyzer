#!/usr/bin/env python3
import subprocess
import sys
import os

def main():
    # Definir el comando dependiendo del sistema operativo
    if sys.platform == 'win32':
        cmd = ['cmd.exe', '/c', r'.\pre-commit\run-tests.cmd']
    else:
        cmd = ['bash', './pre-commit/run-tests.sh']

    # Ejecutar el comando y pasar los argumentos si es necesario
    result = subprocess.call(cmd)

    return result

if __name__ == '__main__':
    exit(main())
