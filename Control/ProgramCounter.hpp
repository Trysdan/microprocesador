#ifndef PROGRAMCOUNTER_H
#define PROGRAMCOUNTER_H

#include <systemc.h>

SC_MODULE(ProgramCounter) {
    sc_in<bool> clk;
    sc_in<bool> reset;
    sc_in<bool> pc_inc;
    sc_in<bool> pc_load;
    sc_in<bool> pc_out_enable;
    sc_in<sc_lv<8>> data_in;
    sc_out<sc_lv<8>> data_out;

    sc_signal<sc_uint<4>> current_value;

    void update_pc();
    void drive_bus();

    SC_CTOR(ProgramCounter) {
        SC_METHOD(update_pc);
        sensitive << clk.pos() << reset;
        SC_METHOD(drive_bus);
        sensitive << current_value << pc_out_enable;
    }
};

#endif
