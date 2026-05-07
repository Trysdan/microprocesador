# Manual de Usuario - Microprocesador de 8 Bits (SystemC)

Este manual describe la arquitectura, el conjunto de instrucciones (ISA) y cómo programar el microprocesador estructural de 8 bits desarrollado en SystemC.

## 1. Arquitectura del Sistema
El procesador utiliza una arquitectura de **8 bits** nativa:
- **Bus de Datos**: 8 bits.
- **Memoria RAM**: 256 Bytes (Direcciones 0x00 a 0xFF).
- **Formato de Instrucción**: 2 Bytes (Byte 1: OpCode, Byte 2: Operando/Dirección).
- **Registros Internos**:
  - `ACC` (Acumulador): Registro principal de 8 bits.
  - `RegB`: Registro temporal para la ALU.
  - `OUT`: Registro de salida para visualización.
  - `PC`: Contador de programa de 8 bits.
  - `IR`: Registro de instrucción (captura el OpCode).

---

## 2. Conjunto de Instrucciones (ISA)

| Mnemónico | OpCode | Descripción | Tamaño |
| :--- | :---: | :--- | :---: |
| **LDA** addr | 0x1 | Carga el valor de RAM[addr] en el Acumulador. | 2 Bytes |
| **ADD** addr | 0x2 | Suma RAM[addr] al Acumulador. | 2 Bytes |
| **SUB** addr | 0x5 | Resta RAM[addr] al Acumulador. | 2 Bytes |
| **STA** addr | 0xD | Guarda el valor del Acumulador en RAM[addr]. | 2 Bytes |
| **JMP** addr | 0x3 | Salta incondicionalmente a la dirección `addr`. | 2 Bytes |
| **JZ** addr  | 0x4 | Salta a `addr` si el Acumulador es cero. | 2 Bytes |
| **IF** addr  | 0xC | Salta a `addr` si el Acumulador NO es cero. | 2 Bytes |
| **GRT** addr | 0xB | Acc = 1 si Acc > RAM[addr], sino Acc = 0. | 2 Bytes |
| **EQL** addr | 0xA | Acc = 1 si Acc == RAM[addr], sino Acc = 0. | 2 Bytes |
| **AND** addr | 0x6 | Operación lógica AND (Acc = Acc & RAM[addr]). | 2 Bytes |
| **OR** addr  | 0x7 | Operación lógica OR (Acc = Acc \| RAM[addr]). | 2 Bytes |
| **NOT**      | 0x9 | Invierte los bits del Acumulador. | 1 Byte |
| **OUT**      | 0xE | Envía el valor del Acumulador al Registro de Salida. | 1 Byte |
| **HLT**      | 0xF | Detiene la ejecución del procesador. | 1 Byte |

---

## 3. Cómo Programar el Microprocesador

Los programas se escriben en archivos de texto con extensión `.asm`. El cargador (`Loader`) procesa estos archivos línea por línea.

### Reglas de Sintaxis:
1.  **Instrucciones**: Escribe el mnemónico seguido de la dirección decimal (ej: `LDA 100`).
2.  **Comentarios**: Usa el punto y coma `;` para añadir comentarios.
3.  **Directiva de Memoria (`@`)**: Usa `@` seguido de un número para saltar a una dirección específica. Es vital para colocar datos (ej: `@100` seguido de `50` pone el dato 50 en la dirección 100).
4.  **Alineación**: Recuerda que las instrucciones de 2 bytes ocupan dos posiciones de memoria. Si una instrucción está en ADDR 0, la siguiente debe estar en ADDR 2.

### Ejemplo de Programa:
```asm
; Sumar 5 + 10
LDA 100    ; Carga el 5 (ADDR 0-1)
ADD 101    ; Suma el 10 (ADDR 2-3)
OUT        ; Muestra resultado (ADDR 4)
HLT        ; Fin (ADDR 5)

@100
5          ; Dato en ADDR 100
@101
10         ; Dato en ADDR 101
```

---

## 4. Ejecución del Simulador

Para ejecutar un programa, compila el proyecto y pasa el nombre del archivo como argumento:

```bash
# En el directorio build
./microprocessor_final mi_programa.asm
```

El simulador mostrará en consola el rastro de cada ciclo de máquina (**T1 a T8**) y el valor final del registro de salida.

---

## 5. Ciclos de Máquina (Control Unit)
El procesador utiliza una máquina de estados de 8 tiempos para procesar cada instrucción:
- **T1-T3**: Fetch del OpCode (Busca el comando en la RAM).
- **T4-T5**: Fetch del Operando (Busca la dirección o dato si aplica).
- **T6**: Decodificación y preparación de buses.
- **T7-T8**: Ejecución final (Escritura en registros o RAM).
