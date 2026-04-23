
#include "AdderSub4Bit.hpp"

void AdderSub4Bit::compute_inversion() {
    for(int i=0; i<4; i++) {
        // B_real = B XOR CTRL
        // Si ctrl=0 -> B_real = B
        // Si ctrl=1 -> B_real = NOT B
        b_real[i].write(b[i].read() ^ ctrl.read());
    }
}

void AdderSub4Bit::compute_pg() {
    for(int i=0; i<4; i++) {
        p[i].write(a[i].read() ^ b_real[i].read());
        g[i].write(a[i].read() & b_real[i].read());
    }
}

void AdderSub4Bit::compute_carry() {
    bool p0 = p[0].read(), p1 = p[1].read(), p2 = p[2].read(), p3 = p[3].read();
    bool g0 = g[0].read(), g1 = g[1].read(), g2 = g[2].read(), g3 = g[3].read();
    
    // El acarreo inicial Cin es igual a ctrl
    // (En resta, ctrl=1 proporciona el +1 necesario para Complemento a 2)
    bool ci = ctrl.read(); 

    bool c1 = g0 | (p0 & ci);
    bool c2 = g1 | (p1 & g0) | (p1 & p0 & ci);
    bool c3 = g2 | (p2 & g1) | (p2 & p1 & g0) | (p2 & p1 & p0 & ci);
    bool c4 = g3 | (p3 & g2) | (p3 & p2 & g1) | (p3 & p2 & p1 & g0) | (p3 & p2 & p1 & p0 & ci);

    c_int[0].write(ci);
    c_int[1].write(c1);
    c_int[2].write(c2);
    c_int[3].write(c3);
    cout.write(c4);
}

void AdderSub4Bit::compute_sum() {
    for(int i=0; i<4; i++) {
        s[i].write(p[i].read() ^ c_int[i].read());
    }
}