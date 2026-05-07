#ifndef MAR_H
#define MAR_H

#include <systemc.h>

SC_MODULE(MAR) {
    sc_in<bool> clk;
    sc_in<bool> reset;
    sc_in<bool> mar_load;
    sc_in<sc_lv<8>> data_in;
    sc_out<sc_uint<4>> address;

    void process_mar();

    SC_CTOR(MAR) {
        SC_METHOD(process_mar);
        sensitive << clk.pos() << reset;
    }
};

#endif
