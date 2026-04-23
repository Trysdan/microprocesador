#!/bin/bash
# Script para compilar y correr todas las pruebas (Testbenches) de SystemC en WSL

echo "============================================="
echo "   Iniciando compilación y pruebas...        "
echo "============================================="

# Aseguramos que la variable SYSTEMC_HOME esté configurada (por defecto a /usr/local/systemc si está vacía)
if [ -z "$SYSTEMC_HOME" ]; then
    export SYSTEMC_HOME=/usr/local/systemc
    echo "Info: SYSTEMC_HOME no estaba configurado. Usando valor por defecto: $SYSTEMC_HOME"
fi

# Crea la carpeta build si no existe
mkdir -p build

# Ingresa a la carpeta build
cd build

# Genera los archivos de CMake
echo "[1/3] Configurando CMake..."
cmake ..

# Compila el código
echo "[2/3] Compilando el código..."
make -j$(nproc)

# Corre las pruebas usando CTest
echo "[3/3] Ejecutando testbenches..."
ctest --output-on-failure

echo "============================================="
echo "   Proceso finalizado.                       "
echo "============================================="
