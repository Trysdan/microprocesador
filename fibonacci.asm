; fibonacci.asm — Secuencia de Fibonacci hasta 55
; 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55
MVI R1, 0          ; F(n-2)
MVI R2, 1          ; F(n-1)
MVI R5, 60         ; Limite

OUT R1             ; Muestra 0
OUT R2             ; Muestra 1

loop:
    ADD R3, R1, R2     ; R3 = F(n)
    OUT R3
    
    ; Actualizar para siguiente iteracion
    MOV R1, R2
    MOV R2, R3
    
    ; Comprobar limite
    MVI R6, 55
    SUB R6, R3, R6     ; Si R3 >= 55, R6 >= 0
    BEZ R6, fin        ; Si llegamos a 55, parar
    JMP loop

fin:
    HLT
