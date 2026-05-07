; mult.asm — Multiplicación por sumas repetidas
MVI R1, 10
MVI R2, 5
MVI R3, 0
MVI R4, 1

loop:
    ADD R3, R3, R1
    SUB R2, R2, R4
    BNZ R2, loop

    OUT R3
    HLT
