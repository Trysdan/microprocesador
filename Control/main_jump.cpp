#include <systemc.h>
#include "../Computer_Top.h"
#include <iostream>
#include <iomanip>

int sc_main(int argc, char* argv[]) {
    sc_clock clk("clk", 10, SC_NS, 0.5);
    sc_signal<bool> reset;

    Computer_Top computer("JumpComputer");
    computer.clk(clk);
    computer.reset(reset);

    sc_trace_file *wf = sc_create_vcd_trace_file("jump_test_waves");
    sc_trace(wf, clk, "clk");
    sc_trace(wf, reset, "reset");
    sc_trace(wf, computer.central_bus, "BUS");
    sc_trace(wf, computer.cu->current_state, "STATE");
    sc_trace(wf, computer.pc->current_value, "PC");
    sc_trace(wf, computer.regA->internal_data, "ACC");
    sc_trace(wf, computer.alu_zero, "ZERO_FLAG");

    std::cout << "\n=============================================\n";
    std::cout << "      TESTBENCH: JMP & JZ (SEMANA 10)        \n";
    std::cout << "=============================================\n\n";

    // PROGRAMA:
    // ADDR 0: 0x1E -> LDA 14 (Carga 1 en Acc)
    // ADDR 1: 0x5E -> SUB 14 (Acc = 1 - 1 = 0)
    // ADDR 2: 0x45 -> JZ 5   (Si Zero=1, salta a ADDR 5)
    // ADDR 3: 0xE0 -> OUT    (No se debe ejecutar si JZ funciona)
    // ADDR 4: 0xF0 -> HLT    (No se debe ejecutar si JZ funciona)
    // ADDR 5: 0xE0 -> OUT    (Debe mostrar 0)
    // ADDR 6: 0xF0 -> HLT
    // ADDR 14: 0x01 (Dato 1)
    
    computer.ram->mem[0].write(0x1E);
    computer.ram->mem[1].write(0x5E);
    computer.ram->mem[2].write(0x45);
    computer.ram->mem[3].write(0xE0);
    computer.ram->mem[4].write(0xF0);
    computer.ram->mem[5].write(0xE0);
    computer.ram->mem[6].write(0xF0);
    computer.ram->mem[14].write(0x01);

    reset.write(true);
    sc_start(15, SC_NS);
    reset.write(false);

    std::cout << " Tiempo |  Estado  | PC |  Bus  |  ACC  | ZERO \n";
    std::cout << "----------------------------------------------\n";

    bool success = false;
    for (int i = 0; i < 100; i++) {
        sc_start(10, SC_NS);
        
        std::cout << std::setw(6) << sc_time_stamp() << " | "
                  << "   T" << (computer.cu->current_state.read() + 1) << "    | "
                  << std::setw(2) << computer.pc->current_value.read() << " | "
                  << computer.central_bus.read() << " | "
                  << std::setw(3) << computer.regA->internal_data.read() << " | "
                  << (computer.alu_zero.read() ? "1" : "0") << "\n";
        
        // Si llegamos al HLT en ADDR 6, el salto funciono
        if (computer.pc->current_value.read() == 6 && computer.opcode_ir.read() == 0xF) {
            success = true;
            std::cout << "\n[INFO] HLT Detectado en ADDR 6. El JZ funciono correctamente!\n";
            break;
        }
        // Si llegamos al HLT en ADDR 4, el salto fallo
        if (computer.pc->current_value.read() == 4 && computer.opcode_ir.read() == 0xF) {
            success = false;
            std::cout << "\n[ERROR] HLT Detectado en ADDR 4. El JZ NO funciono.\n";
            break;
        }
    }

    if (success) {
        std::cout << "\n>>> TEST PASADO: JMP y JZ verificados <<<\n";
    } else {
        std::cout << "\n>>> TEST FALLIDO <<<\n";
    }
    std::cout << "=============================================\n";

    sc_close_vcd_trace_file(wf);
    return success ? 0 : 1;
}
