#ifndef REGISTERB_H
#define REGISTERB_H

#include <systemc.h>

// Modulo RegisterB: Buffer para la entrada B de la ALU
SC_MODULE(RegisterB) {
    sc_in<bool> clk;
    sc_in<bool> reset;
    sc_in<bool> load_enable;
    sc_in<sc_lv<8>> data_in;
    sc_out<sc_uint<4>> data_out;

    sc_signal<sc_uint<4>> internal_data;

    void process_load();
    void drive_output();

    SC_CTOR(RegisterB) {
        SC_METHOD(process_load);
        sensitive << clk.pos() << reset;
        SC_METHOD(drive_output);
        sensitive << internal_data;
    }
};

#endif
