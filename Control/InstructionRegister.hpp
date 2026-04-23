#ifndef INSTRUCTIONREGISTER_H
#define INSTRUCTIONREGISTER_H

#include <systemc.h>

// Modulo InstructionRegister: Almacena y decodifica la instruccion actual
SC_MODULE(InstructionRegister) {
    // Puertos
    sc_in<bool> clk;
    sc_in<bool> reset;
    sc_in<bool> ir_load;        // Habilitador de carga de instruccion
    sc_in<sc_lv<8>> data_in;    // Instruccion de 8 bits (OpCode[7:4] | Operando[3:0])

    // Salidas decodificadas
    sc_out<sc_uint<4>> opcode;
    sc_out<sc_uint<4>> operand;

    // Proceso de carga
    void load_instruction();

    SC_CTOR(InstructionRegister) {
        SC_METHOD(load_instruction);
        sensitive << clk.pos() << reset;
    }
};

#endif
