#ifndef RAM16X8_H
#define RAM16X8_H

#include <systemc.h>

SC_MODULE(RAM16x8) {
    sc_in<bool> clk;
    sc_in<bool> write_enable;
    sc_in<bool> out_enable;
    sc_in<sc_uint<4>> address;
    sc_in<sc_lv<8>> data_in;
    sc_out<sc_lv<8>> data_out;

    sc_signal<sc_uint<8>> mem[16];

    void write_data();
    void read_data();

    SC_CTOR(RAM16x8) {
        SC_METHOD(write_data);
        sensitive << clk.pos();
        SC_METHOD(read_data);
        sensitive << address << out_enable;
        for (int i = 0; i < 16; i++) sensitive << mem[i];
    }
};

#endif
