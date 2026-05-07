#ifndef ALU_V2_HPP
#define ALU_V2_HPP

#include <systemc.h>

// =====================================================
//  ALU_v2 — ALU Comportamental de 8 bits
//
//  Interfaz directa sc_uint<8> (sin arrays de bool).
//  Operación determinada por alu_op (= opcode ISA).
//
//  ADD=0x5 SUB=0x6 AND=0x7 OR=0x8 XOR=0x9 NOT=0xA
//  MOV/MVI/LOAD/STORE → PASS_A (propaga a sin modificar)
// =====================================================
SC_MODULE(ALU_v2) {
    sc_in<sc_uint<8>>  a;
    sc_in<sc_uint<8>>  b;
    sc_in<sc_uint<4>>  alu_op;

    sc_out<sc_uint<8>> result;
    sc_out<bool>       zero;    // result == 0

    void compute();

    SC_CTOR(ALU_v2) {
        SC_METHOD(compute);
        sensitive << a << b << alu_op;
    }
};

#endif // ALU_V2_HPP
