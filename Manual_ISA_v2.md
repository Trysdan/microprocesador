# Manual de la ISA RISC v2 (16 bits)

## Formatos de Instrucción

### Tipo-R (Aritmética/Lógica / Indirectos)
Uso: `OP Rd, Rs1, Rs2`
`[ OpCode(15:12) | Rd(11:9) | Rs1(8:6) | Rs2(5:3) | Relleno(2:0) ]`

| Mnemónico | OpCode | Descripción |
|-----------|--------|-------------|
| ADD       | 0101   | Rd = Rs1 + Rs2 |
| SUB       | 0110   | Rd = Rs1 - Rs2 |
| AND       | 0111   | Rd = Rs1 & Rs2 |
| OR        | 1000   | Rd = Rs1 \| Rs2 |
| XOR       | 1001   | Rd = Rs1 ^ Rs2 |
| MOV       | 0011   | Rd = Rs1 |
| LOADI     | 1010   | Rd = RAM[Rs1] (Load Indirecto) |
| STORI     | 0000   | RAM[Rs2] = Rs1 (Store Indirecto) |
| OUT       | 1110   | Output = Rs1 |

### Tipo-I (Inmediatos / Memoria / Saltos)
Uso: `OP Rd, Imm8`
`[ OpCode(15:12) | Rd(11:9) | Imm8(8:1) | 0(0) ]`

| Mnemónico | OpCode | Descripción |
|-----------|--------|-------------|
| MVI       | 0100   | Rd = Imm8 |
| LOAD      | 0001   | Rd = RAM[Imm8] |
| STORE     | 0010   | RAM[Imm8] = Rd |
| JMP       | 1011   | PC = Imm8 |
| BEZ       | 1100   | if Rd == 0: PC = Imm8 |
| BNZ       | 1101   | if Rd != 0: PC = Imm8 |
| HLT       | 1111   | Detiene el procesador |

## Mapa de Memoria (MMIO)
El procesador utiliza direccionamiento de 8 bits para memoria (0-255).
- **0x00 - 0xBF**: RAM de Propósito General (Instrucciones y Datos).
- **0xC0 - 0xEF**: **Framebuffer de Video** (8x6 píxeles, lineal).
- **0xF0**: **Entrada de Teclado** (Lectura del último ASCII presionado).
- **0xF1**: **Trigger de Refresh** (Cualquier escritura refresca la consola).

## Notas de Programación
1. **R0**: Siempre vale 0. Cualquier escritura en R0 es ignorada.
2. **Branch Latency**: Los saltos se resuelven en EX. Hay 1 *delay slot*.
3. **Forwarding**: El hardware resuelve automáticamente RAW hazards EX->EX y MEM->EX.
4. **Labels**: El ensamblador soporta etiquetas (ej: `loop:`) y saltos simbólicos (`JMP loop`).
