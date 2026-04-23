cmake_minimum_required(VERSION 3.14)
project(MicroprocesadorSystemC CXX)

# SystemC recomienda fuertemente el uso de C++17 o superior para las versiones mas recientes
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# ==========================================
# Configuracion de SystemC
# ==========================================
# Opcion 1: Buscar paquete si SystemC fue instalado mediante CMake
find_package(SystemC CONFIG QUIET HINTS $ENV{SYSTEMC_HOME})

if(NOT SystemC_FOUND)
    message(STATUS "SystemC_FOUND no se resolvio via find_package. Configuraremos manualmente usando la variable de entorno SYSTEMC_HOME")
    
    if("$ENV{SYSTEMC_HOME}" STREQUAL "")
        message(FATAL_ERROR "No se encontro SystemC y la variable de entorno SYSTEMC_HOME esta vacia. Por favor configurala apuntando al directorio raiz de SystemC.")
    endif()

    # Declaramos SystemC como una libreria externa
    add_library(SystemC::systemc SHARED IMPORTED)
    
    # Resolviendo la arquitectura de la libreria (Depende del compilador / OS)
    if(WIN32)
        set(SYS_C_LIB_NAME "systemc.lib")
        # El directorio lib-mingw, lib-cygwin o lib segun compilador en Windows
        set(SYS_C_LIB_DIR "lib") 
    else()
        set(SYS_C_LIB_NAME "libsystemc.so")
        set(SYS_C_LIB_DIR "lib")
    endif()

    set_target_properties(SystemC::systemc PROPERTIES
        IMPORTED_LOCATION "$ENV{SYSTEMC_HOME}/${SYS_C_LIB_DIR}/${SYS_C_LIB_NAME}"
        INTERFACE_INCLUDE_DIRECTORIES "$ENV{SYSTEMC_HOME}/include"
    )
endif()

# ==========================================
# Entorno de Pruebas (CTest)
# ==========================================
# Habilitamos CTest. Esto te permitira correr multiples testbenches
# usando el comando `ctest` desde la carpeta build.
enable_testing()

# ==========================================
# Componente 1: ALU (Testbench actual)
# ==========================================
# Definimos los archivos fuente de la ALU para reutilizarlos en otros testbenches
set(ALU_SOURCES
    ALU/ALU.cpp
    ALU/AdderSub4Bit.cpp
    ALU/ComparatorUnit4Bit.cpp
    ALU/LogicUnit4Bit.cpp
    ALU/Mux16to1.cpp
)

# Agrupamos todos los archivos fuente de la ALU para generar un ejecutable (Testbench)
add_executable(tb_alu
    ALU/main.cpp
    ${ALU_SOURCES}
)

# Enlazamos la libreria de SystemC a nuestro ejecutable tb_alu
target_link_libraries(tb_alu PRIVATE SystemC::systemc)

# Registramos el ejecutable como una prueba
add_test(NAME Test_ALU COMMAND tb_alu)

# ==========================================
# Componente 2: Registro (Acumulador)
# ==========================================
add_executable(tb_register
    Registers/main.cpp
    Registers/RegisterA.cpp
    ${ALU_SOURCES}
)
target_link_libraries(tb_register PRIVATE SystemC::systemc)
add_test(NAME Test_Register COMMAND tb_register)

# ==========================================
# Componente 3: Memoria RAM 16x8
# ==========================================
add_executable(tb_ram
    Memory/main.cpp
    Memory/RAM16x8.cpp
)
target_link_libraries(tb_ram PRIVATE SystemC::systemc)
add_test(NAME Test_RAM COMMAND tb_ram)

# ==========================================
# Componente 4: Bus Central y Logica Tri-State
# ==========================================
add_executable(tb_bus
    Bus/main.cpp
    Memory/RAM16x8.cpp
    Registers/RegisterA.cpp
)
target_link_libraries(tb_bus PRIVATE SystemC::systemc)
add_test(NAME Test_Bus COMMAND tb_bus)

# ==========================================
# Componente 5: Registro de Instruccion (IR)
# ==========================================
add_executable(tb_ir
    Control/main.cpp
    Control/InstructionRegister.cpp
)
target_link_libraries(tb_ir PRIVATE SystemC::systemc)
add_test(NAME Test_IR COMMAND tb_ir)

# ==========================================
# Componente 6: Program Counter (PC)
# ==========================================
add_executable(tb_pc
    Control/main_pc.cpp
    Control/ProgramCounter.cpp
)
target_link_libraries(tb_pc PRIVATE SystemC::systemc)
add_test(NAME Test_PC COMMAND tb_pc)

# ==========================================
# Componente 7: Unidad de Control (CU)
# ==========================================
add_executable(tb_cu
    Control/main_cu.cpp
    Control/ControlUnit.cpp
)
target_link_libraries(tb_cu PRIVATE SystemC::systemc)
add_test(NAME Test_CU COMMAND tb_cu)

# ==========================================
# Componente 8: Microprocesador Final (Semana 9)
# ==========================================
add_executable(microprocessor_final
    main.cpp
    ${ALU_SOURCES}
    Memory/RAM16x8.cpp
    Memory/MAR.cpp
    Registers/RegisterA.cpp
    Registers/RegisterB.cpp
    Control/ProgramCounter.cpp
    Control/InstructionRegister.cpp
    Control/ControlUnit.cpp
)
target_link_libraries(microprocessor_final PRIVATE SystemC::systemc)
add_test(NAME Test_Final COMMAND microprocessor_final)

# ==========================================
# Componente Futuro: Unidad de Control (Ejemplo)
# ==========================================
# Cuando tengas mas componentes con su propio Testbench (por ejemplo main.cpp),
# solo tienes que replicar el bloque anterior:

# add_executable(tb_control_unit ControlUnit/main.cpp ControlUnit/ControlUnit.cpp)
# target_link_libraries(tb_control_unit PRIVATE SystemC::systemc)
# add_test(NAME Test_ControlUnit COMMAND tb_control_unit)
