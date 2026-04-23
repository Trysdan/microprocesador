#ifndef FULLADDER_H
#define FULLADDER_H

#include <systemc.h>
#include "HalfAdder.hpp"

SC_MODULE(FullAdder) {
    sc_in<bool> a, b, cin;
    sc_out<bool> s, cout;

    sc_signal<bool> s1, c1, c2;
    HalfAdder ha1, ha2;

    void compute_cout();

    SC_CTOR(FullAdder) : ha1("ha1"), ha2("ha2") {
        ha1.a(a);   ha1.b(b);   ha1.s(s1);  ha1.c(c1);
        ha2.a(s1);  ha2.b(cin); ha2.s(s);   ha2.c(c2);

        SC_METHOD(compute_cout);
        sensitive << c1 << c2;
    }
};

#endif