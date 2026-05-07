# Manual de Usuario: Microprocesador RISC 16-bit (Gaming Edition)

Este manual describe la arquitectura de 16 bits segmentada, el conjunto de instrucciones (ISA v2) y el sistema de periféricos MMIO diseñado para la ejecución de videojuegos.

## 1. Arquitectura del Sistema
El procesador es un diseño RISC con un **Pipeline de 5 etapas**:
1.  **IF (Fetch)**: Captura de la instrucción de 16 bits y actualización del PC.
2.  **ID (Decode)**: Decodificación, lectura del Register File y detección de riesgos.
3.  **EX (Execute)**: Operaciones de la ALU, resolución de saltos y forwarding.
4.  **MEM (Memory)**: Acceso a la RAM o Periféricos (MMIO).
5.  **WB (Write-Back)**: Escritura del resultado en el banco de registros.

### Características Técnicas:
- **Registros**: 8 registros de propósito general (R0-R7). **R0** está cableado a 0.
- **Memoria**: 256 bytes compartidos para instrucciones y datos (Dual Port).
- **Gestión de Riesgos**: 
    - **Forwarding Unit**: Resuelve dependencias EX->EX y MEM->EX sin pérdida de ciclos.
    - **Hazard Unit**: Inserta burbujas (stalls) cuando es estrictamente necesario.
    - **Delay Slot**: Los saltos tienen una latencia de 1 ciclo (la siguiente instrucción siempre se ejecuta).

---

## 2. Conjunto de Instrucciones (ISA v2)

### Formato de Instrucción
- **Tipo-R**: `[OpCode(4) | Rd(3) | Rs1(3) | Rs2(3) | Relleno(3)]`
- **Tipo-I**: `[OpCode(4) | Rd(3) | Imm8(8) | x(1)]`

### Tabla de Instrucciones
| Mnemónico | OpCode | Formato | Descripción |
| :--- | :---: | :---: | :--- |
| **MVI** Rd, Imm8 | 0x4 | I | Carga inmediata: Rd = Imm8 |
| **ADD** Rd, Rs1, Rs2 | 0x5 | R | Suma: Rd = Rs1 + Rs2 |
| **SUB** Rd, Rs1, Rs2 | 0x6 | R | Resta: Rd = Rs1 - Rs2 |
| **AND** Rd, Rs1, Rs2 | 0x7 | R | AND lógico: Rd = Rs1 & Rs2 |
| **OR**  Rd, Rs1, Rs2 | 0x8 | R | OR lógico: Rd = Rs1 \| Rs2 |
| **XOR** Rd, Rs1, Rs2 | 0x9 | R | XOR lógico: Rd = Rs1 ^ Rs2 |
| **MOV** Rd, Rs1 | 0x3 | R | Copia: Rd = Rs1 |
| **LOAD** Rd, Imm8 | 0x1 | I | Rd = RAM[Imm8] |
| **STORE** Rd, Imm8 | 0x2 | I | RAM[Imm8] = Rd |
| **LOADI** Rd, Rs1 | 0xA | R | Indirecto: Rd = RAM[Rs1] |
| **STORI** Rs1, Rs2 | 0x0 | R | Indirecto: RAM[Rs2] = Rs1 |
| **JMP** Label/Imm8 | 0xB | I | PC = Imm8 |
| **BEZ** Rd, Label/Imm8 | 0xC | I | Salta si Rd == 0 |
| **BNZ** Rd, Label/Imm8 | 0xD | I | Salta si Rd != 0 |
| **OUT** Rs1 | 0xE | R | Envía Rs1 al registro de salida del sistema |
| **HLT** | 0xF | - | Detiene el procesador |

---

## 3. Sistema de Periféricos (MMIO)
El acceso a hardware se realiza mediante direcciones de memoria específicas:
- **0xC0 - 0xEF**: **Video Framebuffer**. Una matriz de 8x6 píxeles que se dibuja en la terminal.
- **0xF0**: **Teclado (Input)**. Al leer esta dirección, se obtiene el código ASCII de la última tecla pulsada (no bloqueante).
- **0xF1**: **Refresh (Output)**. Escribir cualquier valor aquí fuerza el redibujado de la pantalla en consola.

---

## 4. Programación y Ensamblador
El simulador incluye un ensamblador de dos pasadas con soporte para:
1.  **Etiquetas**: Usa `nombre:` para definir puntos de salto y `JMP nombre` para referenciarlos.
2.  **Direccionamiento Absoluto**: Usa `@N` para fijar la dirección del código en memoria.
3.  **Comentarios**: Usa `;` para documentación interna del código.

### Ejemplo de Bucle (Countdown):
```asm
    MVI R1, 10
loop:
    OUT R1
    MVI R2, 1
    SUB R1, R1, R2
    BNZ R1, loop
    HLT
```

---

## 5. Ejecución
Para compilar y correr programas:
```bash
cd build
make microprocessor_v2
./microprocessor_v2 ../tu_programa.asm
```
