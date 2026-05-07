; max3.asm — Maximo de tres valores
MVI R1, 15
MVI R2, 80
MVI R3, 42
MVI R6, 128        ; mascara 0x80

; Paso 1: R4 = max(R1, R2)
MOV R4, R1
SUB R5, R1, R2
AND R5, R5, R6
BEZ R5, paso2
MOV R4, R2

paso2:
    SUB R5, R4, R3
    AND R5, R5, R6
    BEZ R5, finalizar
    MOV R4, R3

finalizar:
    OUT R4
    HLT
