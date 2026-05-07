# Hoja de Ruta: Gaming CPU (8-bit GPR Architecture)

Este documento detalla la evolución técnica desde el microprocesador actual hacia una arquitectura de propósito general capaz de ejecutar juegos básicos.

---

## Fase 1: Infraestructura de 8 bits (Fundamentos)
**Objetivo**: Expandir el bus de direcciones y la capacidad aritmética.

### ¿Cómo lo haremos?
1.  **PC y MAR (8 bits)**: Modificaremos las clases `ProgramCounter` y `MAR` para que sus puertos de datos y registros internos pasen de `sc_uint<4>` a `sc_uint<8>`.
2.  **RAM (256x8)**: Actualizaremos el direccionamiento de la memoria. Ya no usaremos un arreglo de 16 posiciones, sino de 256. La lógica de selección de dirección ahora leerá los 8 bits completos del MAR.
3.  **ALU Estructural (8 bits)**: 
    *   **Cascada**: Conectaremos dos sumadores estructurales de 4 bits en serie. El acarreo de salida (`cout`) del primer bloque se conectará al acarreo de entrada (`cin`) del segundo bloque.
    *   **Banderas**: Actualizaremos la lógica del `Zero Flag` para que verifique los 8 bits de salida mediante una red de compuertas NOR.

**Verificación**: Crearemos un programa en la dirección `0x00` que salte a la dirección `0xFE` (254) para validar que el direccionamiento extendido funciona.

---

## Fase 2: El Banco de Registros (R0-R7)
**Objetivo**: Implementar memoria interna de alta velocidad.

### ¿Cómo lo haremos?
1.  **Módulo `RegisterFile`**: Crearemos un nuevo componente que contiene un arreglo de 8 registros (`sc_signal<sc_uint<8>> registers[8]`).
2.  **Lógica de Selección**:
    *   Añadiremos 3 líneas de selección (`reg_sel`) que vendrán de la Unidad de Control.
    *   Usaremos un **Decodificador 3-a-8** interno para activar el `load_enable` del registro específico que queramos escribir.
3.  **Integración con el Bus**: El banco de registros tendrá un único puerto de salida al bus central, controlado por una señal `reg_file_out`.

**Verificación**: Una prueba de "Mover datos": Cargar un valor en R0, moverlo a R4, y de R4 sacarlo al Registro de Salida (OUT).

---

## Fase 3: Unidad Lógica y Desplazamiento (Manipulación de Bits)
**Objetivo**: Permitir cálculos lógicos para motores de juegos (colisiones/sprites).

### ¿Cómo lo haremos?
1.  **Bloques Lógicos**: Añadiremos módulos estructurales para compuertas `AND`, `OR`, `XOR` y `NOT` que operen bit a bit sobre las entradas A y B.
2.  **Shifter (Desplazador)**: Implementaremos un desplazador de bits (Shift Left/Right). En hardware, esto se hace "cableando" la salida un bit a la izquierda o derecha de la entrada.
3.  **Multiplexor de ALU**: La Unidad de Control ahora enviará un código de operación extendido para seleccionar si la salida de la ALU será la Suma, la Resta, el AND, etc.

**Verificación**: Realizar una operación `AND` entre `0xAA` y `0x0F` para verificar el enmascaramiento de bits.

---

## Fase 4: Control de Flujo Avanzado (Pila y Llamadas)
**Objetivo**: Soportar funciones y modularidad.

### ¿Cómo lo haremos?
1.  **Stack Pointer (SP)**: Añadiremos un registro de 8 bits que apunte a la zona alta de la RAM (ej. inicia en `0xFF`).
2.  **Lógica `CALL`**: Al llamar a una función, la CU debe:
    1. Poner el valor actual del PC en el bus.
    2. Escribirlo en la RAM en la dirección del SP.
    3. Decrementar el SP.
    4. Cargar el PC con la dirección de la función.
3.  **Lógica `RET`**: Hacer el proceso inverso para recuperar el PC previo.

**Verificación**: Un programa con un bucle que llame a una subrutina de "parpadeo" o "incremento" y regrese exitosamente.

---

## Fase 5: Motor Gráfico y Entrada (I/O Mapeado)
**Objetivo**: Visualización y control.

### ¿Cómo lo haremos?
1.  **VRAM**: Designaremos un rango de la RAM (ej. `0xF0` a `0xFF`) como Memoria de Video.
2.  **Renderer**: Crearemos un proceso en `main.cpp` que cada N ciclos lea esa zona de la RAM y dibuje una matriz de caracteres en la terminal simulando una pantalla.
3.  **Input**: Crearemos una dirección de memoria "mágica" que, al ser leída, devuelva el estado de las teclas presionadas por el usuario.

**Verificación**: El "Hola Mundo" de los juegos: un punto que se mueve por la pantalla usando las teclas WASD.
