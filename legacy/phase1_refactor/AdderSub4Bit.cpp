
#include "AdderSub4Bit.hpp"

void AdderSub4Bit::compute_inversion() {
  for (int i = 0; i < 4; i++) {
    // B_real = B XOR CTRL
    // Si ctrl=0 -> B_real = B
    // Si ctrl=1 -> B_real = NOT B
    b_real[i].write(b[i].read() ^ ctrl.read());
  }
}

void AdderSub4Bit::compute_pg() {
  for (int i = 0; i < 4; i++) {
    p[i].write(a[i].read() ^ b_real[i].read());
    g[i].write(a[i].read() & b_real[i].read());
  }
}

void AdderSub4Bit::compute_carry() {
  c_int[0].write(cin.read());
  c_int[1].write(g[0].read() | (p[0].read() & c_int[0].read()));
  c_int[2].write(g[1].read() | (p[1].read() & g[0].read()) | (p[1].read() & p[0].read() & c_int[0].read()));
  c_int[3].write(g[2].read() | (p[2].read() & g[1].read()) | (p[2].read() & p[1].read() & g[0].read()) |
             (p[2].read() & p[1].read() & p[0].read() & c_int[0].read()));

  cout.write(g[3].read() | (p[3].read() & g[2].read()) | (p[3].read() & p[2].read() & g[1].read()) |
             (p[3].read() & p[2].read() & p[1].read() & g[0].read()) |
             (p[3].read() & p[2].read() & p[1].read() & p[0].read() & c_int[0].read()));
}

void AdderSub4Bit::compute_sum() {
  for (int i = 0; i < 4; i++) {
    s[i].write(p[i].read() ^ c_int[i].read());
  }
}