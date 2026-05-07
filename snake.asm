; =====================================================
;  snake.asm — Culebrita interactiva 8x6
; =====================================================

; --- Inicializacion ---
init:
    MVI R1, 4          ; X = 4
    MVI R2, 2          ; Y = 2
    MVI R5, 1          ; R5 = 1 (pixel ON)
    MVI R6, 8          ; Ancho = 8

draw_init:
    ADD R7, R2, R2     ; R7 = Y*2
    ADD R7, R7, R7     ; R7 = Y*4
    ADD R7, R7, R7     ; R7 = Y*8
    ADD R7, R7, R1     ; R7 = Y*8 + X
    MVI R4, 192        ; Base FB = 0xC0
    ADD R4, R4, R7     ; R4 = Dir pixel
    STORI R4, R5       ; Dibujar
    STORE R5, 241      ; Refrescar

; --- Bucle Principal ---
loop:
    LOAD R3, 240       ; Leer teclado (0xF0)
    BEZ R3, loop       ; Si no hay tecla, esperar

    ; Borrar pixel actual antes de mover
    STORI R4, R0       ; R0 es siempre 0

check_w:
    MVI R7, 119        ; 'w'
    SUB R7, R3, R7
    BNZ R7, check_s
    SUB R2, R2, R5     ; Y--
    JMP update

check_s:
    MVI R7, 115        ; 's'
    SUB R7, R3, R7
    BNZ R7, check_a
    ADD R2, R2, R5     ; Y++
    JMP update

check_a:
    MVI R7, 97         ; 'a'
    SUB R7, R3, R7
    BNZ R7, check_d
    SUB R1, R1, R5     ; X--
    JMP update

check_d:
    MVI R7, 100        ; 'd'
    SUB R7, R3, R7
    BNZ R7, loop       ; Tecla desconocida, ignorar
    ADD R1, R1, R5     ; X++

update:
    ; Recalcular direccion
    ADD R7, R2, R2
    ADD R7, R7, R7
    ADD R7, R7, R7
    ADD R7, R7, R1
    MVI R4, 192
    ADD R4, R4, R7
    
    ; Dibujar y refrescar
    STORI R4, R5
    STORE R5, 241
    JMP loop
