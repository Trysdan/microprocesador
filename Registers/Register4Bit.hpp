#ifndef REGISTER4BIT_H
#define REGISTER4BIT_H

#include <systemc.h>

SC_MODULE(Register4Bit) {
    // Puertos de Entrada
    sc_in<bool> clk;
    sc_in<bool> reset;        // Reset asíncrono
    sc_in<bool> load_enable;  // Habilitador de carga
    sc_in<bool> out_enable;   // Habilitador de salida (Tri-State)
    sc_in<sc_lv<4>> data_in; // Entrada de datos (Logic Vector)
    
    // Puertos de Salida (Logic Vector para soportar Z)
    sc_out<sc_lv<4>> data_out; 

    // Almacenamiento interno
    sc_signal<sc_uint<4>> internal_data;

    // Procesos
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
