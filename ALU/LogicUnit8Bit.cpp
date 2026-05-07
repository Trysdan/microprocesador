#include "LogicUnit8Bit.hpp"

void LogicUnit8Bit::compute() {
    for (int i = 0; i < 8; i++) {
        out_and[i].write(a[i].read() & b[i].read());
        out_or[i].write(a[i].read() | b[i].read());
        out_xor[i].write(a[i].read() ^ b[i].read());
        out_not_a[i].write(!a[i].read());
    }
}
