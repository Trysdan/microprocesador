#ifndef SUBSTRACTOR4BIT_H
#define SUBSTRACTOR4BIT_H

#include <systemc.h>
#include "FullAdder.hpp"

SC_MODULE(Substractor4Bit) {
    sc_in<bool> a[4], b[4];
    sc_out<bool> s[4], bout;
    sc_signal<bool> b_not[4];
    sc_signal<bool> c_int[3];
    sc_signal<bool> v_high; 
    FullAdder fa0, fa1, fa2, fa3;

    void invert_b();

    SC_CTOR(Substractor4Bit) : 
        fa0("fa0"), fa1("fa1"), fa2("fa2"), fa3("fa3"), v_high("v_high") 
    {
        v_high.write(true);

        SC_METHOD(invert_b);
        for(int i=0; i<4; i++) sensitive << b[i];

        fa0.a(a[0]); fa0.b(b_not[0]); fa0.cin(v_high);    fa0.s(s[0]); fa0.cout(c_int[0]);
        fa1.a(a[1]); fa1.b(b_not[1]); fa1.cin(c_int[0]); fa1.s(s[1]); fa1.cout(c_int[1]);
        fa2.a(a[2]); fa2.b(b_not[2]); fa2.cin(c_int[1]); fa2.s(s[2]); fa2.cout(c_int[2]);
        fa3.a(a[3]); fa3.b(b_not[3]); fa3.cin(c_int[2]); fa3.s(s[3]); fa3.cout(bout);
    }
};

#endif