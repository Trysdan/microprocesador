#ifndef LOGICUNIT4BIT_H
#define LOGICUNIT4BIT_H

#include <systemc.h>

SC_MODULE(LogicUnit4Bit) {
    // Entradas
    sc_in<bool> a[4], b[4];
    
    // Salidas: Una para cada operación que el Mux podrá elegir
    sc_out<bool> out_and[4];
    sc_out<bool> out_or[4];
    sc_out<bool> out_xor[4];
    sc_out<bool> out_not_a[4];

    void compute_logic();

    SC_CTOR(LogicUnit4Bit) {
        SC_METHOD(compute_logic);
        // Sensible a cualquier cambio en los bits de entrada
        for(int i = 0; i < 4; i++) {
            sensitive << a[i] << b[i];
        }
    }
};

#endif