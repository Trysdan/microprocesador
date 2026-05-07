# Seguimiento de Hitos - Microprocesador SystemC

## Hito 1: Arquitectura Base 8-bit (Accumulator)
- [x] ALU básica y acumulador.
- [x] Memoria RAM 256x8.
- [x] Unidad de Control Secuencial (FSM).
- [x] ISA v1 (LDA, STA, ADD, SUB, OUT, HLT).

## Hito 2: Banco de Registros y ISA Extendida
- [x] Register File 8x8 (R0-R7).
- [x] Soporte para LDR, STR y ADDR.
- [x] Testbench de validación de registros.

## Hito 3: Pipeline RISC 5 Etapas (Actual)
- [x] Re-estructuración a Datapath de 5 etapas (IF, ID, EX, MEM, WB).
- [x] Implementación de Registros de Pipeline con Flush/Stall.
- [x] **Unidad de Forwarding** para resolver RAW hazards.
- [x] Nueva ISA de 16 bits (Tipo-R y Tipo-I).
- [x] Bypass Interno en el Register File (WB->ID).
- [x] Sistema de traza visual mejorada (Tabla de Simulación).

## Hito 4: Periféricos y Videojuegos (Completado)
- [x] Memory Mapped I/O (MMIO).
- [x] **Framebuffer 8x6** integrado (0xC0-0xEF).
- [x] **Entrada de Teclado** no bloqueante (0xF0).
- [x] **Instrucciones Indirectas** (LOADI/STORI) para punteros.
- [x] Loader con soporte para **Etiquetas (Labels)**.
- [x] Primer Videojuego funcional: **Snake** interactivo.
