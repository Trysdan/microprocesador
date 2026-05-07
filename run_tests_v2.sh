#!/bin/bash
# Script para automatizar la compilacion y ejecucion de pruebas de la v2
export SYSTEMC_HOME=/usr/local/systemc

if [ ! -d "build" ]; then
    mkdir build
fi

cd build
echo "[1/2] Compilando Pipeline RISC v2..."
cmake .. > /dev/null
make microprocessor_v2 -j$(nproc) > /dev/null

echo ""
echo "============================================="
echo "   REPORTE DE PRUEBAS - PIPELINE RISC v2     "
echo "============================================="

TESTS=("test_rf.asm" "logic.asm" "mult.asm" "count.asm" "programa.asm" "fibonacci.asm" "max3.asm" "test_loop.asm")

for t in "${TESTS[@]}"; do
    printf "%-20s | " "$t"
    RESULT=$(./microprocessor_v2 "../$t" 2>&1 | grep "RESULTADO FINAL" | cut -d':' -f2 | xargs)
    if [ -n "$RESULT" ]; then
        echo "EXITO (Resultado: $RESULT)"
    else
        echo "FALLO"
    fi
done
echo "============================================="
