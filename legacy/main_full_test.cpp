#include <systemc.h>
#include "Computer_Top.h"
#include <iostream>
#include <iomanip>

int sc_main(int argc, char* argv[]) {
    // Reloj Global
    sc_clock clk("clk", 10, SC_NS, 0.5);
    sc_signal<bool> reset;

    // Instancia del Sistema Completo
    Computer_Top computer("MyComputer");
    computer.clk(clk);
    computer.reset(reset);

    // Trazado de Ondas
    sc_trace_file *wf = sc_create_vcd_trace_file("microprocessor_final");
    sc_trace(wf, clk, "clk");
    sc_trace(wf, reset, "reset");
    sc_trace(wf, computer.central_bus, "BUS");
    sc_trace(wf, computer.cu->current_state, "STATE");
    sc_trace(wf, computer.pc->current_value, "PC");
    sc_trace(wf, computer.regA->internal_data, "ACC");
    sc_trace(wf, computer.ram_addr, "MAR_ADDR");

    std::cout << "\n=============================================\n";
    std::cout << "   MICROPROCESADOR 4-BIT FINAL (SEMANA 9)    \n";
    std::cout << "=============================================\n\n";

    // CARGA DEL PROGRAMA EN RAM
    // ADDR 0: LDA 14 (0001 1110)
    // ADDR 1: ADD 15 (0010 1111)
    // ADDR 2: OUT    (1110 0000)
    // ADDR 3: HLT    (1111 0000)
    // ADDR 14: Dato 3 (0000 0011)
    // ADDR 15: Dato 2 (0000 0010)
    
    computer.ram->mem[0].write(0x1E); // LDA 14
    computer.ram->mem[1].write(0x2F); // ADD 15
    computer.ram->mem[2].write(0xE0); // OUT
    computer.ram->mem[3].write(0xF0); // HLT
    computer.ram->mem[14].write(0x03); // Dato 3
    computer.ram->mem[15].write(0x02); // Dato 2

    // Inicializacion
    reset.write(true);
    sc_start(15, SC_NS);
    reset.write(false);

    std::cout << "Iniciando ejecucion...\n";
    std::cout << "---------------------------------------------\n";
    std::cout << " Tiempo |  Estado  | PC |  Bus  |  ACC  | MAR \n";
    std::cout << "---------------------------------------------\n";

    // Corremos ciclos para asegurar que el programa termine
    for (int i = 0; i < 60; i++) {
        sc_start(10, SC_NS);
        
        // Mostrar estado en cada ciclo
        std::cout << std::setw(6) << sc_time_stamp() << " | "
                  << "   T" << (computer.cu->current_state.read() + 1) << "    | "
                  << std::setw(2) << computer.pc->current_value.read() << " | "
                  << computer.central_bus.read() << " | "
                  << std::setw(3) << computer.regA->internal_data.read() << " | "
                  << std::setw(2) << computer.ram_addr.read() << "\n";
        
        // Detener si llegamos al HLT (OpCode F)
        if (computer.opcode_ir.read() == 0xF && computer.cu->current_state.read() == 3) {
            std::cout << "\n[INFO] HLT Detectado. Deteniendo simulacion.\n";
            break;
        }
    }

    std::cout << "---------------------------------------------\n";
    std::cout << "\nRESULTADO FINAL EN ACUMULADOR: " << computer.regA->internal_data.read() << "\n";
    
    if (computer.regA->internal_data.read() == 5) {
        std::cout << ">>> TEST PASADO: 3 + 2 = 5 <<<\n";
    } else {
        std::cout << ">>> TEST FALLIDO <<<\n";
    }
    std::cout << "=============================================\n";

    sc_close_vcd_trace_file(wf);
    return 0;
}
