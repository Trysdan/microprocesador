# Cronología de Hitos: Microprocesador SystemC

Este documento registra la evolución técnica del proyecto, los hitos alcanzados y la justificación de cada cambio arquitectónico.

## Fase 1: Cimientos y Lógica Estructural (Semanas 1-8)
*   **Hito**: Creación de módulos básicos (Compuertas, Full Adders, Registros de 4 bits).
*   **Razón**: Establecer las bases de la lógica digital antes de la integración total. Se priorizó el diseño estructural sobre el funcional para entender el hardware a bajo nivel.

## Fase 2: Integración de Bus y 8 Bits (Semana 9)
*   **Hito**: Implementación de `RAM16x8` y `Central Bus` con Tri-State.
*   **Razón**: El sistema original de 4 bits era muy limitado para instrucciones. Se migró a un bus de 8 bits para permitir el formato `[OpCode(4) | Operando(4)]`, permitiendo hasta 16 instrucciones distintas.
*   **Instrucciones iniciales**: `LDA`, `ADD`, `OUT` (básico), `HLT`.

## Fase 3: Control de Flujo y Toma de Decisiones (Semana 10)
*   **Hito**: Implementación de `JMP`, `JZ` y `Zero Flag`.
*   **Razón**: Sin saltos, el procesador solo podía ejecutar programas lineales. La adición de la bandera de cero en la ALU y las instrucciones de salto permitieron la creación de bucles (loops) y lógica condicional (if-else).
*   **Mejora ALU**: Se añadió la instrucción `SUB` para permitir restas y facilitar la activación de la bandera de cero.

## Fase 4: Interfaz de Salida y Estabilidad (Semana 11 - Parte A)
*   **Hito**: Creación del `OutputRegister` dedicado.
*   **Razón**: Se necesitaba una forma limpia de que el procesador "hablara" con el usuario sin depender de logs internos de SystemC. El nuevo registro imprime en consola solo cuando los datos cambian.
*   **Corrección Crítica**: Se arregló el bug de los operandos en el bus durante los saltos (`ir_out` en T4).

## Fase 5: Software y Automatización (Semana 11 - Parte B)
*   **Hito**: Implementación del `Cargador de Programas (Loader)` y `ISA`.
*   **Razón**: Cargar programas manualmente en C++ (`ram->write(...)`) era lento y propenso a errores. El Loader permite escribir código en ensamblador humano en un archivo `.asm`, facilitando la programación rápida.
*   **Infraestructura**: Automatización con `run_tests.sh` para asegurar que ningún cambio nuevo rompa la compatibilidad hacia atrás (Regresión).

---
**Estado Actual**: Computadora funcional de 8-bit programable mediante archivos externos.
