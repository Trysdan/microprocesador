#ifndef COMPARATORUNIT8BIT_H
#define COMPARATORUNIT8BIT_H

#include <systemc.h>

SC_MODULE(ComparatorUnit8Bit) {
    sc_in<bool> a[8], b[8];
    sc_out<bool> out_equal[8], out_greater[8], out_less[8];

    void compute();

    SC_CTOR(ComparatorUnit8Bit) {
        SC_METHOD(compute);
        for (int i = 0; i < 8; i++) {
            sensitive << a[i] << b[i];
        }
    }
};

#endif
