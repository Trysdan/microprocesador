#ifndef REGISTERFILE_HPP
#define REGISTERFILE_HPP

#include <systemc.h>

SC_MODULE(RegisterFile) {
    sc_in<bool> clk;
    sc_in<bool> reset;
    
    // Senales de Control
    sc_in<bool> reg_write;      // Habilita la escritura en un registro
    sc_in<bool> reg_out;        // Habilita la salida al bus
    sc_in<sc_uint<3>> reg_sel;  // Selecciona cual de los 8 registros (0-7)
    
    // Bus de datos
    sc_inout_rv<8> bus_data;

    // Registros internos (8 registros de 8 bits)
    sc_signal<sc_uint<8>> regs[8];

    void process_write();
    void process_read();

    SC_CTOR(RegisterFile) {
        SC_METHOD(process_write);
        sensitive << clk.pos() << reset;

        SC_METHOD(process_read);
        sensitive << reg_out << reg_sel;
        for(int i=0; i<8; i++) sensitive << regs[i];
    }
};

#endif
