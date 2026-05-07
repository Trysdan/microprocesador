; --- PROGRAMA FIBONACCI 8-BIT (CORREGIDO) ---
; Calcula la secuencia: 0, 1, 1, 2, 3, 5, 8, 13...

LDA 100    ; (0-1) Carga N
OUT        ; (2)   Muestra N
ADD 101    ; (3-4) Acc = N + M
STA 102    ; (5-6) TEMP = N + M
LDA 101    ; (7-8) Acc = M
STA 100    ; (9-10) N = M
LDA 102    ; (11-12) Acc = TEMP
STA 101    ; (13-14) M = TEMP

GRT 103    ; (15-16) ¿Acc > 200?
IF  21     ; (17-18) Si es verdad, salta al HLT (ADDR 21)
JMP 0      ; (19-20) Si no, vuelve al inicio

@21
HLT        ; (21) Fin

; --- DATOS ---
@100
0          ; N
@101
1          ; M
@102
0          ; TEMP
@103
200        ; LIMITE
