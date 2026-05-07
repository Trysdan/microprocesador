# Manual de Usuario - Microprocesador de 8 Bits (SystemC)

Este manual describe la arquitectura, el conjunto de instrucciones (ISA) y cómo programar el microprocesador estructural de 8 bits desarrollado en SystemC.

## 1. Arquitectura del Sistema
El procesador utiliza una arquitectura de **8 bits** nativa:
- **Bus de Datos**: 8 bits.
- **Memoria RAM**: 256 Bytes (Direcciones 0x00 a 0xFF).
- **Formato de Instrucción**: 2 Bytes (Byte 1: OpCode, Byte 2: Operando/Dirección).
- **Registros Internos**:
  - `ACC` (Acumulador): Registro principal de 8 bits para operaciones ALU.
  - `Banco de Registros (R0-R7)`: 8 registros de uso general para almacenamiento rápido.
  - `RegB`: Registro temporal para la ALU (sujeta el segundo operando).
  - `OUT`: Registro de salida para visualización.
  - `PC`: Contador de programa de 8 bits.
  - `IR`: Registro de instrucción (captura el OpCode).

---

## 2. Conjunto de Instrucciones (ISA)

### Instrucciones de Memoria y Control
| Mnemónico | OpCode | Descripción | Tamaño |
| :--- | :---: | :--- | :---: |
| **LDA** addr | 0x01 | Carga el valor de RAM[addr] en el Acumulador. | 2 Bytes |
| **STA** addr | 0x0D | Guarda el valor del Acumulador en RAM[addr]. | 2 Bytes |
| **JMP** addr | 0x03 | Salta incondicionalmente a la dirección `addr`. | 2 Bytes |
| **JZ** addr  | 0x04 | Salta a `addr` si el Acumulador es cero. | 2 Bytes |
| **IF** addr  | 0x0C | Salta a `addr` si el Acumulador NO es cero. | 2 Bytes |
| **HLT**      | 0x0F | Detiene la ejecución del procesador. | 1 Byte |

### Instrucciones del Banco de Registros (Fase 2)
| Mnemónico | OpCode | Descripción | Tamaño |
| :--- | :---: | :--- | :---: |
| **LDR** Rx, addr | 0x10-17 | Carga RAM[addr] en el registro Rx (donde x es 0-7). | 2 Bytes |
| **STR** Rx, addr | 0x18-1F | Guarda el valor de Rx en RAM[addr]. | 2 Bytes |
| **ADDR** Rx      | 0x20-27 | Suma el valor de Rx al Acumulador (Acc = Acc + Rx). | 1 Byte |

### Instrucciones Aritmético-Lógicas (ALU)
| Mnemónico | OpCode | Descripción | Tamaño |
| :--- | :---: | :--- | :---: |
| **ADD** addr | 0x02 | Suma RAM[addr] al Acumulador. | 2 Bytes |
| **SUB** addr | 0x05 | Resta RAM[addr] al Acumulador. | 2 Bytes |
| **GRT** addr | 0x0B | Acc = 1 si Acc > RAM[addr], sino Acc = 0. | 2 Bytes |
| **EQL** addr | 0x0A | Acc = 1 si Acc == RAM[addr], sino Acc = 0. | 2 Bytes |
| **AND** addr | 0x06 | Operación lógica AND (Acc = Acc & RAM[addr]). | 2 Bytes |
| **OR** addr  | 0x07 | Operación lógica OR (Acc = Acc \| RAM[addr]). | 2 Bytes |
| **NOT**      | 0x09 | Invierte los bits del Acumulador. | 1 Byte |
| **OUT**      | 0x0E | Envía el valor del Acumulador al Registro de Salida. | 1 Byte |

---

## 3. Cómo Programar el Microprocesador

### Uso de Registros (R0-R7)
Los registros permiten evitar el acceso constante a la RAM, lo que acelera los bucles y cálculos complejos.
Ejemplo: `LDR R1, 100` carga el dato de la dirección 100 en el registro R1.

### Reglas de Sintaxis:
1.  **Instrucciones**: Escribe el mnemónico seguido de la dirección decimal o el nombre del registro (ej: `LDR R1, 100` o `ADDR R2`).
2.  **Comentarios**: Usa el punto y coma `;` para añadir comentarios.
3.  **Directiva de Memoria (`@`)**: Usa `@` seguido de un número para saltar a una dirección específica.
4.  **Alineación**: El cargador maneja automáticamente el tamaño de 1 o 2 bytes. 

### Ejemplo con Registros:
```asm
; Sumar R1 + R2 y mostrar resultado
LDR R1, 100    ; R1 = RAM[100]
LDR R2, 101    ; R2 = RAM[101]
LDA 102        ; Acc = 0
ADDR R1        ; Acc = Acc + R1
ADDR R2        ; Acc = Acc + R2
OUT            ; Muestra suma
HLT            ; Fin

@100
50             ; Dato 1
@101
25             ; Dato 2
@102
0              ; Inicializador
```

---

## 4. Ejecución del Simulador

Para ejecutar un programa, compila el proyecto y pasa el nombre del archivo como argumento:

```bash
# En el directorio build
./microprocessor_final mi_programa.asm
```

El simulador mostrará una tabla detallada con el rastro de cada ciclo:
- **Est**: Estado actual (T1-T8).
- **PC**: Contador de Programa.
- **Bus**: Valor actual en el bus central.
- **ACC/RegB**: Valores de los registros principales.

---

## 5. Ciclos de Máquina (Control Unit)
El procesador utiliza una máquina de estados de 8 tiempos:
- **T1-T3**: Fetch del OpCode.
- **T4-T5**: Fetch del Operando (si la instrucción es de 2 bytes).
- **T6**: Estabilización de señales.
- **T7**: Operación de Memoria/Registros (Lectura de RAM o transferencia entre registros).
- **T8**: Operación Final de la ALU (Escritura en el Acumulador).
