; --- PROGRAMA DE PRUEBA 8-BIT NATIVO ---
; Sumar 120 + 90 y verificar si es > 200

LDA 100    ; Carga valor en ADDR 100 (120)
ADD 101    ; Suma valor en ADDR 101 (90)
OUT        ; Debe mostrar 210

GRT 102    ; Compara con limite en ADDR 102 (200)
IF  8      ; Si es mayor (Acc=1), salta a ADDR 8
HLT        ; Si no, termina (PC 7)

; Rutina de éxito (Direccion 8)
@8
LDA 103    ; Carga 255
OUT        ; Lo muestra
HLT        ; Fin

; --- DATOS EN MEMORIA ---
@100
120        ; Dato A
90         ; Dato B
200        ; Limite
255        ; Exito
