#ifndef REGISTERA_H
#define REGISTERA_H

#include <systemc.h>

// Modulo RegisterA (Acumulador): Registro principal de datos
SC_MODULE(RegisterA) {
    sc_in<bool> clk;
    sc_in<bool> reset;
    sc_in<bool> load_enable;
    sc_in<bool> out_enable;
    sc_in<sc_lv<8>> data_in;
    sc_out<sc_lv<8>> data_out;

    sc_signal<sc_uint<4>> internal_data;

    void write_data();
    void read_data();

    SC_CTOR(RegisterA) {
        SC_METHOD(write_data);
        sensitive << clk.pos() << reset;
        SC_METHOD(read_data);
        sensitive << out_enable << internal_data;
    }
};

#endif
