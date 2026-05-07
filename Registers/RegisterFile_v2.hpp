#ifndef REGISTER_FILE_V2_HPP
#define REGISTER_FILE_V2_HPP

#include <systemc.h>

// =====================================================
//  RegisterFile_v2 — Banco de Registros de 3 Puertos
//
//  R0..R7 (8 registros de 8 bits)
//
//  Puerto 1 (RS1): lectura combinacional
//  Puerto 2 (RS2): lectura combinacional
//  Puerto 3 (WB):  escritura síncrona (WB stage)
//
//  Las dos lecturas son simultáneas e independientes.
//  La escritura ocurre en el flanco positivo del reloj.
// =====================================================
SC_MODULE(RegisterFile_v2) {
    sc_in<bool> clk;
    sc_in<bool> reset;

    // ---- Puerto de Lectura 1 (RS1) ----
    sc_in<sc_uint<3>>  rs1_addr;
    sc_out<sc_uint<8>> rs1_data;

    // ---- Puerto de Lectura 2 (RS2 / Rd para branches/OUT) ----
    sc_in<sc_uint<3>>  rs2_addr;
    sc_out<sc_uint<8>> rs2_data;

    // ---- Puerto de Escritura (WB) ----
    sc_in<bool>        reg_write;
    sc_in<sc_uint<3>>  rd_addr;
    sc_in<sc_uint<8>>  rd_data;

    // Registros internos R0..R7
    sc_signal<sc_uint<8>> regs[8];

    void process_read();   // Lectura combinacional (ambos puertos)
    void process_write();  // Escritura síncrona

    SC_CTOR(RegisterFile_v2) {
        SC_METHOD(process_read);
        sensitive << rs1_addr << rs2_addr
                  << reg_write << rd_addr << rd_data;  // WB→ID bypass
        for (int i = 0; i < 8; i++) sensitive << regs[i];

        SC_METHOD(process_write);
        sensitive << clk.pos() << reset;
    }
};

#endif // REGISTER_FILE_V2_HPP
