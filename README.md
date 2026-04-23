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

#### Linux (Ubuntu/Debian)
1. **Install dependencies:**
   ```bash
   sudo apt update
   sudo apt install build-essential cmake
   ```
2. **Install SystemC:**
   You can download the source from [Accellera](https://www.accellera.org/downloads/standards/systemc) and compile it, or install a pre-compiled version if available in your distro:
   ```bash
   sudo apt install libsystemc-dev
   ```
3. **Compile and Run:**
   Assuming you have a `CMakeLists.txt` or a simple `Makefile` setup:
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ./microprocessor_sim
   ```

#### Windows
Setting up SystemC on Windows can be done via MSYS2/MinGW or Visual Studio.
**Using MSYS2 (MinGW-w64) [Recommended for simplicity]:**
1. Download and install [MSYS2](https://www.msys2.org/).
2. Open the MSYS2 UCRT64 terminal and install the GCC toolchain and CMake:
   ```bash
   pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake make
   ```
3. Download the SystemC source code from [Accellera](https://www.accellera.org/downloads/standards/systemc).
4. Extract the source, navigate to the folder in the terminal, and compile SystemC using CMake:
   ```bash
   mkdir build && cd build
   cmake .. -DCMAKE_CXX_STANDARD=14
   make
   make install
   ```
5. Set the `SYSTEMC_HOME` environment variable to your compiled SystemC directory.
6. Compile the project using CMake similarly to Linux.

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

#### Linux (Ubuntu/Debian)
1. **Instalar dependencias:**
   ```bash
   sudo apt update
   sudo apt install build-essential cmake
   ```
2. **Instalar SystemC:**
   Puedes descargar el código fuente desde [Accellera](https://www.accellera.org/downloads/standards/systemc) y compilarlo, o instalar una versión precompilada si tu distribución lo permite:
   ```bash
   sudo apt install libsystemc-dev
   ```
3. **Compilar y Ejecutar:**
   Asumiendo que cuentas con un `CMakeLists.txt` o un `Makefile`:
   ```bash
   mkdir build && cd build
   cmake ..
   make
   ./microprocessor_sim
   ```

#### Windows
La configuración de SystemC en Windows puede realizarse mediante MSYS2/MinGW o Visual Studio.
**Usando MSYS2 (MinGW-w64) [Recomendado por simplicidad]:**
1. Descarga e instala [MSYS2](https://www.msys2.org/).
2. Abre la terminal MSYS2 UCRT64 e instala la cadena de herramientas GCC y CMake:
   ```bash
   pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake make
   ```
3. Descarga el código fuente de SystemC desde [Accellera](https://www.accellera.org/downloads/standards/systemc).
4. Extrae el código, navega a la carpeta en la terminal y compila SystemC usando CMake:
   ```bash
   mkdir build && cd build
   cmake .. -DCMAKE_CXX_STANDARD=14
   make
   make install
   ```
5. Configura la variable de entorno `SYSTEMC_HOME` apuntando al directorio donde instalaste SystemC.
6. Compila el proyecto utilizando CMake de manera similar a Linux.
