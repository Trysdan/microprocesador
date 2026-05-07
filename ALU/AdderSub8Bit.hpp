#ifndef ADDERSUB8BIT_H
#define ADDERSUB8BIT_H

#include <systemc.h>

SC_MODULE(AdderSub8Bit) {
  sc_in<bool> a[8], b[8], ctrl; // 0 para Suma, 1 para Resta
  sc_out<bool> s[8], cout;

  // Senales internas para CLA
  sc_signal<bool> b_real[8];
  sc_signal<bool> p[8], g[8];
  sc_signal<bool> c[9]; // c[0] es cin, c[8] es cout

  void compute_inversion();
  void compute_pg();
  void compute_carry();
  void compute_sum();

  SC_CTOR(AdderSub8Bit) {
    SC_METHOD(compute_inversion);
    for (int i = 0; i < 8; i++) sensitive << b[i] << ctrl;

    SC_METHOD(compute_pg);
    for (int i = 0; i < 8; i++) sensitive << a[i] << b_real[i];

    SC_METHOD(compute_carry);
    sensitive << ctrl;
    for (int i = 0; i < 8; i++) sensitive << p[i] << g[i];

    SC_METHOD(compute_sum);
    for (int i = 0; i < 8; i++) sensitive << p[i] << c[i];
  }
};

#endif
