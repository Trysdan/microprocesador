# SystemC Microprocessor Design / Diseño de Microprocesador en SystemC

*[Read in English](#english) | [Leer en Español](#español)*

---

<a id="english"></a>
## 🇬🇧 English

### Overview
This project is an educational, long-term endeavor to build a complete microprocessor from scratch using **SystemC**. The design approach is incremental, starting from fundamental digital logic components and scaling up to a fully functional processor architecture. 

Currently, the project begins with a **4-bit binary adder** and will evolve iteratively.

### Project Structure
To maintain a clear record of the design evolution, the repository is structured as follows:
- `ALU/`: Contains the active development files for the Arithmetic Logic Unit and related components.
- `legacy/`: Serves as an archive for previous versions and iterations of the code. This preserves the history of the design decisions and allows for comparative analysis of optimizations over time.

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

<a id="español"></a>
## 🇪🇸 Español

### Descripción General
Este proyecto es una iniciativa educativa a largo plazo con el objetivo de construir un microprocesador completo desde cero utilizando **SystemC**. El enfoque de diseño es incremental, comenzando con componentes lógicos fundamentales hasta llegar a una arquitectura de procesador totalmente funcional.

Actualmente, el proyecto inicia con un **sumador binario de 4 bits** y evolucionará de forma iterativa.

### Estructura del Proyecto
Para mantener un registro claro de la evolución del diseño, el repositorio se estructura de la siguiente manera:
- `ALU/`: Contiene los archivos de desarrollo activo para la Unidad Aritmético Lógica y componentes relacionados.
- `legacy/`: Funciona como un archivo para versiones e iteraciones anteriores del código. Esto preserva el historial de las decisiones de diseño y permite un análisis comparativo de las optimizaciones de hardware a lo largo del tiempo.

### Guía de Inicio (Configuración de SystemC)
Dado que SystemC es una biblioteca de C++, es necesario contar con un compilador de C++ y la biblioteca instalada.

#### Linux / WSL (Ubuntu/Debian)
Este proyecto está configurado para compilarse nativamente en entornos Linux o en el Subsistema de Windows para Linux (WSL).

1. **Instalar dependencias:**
   ```bash
   sudo apt update
   sudo apt install build-essential cmake
   ```
2. **Instalar SystemC:**
   Asegúrate de tener SystemC instalado (por ejemplo, en `/usr/local/systemc`). Configura la variable de entorno:
   ```bash
   export SYSTEMC_HOME=/usr/local/systemc
   ```
3. **Compilar y Ejecutar:**
   Puedes usar el script en bash incluido para compilar automáticamente y ejecutar todas las pruebas con un solo comando:
   ```bash
   ./run_tests.sh
   ```
