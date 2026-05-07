#ifndef DUAL_PORT_RAM_HPP
#define DUAL_PORT_RAM_HPP

#include <systemc.h>

// =====================================================
//  DualPortRAM — RAM de 256 bytes con 2 puertos
//
//  Puerto IF  (Puerto A): lectura combinacional de 16 bits
//             Siempre activo, usado por la etapa IF.
//             Lee mem[addr] || mem[addr+1] en un ciclo.
//
//  Puerto DATA (Puerto B): lectura/escritura síncrona de 8 bits
//             Usado por la etapa MEM para LOAD/STORE.
// =====================================================
SC_MODULE(DualPortRAM) {
    // ---- Puerto IF (lectura de instrucción, 16-bit) ----
    sc_in<sc_uint<8>>  if_addr;        // Dirección de instrucción (PC)
    sc_out<sc_uint<16>> if_data;       // Instrucción de 16 bits (combinacional)

    // ---- Puerto DATA (LOAD/STORE, 8-bit) ----
    sc_in<bool>         clk;
    sc_in<bool>         data_write_en; // STORE: habilita escritura síncrona
    sc_in<bool>         data_read_en;  // LOAD:  habilita lectura combinacional
    sc_in<sc_uint<8>>  data_addr;     // Dirección de datos
    sc_in<sc_uint<8>>  data_in;       // Dato para STORE
    sc_out<sc_uint<8>> data_out;      // Dato para LOAD

    // Memoria interna: 256 bytes (acceso público para carga de programa)
    sc_signal<sc_uint<8>> mem[256];

    void read_instruction();  // Puerto IF: lectura 16-bit combinacional
    void read_data();         // Puerto DATA: lectura 8-bit combinacional
    void write_data();        // Puerto DATA: escritura 8-bit síncrona

    SC_CTOR(DualPortRAM) {
        SC_METHOD(read_instruction);
        sensitive << if_addr;
        for (int i = 0; i < 256; i++) sensitive << mem[i];

        SC_METHOD(read_data);
        sensitive << data_addr << data_read_en;
        for (int i = 0; i < 256; i++) sensitive << mem[i];

        SC_METHOD(write_data);
        sensitive << clk.pos();
    }
};

#endif // DUAL_PORT_RAM_HPP
