#ifndef DECODER_HPP
#define DECODER_HPP

#include <systemc.h>

// =====================================================
//  Decoder — Decodificador Combinacional
//
//  Reemplaza la FSM ControlUnit (T1-T8).
//  Convierte el OpCode de 4 bits en señales de control
//  en un único ciclo de reloj (etapa ID del pipeline).
// =====================================================
SC_MODULE(Decoder) {
    sc_in<sc_uint<4>> opcode;

    // Señales de control de salida (conectan a ID/EX register)
    sc_out<bool>        ctrl_reg_write;  // WB:  escribir en RF
    sc_out<bool>        ctrl_mem_read;   // MEM: LOAD de RAM
    sc_out<bool>        ctrl_mem_write;  // MEM: STORE a RAM
    sc_out<bool>        ctrl_alu_src;    // EX:  0=Rs2, 1=Imm
    sc_out<bool>        ctrl_branch;     // EX:  BEZ o BNZ
    sc_out<bool>        ctrl_branch_nz;  // EX:  1=BNZ, 0=BEZ
    sc_out<bool>        ctrl_jump;       // EX:  JMP incondicional
    sc_out<bool>        ctrl_mem_to_reg; // WB:  1=dato mem, 0=ALU
    sc_out<bool>        ctrl_out_en;     // SYS: OUT register
    sc_out<bool>        ctrl_halt;       // SYS: HLT
    sc_out<bool>        ctrl_indirect;   // MEM: acceso indirecto (LOADI/STORI)
    sc_out<sc_uint<4>>  ctrl_alu_op;    // EX:  operación ALU

    void decode();

    SC_CTOR(Decoder) {
        SC_METHOD(decode);
        sensitive << opcode;
    }
};

#endif // DECODER_HPP
