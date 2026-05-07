; programa.asm — Suma 120+90, verifica si > 200
MVI R1, 120
MVI R2, 90
MVI R4, 200
MVI R5, 255

ADD R3, R1, R2     ; R3 = 210
OUT R3             ; Muestra 210

SUB R6, R3, R4     ; R6 = 10
BEZ R6, fin_normal
OUT R5             ; Muestra 255 si > 200
HLT

fin_normal:
    OUT R3
    HLT
