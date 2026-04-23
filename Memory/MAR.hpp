#ifndef MAR_H
#define MAR_H

#include <systemc.h>

// Modulo MAR (Memory Address Register): Captura direcciones del bus para la RAM
SC_MODULE(MAR) {
    sc_in<bool> clk;
    sc_in<bool> reset;
    sc_in<bool> mar_load;      // Senal de carga desde el bus
    sc_in<sc_lv<4>> data_in;   // Entrada desde el Bus Central
    sc_out<sc_uint<4>> address; // Salida directa a los pines de direccion de la RAM

    void process_mar() {
        if (reset.read() == true) {
            address.write(0);
        } else if (clk.event() && clk.read() == true) {
            if (mar_load.read() == true) {
                address.write(data_in.read().to_uint());
            }
        }
    }

    SC_CTOR(MAR) {
        SC_METHOD(process_mar);
        sensitive << clk.pos() << reset;
    }
};

#endif
