#!/bin/bash

# Script para automatizar la compilacion y ejecucion de pruebas en WSL
# Uso: chmod +x run_tests.sh && ./run_tests.sh

echo "============================================="
echo "   Iniciando compilacion y pruebas...        "
echo "============================================="

# Aseguramos que la variable SYSTEMC_HOME este configurada (por defecto a /usr/local/systemc si esta vacia)
if [ -z "$SYSTEMC_HOME" ]; then
    export SYSTEMC_HOME=/usr/local/systemc
fi

# Creamos la carpeta de construccion si no existe
if [ ! -d "build" ]; then
    mkdir build
fi

# Entramos a la carpeta de construccion
cd build

# Configuramos con CMake
echo "[1/3] Configurando CMake..."
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo > /dev/null

# Compila el codigo
echo "[2/3] Compilando el codigo..."
make -j$(nproc) > /dev/null

# Ejecuta los testbenches usando CTest
echo "[3/3] Ejecutando testbenches..."
ctest --output-on-failure

echo "============================================="
echo "   Proceso finalizado.                       "
echo "============================================="
