#include "InstructionRegister.hpp"

void InstructionRegister::load_instruction() {
    // 1. Reset Asincrono
    if (reset.read() == true) {
        opcode.write(0);
        operand.write(0);
    } 
    // 2. Si no hay reset, y estamos en un flanco de subida del reloj
    else if (clk.event() && clk.read() == true) {
        if (ir_load.read() == true) {
            sc_lv<8> raw_data = data_in.read();
            
            // Dividimos la instruccion de 8 bits:
            // Bits [7:4] son el OpCode
            // Bits [3:0] son el Operando
            opcode.write(raw_data.range(7, 4).to_uint());
            operand.write(raw_data.range(3, 0).to_uint());
        }
    }
}
