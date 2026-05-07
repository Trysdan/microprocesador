#include "LogicUnit4Bit.hpp"

void LogicUnit4Bit::compute_logic() {
  for (int i = 0; i < 4; i++) {
    bool va = a[i].read();
    bool vb = b[i].read();

    // Operaciones bit a bit puras
    out_and[i].write(va & vb);
    out_or[i].write(va | vb);
    out_xor[i].write(va ^ vb);
    out_not_a[i].write(!va); // Operacion unaria sobre A
  }
}