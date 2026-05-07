#ifndef LOGICUNIT8BIT_H
#define LOGICUNIT8BIT_H

#include <systemc.h>

SC_MODULE(LogicUnit8Bit) {
    sc_in<bool> a[8], b[8];
    sc_out<bool> out_and[8], out_or[8], out_xor[8], out_not_a[8];

    void compute();

    SC_CTOR(LogicUnit8Bit) {
        SC_METHOD(compute);
        for (int i = 0; i < 8; i++) {
            sensitive << a[i] << b[i];
        }
    }
};

#endif
