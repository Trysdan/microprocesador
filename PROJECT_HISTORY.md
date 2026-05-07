# Cronología y Evolución: Microprocesador RISC SystemC

Este documento registra la evolución técnica del proyecto, desde sus cimientos en lógica digital de 4 bits hasta su estado actual como una unidad de procesamiento de 16 bits optimizada para videojuegos.

## Fase 1: Cimientos y Lógica Estructural (Semanas 1-8)
- **Hito**: Creación de módulos básicos (Compuertas, Full Adders, Registros de 4 bits).
- **Justificación**: Establecer las bases de la lógica digital antes de la integración total. Se priorizó el diseño estructural sobre el funcional para entender el hardware a bajo nivel.

## Fase 2: Integración de Bus y 8 Bits (Semana 9)
- **Hito**: Implementación de `RAM16x8` y `Central Bus` con Tri-State.
- **Justificación**: El sistema original de 4 bits era muy limitado. Se migró a un bus de 8 bits para permitir el formato `[OpCode(4) | Operando(4)]`.

## Fase 3: Control de Flujo e ISA v1 (Semana 10)
- **Hito**: Implementación de `JMP`, `JZ`, `Zero Flag` y Banco de Registros inicial.
- **Justificación**: Permitir bucles y lógica condicional. Se introdujo el concepto de registros (R0-R7) para reducir el tráfico de memoria.

## Fase 4: Pipeline RISC de 5 Etapas (Evolución Mayor)
- **Hito**: Re-arquitectura completa a Datapath segmentado: **IF, ID, EX, MEM, WB**.
- **Justificación**: Para alcanzar el rendimiento necesario para juegos, se abandonó el diseño secuencial. El pipeline permite ejecutar una instrucción por ciclo de reloj ideal (CPI ≈ 1).
- **Innovación**: Implementación de una **Forwarding Unit** para resolver riesgos de datos (RAW) y un **Bypass Interno** en el Register File.

## Fase 5: Expansión a 16 Bits e ISA v2
- **Hito**: Implementación de instrucciones de 16 bits (Tipo-R y Tipo-I).
- **Justificación**: Los 8 bits impedían tener suficientes registros y opcodes simultáneos. Los 16 bits permitieron una arquitectura de **3 operandos** (`Rd, Rs1, Rs2`), optimizando drásticamente el código.

## Fase 6: Periféricos y Gaming Edition (Estado Actual)
- **Hito**: Implementación de **MMIO (Memory Mapped I/O)** y Videojuego **Snake**.
- **Justificación**: Se integró un **Framebuffer 8x6** (0xC0-0xEF) y entrada de teclado (0xF0). Para manejar estos periféricos de forma eficiente, se añadieron instrucciones de direccionamiento indirecto (**LOADI/STORI**), permitiendo el uso de punteros en ensamblador.
- **Resultado**: Procesador capaz de ejecutar software interactivo en tiempo real con una interfaz visual dinámica.

---
**Estado Actual**: RISC 16-bit Pipeline v2 con soporte MMIO y Gaming.
