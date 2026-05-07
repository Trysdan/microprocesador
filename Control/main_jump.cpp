#include <systemc.h>
#include "../Computer_Top.hpp"
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

    // PROGRAMA (Formato 2 Bytes):
    // ADDR 0: 0x01 (LDA)
    // ADDR 1: 14   (Operando) -> Carga 1 en Acc
    // ADDR 2: 0x05 (SUB)
    // ADDR 3: 14   (Operando) -> Acc = 1 - 1 = 0
    // ADDR 4: 0x04 (JZ)
    // ADDR 5: 8    (Operando) -> Si Zero=1, salta a ADDR 8
    // ADDR 6: 0x0E (OUT) -> No se debe ejecutar
    // ADDR 7: 0x0F (HLT) -> No se debe ejecutar
    // ADDR 8: 0x0E (OUT) -> Debe mostrar 0
    // ADDR 9: 0x0F (HLT)
    // ADDR 14: 0x01 (Dato 1)
    
    computer.ram->mem[0].write(0x01);
    computer.ram->mem[1].write(14);
    computer.ram->mem[2].write(0x05);
    computer.ram->mem[3].write(14);
    computer.ram->mem[4].write(0x04);
    computer.ram->mem[5].write(8);
    computer.ram->mem[6].write(0x0E);
    computer.ram->mem[7].write(0x0F);
    computer.ram->mem[8].write(0x0E);
    computer.ram->mem[9].write(0x0F);
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
        
        // Si llegamos al HLT en ADDR 9, el salto funciono (PC estara en 10 tras el fetch)
        if (computer.pc->current_value.read() == 10 && computer.opcode_ir.read() == 0xF) {
            success = true;
            std::cout << "\n[INFO] HLT Detectado tras ADDR 9. El JZ funciono correctamente!\n";
            break;
        }
        // Si llegamos al HLT en ADDR 7, el salto fallo (PC estara en 8 tras el fetch)
        if (computer.pc->current_value.read() == 8 && computer.opcode_ir.read() == 0xF) {
            success = false;
            std::cout << "\n[ERROR] HLT Detectado tras ADDR 7. El JZ NO funciono.\n";
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
