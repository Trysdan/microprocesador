# SystemC Microprocessor Design / Diseno de Microprocesador en SystemC

*[Read in English](#english) | [Leer en Espanol](#espanol)*

---

<a id="english"></a>
## 🇬🇧 English

### Overview
This project is an educational, long-term endeavor to build a complete microprocessor from scratch using **SystemC**. The design approach is incremental, starting from fundamental digital logic components and scaling up to a fully functional processor architecture. 

Currently, the project features a fully functional **Structural RTL 4-bit ALU** (with Zero Flag), a **RegisterA** acting as an Accumulator, a **16x8 RAM Module**, a **Central Data Bus (8-bit)** with Tri-State logic, an **Instruction Register (IR)**, a **Program Counter (PC)**, a **Control Unit (CU)** with an integrated **FSM**, and a dedicated **Output Register** for hardware-to-console feedback.

The processor now supports a complete set of instructions: **LDA, ADD, SUB, JMP, JZ, AND, OR, XOR, NOT, EQL, GRT, OUT, and HLT**.


### Documentation
For detailed technical specifications, instruction set details (ISA), and usage guides, please refer to the:
👉 **[PROCESSOR_MANUAL.md](PROCESSOR_MANUAL.md)**


### Project Structure
To maintain a clear record of the design evolution, the repository is structured as follows:
- `ALU/`: Contains the active development files for the Arithmetic Logic Unit and related components (pure structural logic using gate-level boolean equations).
- `Registers/`: Contains state-holding elements, including `RegisterA`, `RegisterB` for ALU buffering, and the `OutputRegister`.
- `Control/`: Instruction Register (IR), Program Counter (PC), and the central **Control Unit (CU)** managing the machine cycles (T1-T6).
- `Memory/`: Contains the Random Access Memory structures, including the `RAM16x8` and the `MAR` (Memory Address Register).
- `legacy/`: Serves as an archive for previous versions and iterations of the code.


### Getting Started (SystemC Setup)
Since SystemC is a C++ library, you need a C++ compiler and the SystemC library installed.

#### Linux / WSL (Ubuntu/Debian)
This project is natively compiled using Linux or the Windows Subsystem for Linux (WSL).

1. **Install dependencies:**
   ```bash
   sudo apt update
   sudo apt install build-essential cmake
   ```
2. **Install SystemC:**
   Ensure you have SystemC installed (for example, in `/usr/local/systemc`). Set the environment variable:
   ```bash
   export SYSTEMC_HOME=/usr/local/systemc
   ```
3. **Compile and Run:**
   You can use the provided bash script to automatically build and run all testbenches with a single command:
   ```bash
   ./run_tests.sh
   ```

---

<a id="espanol"></a>
## 🇪🇸 Espanol

### Descripcion General
Este proyecto es una iniciativa educativa a largo plazo con el objetivo de construir un microprocesador completo desde cero utilizando **SystemC**. El enfoque de diseno es incremental, comenzando con componentes logicos fundamentales hasta llegar a una arquitectura de procesador totalmente funcional.

Actualmente, el proyecto cuenta con una **ALU Estructural RTL de 4 bits** (con bandera de Cero), un **RegisterA** funcionando como Acumulador, un **Modulo RAM 16x8**, un **Bus de Datos Central (8-bit)** con logica Tri-State (Z), un **Registro de Instruccion (IR)**, un **Contador de Programa (PC)**, una **Unidad de Control (CU)** con una **FSM** integrada, y un **Registro de Salida (Output Register)** para visualizacion en consola.

El procesador soporta ahora un conjunto completo de instrucciones: **LDA, ADD, SUB, JMP, JZ, AND, OR, XOR, NOT, EQL, GRT, OUT y HLT**.


### Documentacion Detallada
Para especificaciones tecnicas, detalles del conjunto de instrucciones (ISA) y guias de uso, consulta el:
👉 **[PROCESSOR_MANUAL.md](PROCESSOR_MANUAL.md)**


### Estructura del Proyecto
Para mantener un registro claro de la evolucion del diseno, el repositorio se estructura de la siguiente manera:
- `ALU/`: Contiene los archivos de desarrollo activo para la Unidad Aritmetico Logica y componentes (logica estructural pura a nivel de compuertas y ecuaciones booleanas).
- `Registers/`: Contiene los elementos de estado o memoria, incluyendo los modulos `RegisterA`, `RegisterB` y el `OutputRegister`.
- `Control/`: Registro de Instruccion (IR), Contador de Programa (PC) y la **Unidad de Control (CU)** central que gestiona los ciclos de maquina (T1-T6).
- `Memory/`: Aloja las estructuras de memoria, incluyendo la `RAM16x8` y el `MAR` (Registro de Direccion de Memoria).
- `legacy/`: Funciona como un archivo para versiones e iteraciones anteriores del codigo.


### Guia de Inicio (Configuracion de SystemC)
Dado que SystemC es una biblioteca de C++, es necesario contar con un compilador de C++ y la biblioteca instalada.

#### Linux / WSL (Ubuntu/Debian)
Este proyecto esta configurado para compilarse nativamente en entornos Linux o en el Subsistema de Windows para Linux (WSL).

1. **Instalar dependencias:**
   ```bash
   sudo apt update
   sudo apt install build-essential cmake
   ```
2. **Instalar SystemC:**
   Asegurate de tener SystemC instalado (por ejemplo, en `/usr/local/systemc`). Configura la variable de entorno:
   ```bash
   export SYSTEMC_HOME=/usr/local/systemc
   ```
3. **Compilar y Ejecutar:**
   Puedes usar el script en bash incluido para compilar automaticamente y ejecutar todas las pruebas con un solo comando:
   ```bash
   ./run_tests.sh
   ```
