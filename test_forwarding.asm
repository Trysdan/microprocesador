; test_forwarding.asm — Prueba de Forwarding
MVI R1, 5
MVI R2, 3
ADD R3, R1, R2     ; R3 = 8
ADD R4, R3, R1     ; R4 = 13
OUT R4
HLT
