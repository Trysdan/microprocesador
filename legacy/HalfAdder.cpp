#include "HalfAdder.hpp"

void HalfAdder::compute() {
    s.write(a.read() ^ b.read());
    c.write(a.read() & b.read());
}