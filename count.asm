; count.asm — Contador regresivo de 10 a 0
MVI R1, 10
MVI R2, 1

loop:
    OUT R1
    SUB R1, R1, R2
    BNZ R1, loop

    OUT R1          ; Muestra el 0 final
    HLT
