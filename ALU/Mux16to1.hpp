#ifndef MUX16TO1_H
#define MUX16TO1_H

#include <systemc.h>

SC_MODULE(Mux16to1) {
    // Definimos arreglos de puertos directamente
    sc_in<bool> in[16][4]; 
    sc_in<bool> sel[4];
    sc_out<bool> out[4];

    void process_mux();

    SC_CTOR(Mux16to1) {
        SC_METHOD(process_mux);
        // Sensibilidad a todas las entradas y al selector
        for (int i = 0; i < 4; i++) {
            sensitive << sel[i];
            for (int j = 0; j < 16; j++) {
                sensitive << in[j][i];
            }
        }
    }
};

#endif