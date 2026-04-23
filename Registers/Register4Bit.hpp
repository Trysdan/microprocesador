#ifndef REGISTER4BIT_H
#define REGISTER4BIT_H

#include <systemc.h>

SC_MODULE(Register4Bit) {
    // Puertos de Entrada
    sc_in<bool> clk;
    sc_in<bool> reset;        // Reset asíncrono
    sc_in<bool> load_enable;  // Habilitador de carga
    sc_in<sc_uint<4>> data_in; // Entrada de datos (4 bits)
    
    // Puertos de Salida
    sc_out<sc_uint<4>> data_out; // Salida de datos (4 bits)

    // Proceso Principal
    void write_data();

    SC_CTOR(Register4Bit) {
        SC_METHOD(write_data);
        // Sensible al flanco de subida del reloj y al nivel alto del reset (asíncrono)
        sensitive << clk.pos() << reset;
    }
};

#endif
