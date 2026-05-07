#ifndef INSTRUCTIONREGISTER_H
#define INSTRUCTIONREGISTER_H

#include <systemc.h>

SC_MODULE(InstructionRegister) {
    sc_in<bool> clk;
    sc_in<bool> reset;
    sc_in<bool> ir_load;
    sc_in<sc_lv<8>> data_in;
    sc_out<sc_uint<4>> opcode;
    sc_out<sc_uint<4>> operand;
    sc_in<bool> ir_out_en;
    sc_out<sc_lv<8>> bus_out;

    void load_instruction();
    void drive_bus();

    SC_CTOR(InstructionRegister) {
        SC_METHOD(load_instruction);
        sensitive << clk.pos() << reset;
        SC_METHOD(drive_bus);
        sensitive << ir_out_en << operand;
    }
};

#endif
