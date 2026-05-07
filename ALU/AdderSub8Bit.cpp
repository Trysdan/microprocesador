#include "AdderSub8Bit.hpp"

void AdderSub8Bit::compute_inversion() {
  for (int i = 0; i < 8; i++) {
    b_real[i].write(b[i].read() ^ ctrl.read());
  }
}

void AdderSub8Bit::compute_pg() {
  for (int i = 0; i < 8; i++) {
    p[i].write(a[i].read() ^ b_real[i].read());
    g[i].write(a[i].read() & b_real[i].read());
  }
}

void AdderSub8Bit::compute_carry() {
  bool cin = ctrl.read();
  bool carries[9];
  carries[0] = cin;
  c[0].write(cin);

  for (int i = 0; i < 8; i++) {
    carries[i+1] = g[i].read() | (p[i].read() & carries[i]);
    c[i+1].write(carries[i+1]);
  }
  cout.write(carries[8]);
}

void AdderSub8Bit::compute_sum() {
  for (int i = 0; i < 8; i++) {
    s[i].write(p[i].read() ^ c[i].read());
  }
}
