; test_rf.asm — Prueba del Register File de 3 puertos
MVI R1, 50
MVI R2, 25
MVI R3, 10         ; acumulador

ADD R3, R3, R1     ; R3 = 60
ADD R3, R3, R2     ; R3 = 85

OUT R3             ; Resultado: 85
HLT
