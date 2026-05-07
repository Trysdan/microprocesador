#ifndef INSTRUCTIONREGISTER_H
#define INSTRUCTIONREGISTER_H

#include <systemc.h>

SC_MODULE(InstructionRegister) {
    sc_in<bool> clk;
    sc_in<bool> reset;
    sc_in<bool> ir_load;
    sc_in<sc_lv<8>> data_in;
    sc_out<sc_uint<8>> opcode;

    void load_instruction();

    SC_CTOR(InstructionRegister) {
        SC_METHOD(load_instruction);
        sensitive << clk.pos() << reset;
    }
};

#endif
