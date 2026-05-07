# SystemC Microprocessor Design / Diseno de Microprocesador en SystemC

*[Read in English](#english) | [Leer en Espanol](#espanol)*

---

<a id="english"></a>
## 🇬🇧 English

### Overview
This project is an educational, long-term endeavor to build a complete microprocessor from scratch using **SystemC**. The design approach is incremental, starting from fundamental digital logic components and scaling up to a fully functional processor architecture. 

**Current Status**: The processor has transitioned to a native **8-bit Architecture**. It features a native 8-bit CLA ALU, 256 bytes of RAM, and supports 2-byte instructions.

### Key Features
- **8-bit Native ALU**: Structural CLA (Carry Look-Ahead) logic for high-performance arithmetic and comparison.
- **256 Bytes RAM**: Expanded memory addressing (8-bit MAR).
- **2-Byte Instruction Format**: Enhanced ISA allowing large memory addressing and flexible operations.
- **Structural RTL Design**: Every component is built thinking in real gates and hardware cables.

### Milestones
- [x] **Phase 0**: 4-bit Structural Core (Accumulator based).
- [x] **Phase 1**: Transition to native 8-bit Architecture & 2-Byte Instructions.
- [ ] **Phase 2**: General Purpose Register File (R0-R7) & Internal Storage.
- [ ] **Phase 3**: Simple Video/Grid Display (16x16 Mesh).
- [ ] **Phase 4**: Basic Game Logic Implementation.

---

<a id="espanol"></a>
## 🇪🇸 Espanol

### Descripcion General
Este proyecto es una iniciativa educativa a largo plazo con el objetivo de construir un microprocesador completo desde cero utilizando **SystemC**. El enfoque de diseno es incremental, comenzando con componentes logicos fundamentales hasta llegar a una arquitectura de procesador totalmente funcional.

**Estado Actual**: El procesador ha transicionado exitosamente a una **Arquitectura Nativa de 8 bits**. Cuenta con una ALU nativa de 8 bits (CLA), 256 bytes de RAM y soporte para instrucciones de 2 bytes.

### Caracteristicas Clave
- **ALU Nativa de 8 bits**: Logica estructural CLA (Acarreo Anticipado) para operaciones aritmeticas y comparaciones de alto rendimiento.
- **256 Bytes de RAM**: Direccionamiento de memoria expandido (MAR de 8 bits).
- **Formato de Instruccion de 2 Bytes**: ISA mejorado que permite direccionar toda la memoria y operaciones flexibles.
- **Diseno Estructural RTL**: Cada componente esta construido pensando en compuertas reales y cables de hardware.

### Hitos del Proyecto
- [x] **Fase 0**: Nucleo estructural de 4 bits (basado en Acumulador).
- [x] **Fase 1**: Transicion a Arquitectura Nativa de 8 bits e Instrucciones de 2 Bytes.
- [ ] **Fase 2**: Banco de Registros de Proposito General (R0-R7) y Almacenamiento Interno.
- [ ] **Fase 3**: Pantalla de video simple (Malla de 16x16).
- [ ] **Fase 4**: Implementacion de logica de juego basica.

### Guia de Inicio
Para compilar y ejecutar el procesador con un programa especifico:
```bash
./run_tests.sh               # Ejecuta todos los tests unitarios
./microprocessor_final <archivo.asm>  # Ejecuta un programa especifico
```
Consulta el **[PROCESSOR_MANUAL.md](PROCESSOR_MANUAL.md)** para aprender a programar tu procesador.
