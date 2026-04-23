#include "Substractor4Bit.hpp"

void Substractor4Bit::invert_b() {
    for(int i = 0; i < 4; i++) {
        b_not[i].write(!b[i].read());
    }
}