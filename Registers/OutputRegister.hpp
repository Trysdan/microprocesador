#ifndef OUTPUTREGISTER_H
#define OUTPUTREGISTER_H

#include <systemc.h>
#include <iostream>

SC_MODULE(OutputRegister) {
    sc_in<bool> clk;
    sc_in<bool> reset;
    sc_in<bool> out_load;
    sc_in<sc_lv<8>> data_in;

    sc_signal<sc_uint<8>> internal_value;

    void process_load();
    void process_display();

    SC_CTOR(OutputRegister) {
        SC_METHOD(process_load);
        sensitive << clk.pos() << reset;

        SC_METHOD(process_display);
        sensitive << internal_value;
    }
};

#endif
