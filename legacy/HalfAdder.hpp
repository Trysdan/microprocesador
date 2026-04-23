#ifndef HALFADDER_H
#define HALFADDER_H

#include <systemc.h>

SC_MODULE(HalfAdder) {
    sc_in<bool> a, b;
    sc_out<bool> s, c;

    void compute();

    SC_CTOR(HalfAdder) {
        SC_METHOD(compute);
        sensitive << a << b;
    }
};

#endif