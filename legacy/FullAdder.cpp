#include "FullAdder.hpp"

void FullAdder::compute_cout() {
    cout.write(c1.read() | c2.read());
}