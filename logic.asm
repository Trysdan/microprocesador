; logic.asm — Operaciones lógicas AND, XOR
MVI R1, 170
MVI R2, 15
AND R3, R1, R2     ; R3 = 10
XOR R4, R3, R2     ; R4 = 5
OUT R4
HLT
