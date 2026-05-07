; test_pipeline.asm — Prueba Completa del Pipeline
MVI R1, 5
MVI R2, 3
ADD R3, R1, R2         ; R3 = 8
ADD R4, R3, R1         ; R4 = 13
BEZ R1, error_jump     ; no debe saltar
JMP out_section

error_jump:
    MVI R4, 99         ; si llega aquí es un error

out_section:
    OUT R4             ; imprime 13
    HLT
