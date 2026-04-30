# Manual del Microprocesador SystemC (Semana 11)

Este documento detalla la arquitectura, el conjunto de instrucciones y el flujo de trabajo para utilizar el microprocesador estructural desarrollado en SystemC.

## 1. Especificaciones Tecnicas
- **Arquitectura**: Basada en SAP-1 (Simple As Possible), implementada a nivel estructural (RTL).
- **Bus de Datos**: 8 bits con soporte para logica Tri-State (Z).
- **Instrucciones**: 8 bits (4 bits de OpCode + 4 bits de Operando).
- **Memoria**: RAM de 16 posiciones x 8 bits cada una.
- **Registros**: 
  - `RegisterA` (Acumulador principal).
  - `RegisterB` (Buffer para la ALU).
  - `OutputRegister` (Registro de salida con feedback en consola).
- **ALU**: Estructural de 4 bits con soporte para Suma y Resta, incluyendo bandera de Cero (Zero Flag).

---

## 2. Conjunto de Instrucciones (ISA)

El procesador soporta las siguientes instrucciones empaquetadas en un byte `[OpCode(4) | Operando(4)]`:

| Mnemonic | OpCode (Hex) | Descripcion |
|:---:|:---:|:---|
| **LDA** | `0x1` | **Load Accumulator**: Carga el valor de la RAM[Operando] en el Acumulador. |
| **ADD** | `0x2` | **Add**: Suma RAM[Operando] al valor del Acumulador. |
| **JMP** | `0x3` | **Jump**: Salto incondicional a la direccion indicada por el Operando. |
| **JZ** | `0x4` | **Jump if Zero**: Salta si la bandera de cero esta activa. |
| **SUB** | `0x5` | **Subtract**: Resta RAM[Operando] al valor del Acumulador. |
| **OUT** | `0xE` | **Output**: Muestra el valor del Acumulador en el Registro de Salida. |
| **HLT** | `0xF` | **Halt**: Detiene la ejecucion del programa. |

---

## 3. Flujo de Software (Asm -> Hardware)

El sistema incluye un **Loader (Cargador)** automatico que procesa archivos de texto plano con extension `.asm`.

### Formato del archivo `programa.asm`:
- Se escribe una instruccion o dato por linea.
- El Loader mapea secuencialmente cada linea a una direccion de RAM (0, 1, 2...).
- Para cargar datos directos (variables), simplemente escribe el numero en la linea correspondiente.

**Ejemplo de Programa (Suma 3 + 6):**
```asm
LDA 4    ; Direccion 0: Carga dato de la dir 4
ADD 5    ; Direccion 1: Suma dato de la dir 5
OUT      ; Direccion 2: Muestra resultado
HLT      ; Direccion 3: Fin
3        ; Direccion 4: Dato A
6        ; Direccion 5: Dato B
```

---

## 4. Como Ejecutar

### Requisitos
- WSL (Ubuntu/Debian).
- SystemC 3.0.0 instalado en `/usr/local/systemc`.
- CMake y compilador C++.

### Comandos Rapidos
1.  **Compilar y Correr Todo**:
    ```bash
    ./run_tests.sh
    ```
2.  **Ejecutar solo el programa principal (`programa.asm`)**:
    ```bash
    wsl bash -c "cp programa.asm build/ && cd build && ./microprocessor_final"
    ```

---

## 5. Estructura del Proyecto
- `/ALU`: Logica estructural a nivel de compuertas.
- `/Control`: Unidad de Control (FSM), IR, PC.
- `/Memory`: RAM y MAR.
- `/Registers`: Acumulador, Registro B y Registro de Salida.
- `/legacy`: Versiones anteriores del diseño para referencia.
- `main.cpp`: Banco de pruebas principal con el Cargador de Programas.

---

## 6. Visualizacion de Resultados
El procesador genera dos tipos de salida:
1.  **Terminal**: Muestra el rastro (trace) de cada ciclo (T1-T6) y los cambios en el registro de salida `[DISPLAY]`.
2.  **Ondas (VCD)**: Genera archivos `.vcd` (ej. `microprocessor_loader.vcd`) que pueden abrirse con **GTKWave** para ver los cables internos.
