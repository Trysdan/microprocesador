#include "InstructionRegister.hpp"

void InstructionRegister::load_instruction() {
    if (reset.read() == true) {
        opcode.write(0);
    } else if (clk.event() && clk.read() == true) {
        if (ir_load.read() == true) {
            opcode.write(data_in.read().to_uint());
        }
    }
}
