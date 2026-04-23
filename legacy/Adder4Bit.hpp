#ifndef ADDER4BIT_H
#define ADDER4BIT_H

#include <systemc.h>
#include "FullAdder.hpp"

SC_MODULE(Adder4Bit) {
    sc_in<bool> a[4], b[4], cin;
    sc_out<bool> s[4], cout;

    sc_signal<bool> c_int[3];
    FullAdder fa0, fa1, fa2, fa3;

    SC_CTOR(Adder4Bit) : 
        fa0("fa0"), fa1("fa1"), fa2("fa2"), fa3("fa3") 
    {
        fa0.a(a[0]); fa0.b(b[0]); fa0.cin(cin);      fa0.s(s[0]); fa0.cout(c_int[0]);
        fa1.a(a[1]); fa1.b(b[1]); fa1.cin(c_int[0]); fa1.s(s[1]); fa1.cout(c_int[1]);
        fa2.a(a[2]); fa2.b(b[2]); fa2.cin(c_int[1]); fa2.s(s[2]); fa2.cout(c_int[2]);
        fa3.a(a[3]); fa3.b(b[3]); fa3.cin(c_int[2]); fa3.s(s[3]); fa3.cout(cout);
    }
};

#endif