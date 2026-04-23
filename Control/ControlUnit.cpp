#include "ControlUnit.hpp"

void ControlUnit::reset_signals() {
    pc_inc.write(false);
    pc_out.write(false);
    mar_load.write(false);
    ir_load.write(false);
    ir_out.write(false);
    ram_out.write(false);
    ram_write.write(false);
    acc_load.write(false);
    acc_out.write(false);
    alu_out.write(false);
}

void ControlUnit::process_fsm() {
    if (reset.read() == true) {
        current_state.write(T1);
        reset_signals();
    } 
    else if (clk.event() && clk.read() == true) {
        reset_signals(); // Por defecto todas apagadas al inicio del ciclo

        switch (current_state.read()) {
            case T1: // Fetch 1: PC a MAR
                pc_out.write(true);
                mar_load.write(true);
                current_state.write(T2);
                break;

            case T2: // Fetch 2: PC++
                pc_inc.write(true);
                current_state.write(T3);
                break;

            case T3: // Fetch 3: RAM[MAR] a IR
                ram_out.write(true);
                ir_load.write(true);
                current_state.write(T4);
                break;

            case T4: // Execute 1
                switch (opcode.read()) {
                    case 0x1: // LDA: IR[operand] a MAR
                        ir_out.write(true);
                        mar_load.write(true);
                        break;
                    case 0x2: // ADD: IR[operand] a MAR
                        ir_out.write(true);
                        mar_load.write(true);
                        break;
                    case 0xE: // OUT: ACC a Bus
                        acc_out.write(true);
                        break;
                    case 0xF: // HLT: Detener (permanecer en T4)
                        current_state.write(T4); 
                        return; 
                }
                current_state.write(T5);
                break;

            case T5: // Execute 2
                switch (opcode.read()) {
                    case 0x1: // LDA: RAM a ACC
                        ram_out.write(true);
                        acc_load.write(true);
                        break;
                    case 0x2: // ADD: RAM a Bus (ALU suma internamente)
                        ram_out.write(true);
                        break;
                }
                current_state.write(T6);
                break;

            case T6: // Execute 3
                switch (opcode.read()) {
                    case 0x2: // ADD: ALU a ACC
                        alu_out.write(true);
                        acc_load.write(true);
                        break;
                }
                current_state.write(T1); // Volver a Fetch
                break;
        }
    }
}
