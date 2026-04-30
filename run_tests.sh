#!/bin/bash

# Script para automatizar la compilacion y ejecucion de pruebas en WSL
# Uso: chmod +x run_tests.sh && ./run_tests.sh

echo "============================================="
echo "   Iniciando compilacion y pruebas...        "
echo "============================================="

if [ -z "$SYSTEMC_HOME" ]; then
    export SYSTEMC_HOME=/usr/local/systemc
fi

if [ ! -d "build" ]; then
    mkdir build
fi

# Copiamos archivos de programa .asm al directorio build
cp *.asm build/ 2>/dev/null

cd build

echo "[1/3] Configurando CMake..."
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo > /dev/null

echo "[2/3] Compilando el codigo..."
make -j$(nproc) > /dev/null

echo "[3/3] Ejecutando testbenches..."
ctest --output-on-failure

echo "============================================="
echo "   Proceso finalizado.                       "
echo "============================================="
