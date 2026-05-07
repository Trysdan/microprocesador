; --- PRUEBA BANCO DE REGISTROS (FASE 2) ---
LDR R1, 100    ; R1 = 50
LDR R2, 101    ; R2 = 25
LDA 102        ; Acc = 10
ADDR R1        ; Acc = 10 + 50 = 60
ADDR R2        ; Acc = 60 + 25 = 85
STR R1, 103    ; RAM[103] = R1 (50)
OUT            ; Mostrar 85
HLT

@100
50
@101
25
@102
10
@103
0
