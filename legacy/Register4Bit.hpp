#ifndef REGISTER4BIT_H
#define REGISTER4BIT_H

#include <systemc.h>

SC_MODULE(Register4Bit) {
    sc_in<bool> clk;
    sc_in<bool> reset;
    sc_in<bool> load_enable;
    sc_in<bool> out_enable;
    sc_in<sc_lv<8>> data_in;
    sc_out<sc_lv<8>> data_out;

    sc_signal<sc_uint<4>> internal_data;

    void write_data();
    void read_data();

    SC_CTOR(Register4Bit) {
        SC_METHOD(write_data);
        sensitive << clk.pos() << reset;
        SC_METHOD(read_data);
        sensitive << out_enable << internal_data;
    }
};

#endif
