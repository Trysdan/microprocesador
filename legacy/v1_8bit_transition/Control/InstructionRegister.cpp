#include "InstructionRegister.hpp"

void InstructionRegister::load_instruction() {
    if (reset.read() == true) {
        opcode.write(0);
        operand.write(0);
    } else if (clk.event() && clk.read() == true) {
        if (ir_load.read() == true) {
            sc_lv<8> raw_data = data_in.read();
            opcode.write(raw_data.range(7, 4).to_uint());
            operand.write(raw_data.range(3, 0).to_uint());
        }
    }
}

void InstructionRegister::drive_bus() {
    if (ir_out_en.read() == true) {
        sc_lv<8> temp;
        sc_uint<4> op = operand.read();
        for (int i = 0; i < 4; i++) temp[i] = op[i] ? '1' : '0';
        for (int i = 4; i < 8; i++) temp[i] = '0';
        bus_out.write(temp);
    } else {
        bus_out.write("ZZZZZZZZ");
    }
}
