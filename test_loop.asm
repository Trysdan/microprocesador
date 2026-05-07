; test_loop.asm — Suma 5+4+3+2+1 = 15
MVI R1, 5          ; contador
MVI R2, 0          ; acumulador
MVI R3, 1          ; paso

loop:
    ADD R2, R2, R1
    SUB R1, R1, R3
    BNZ R1, loop

    OUT R2             ; Resultado: 15
    HLT
