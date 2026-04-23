#ifndef ADDERSUB4BIT_H
#define ADDERSUB4BIT_H

#include <systemc.h>

SC_MODULE(AdderSub4Bit) {
  // Puertos
  sc_in<bool> a[4], b[4], ctrl; // ctrl = 0 (Suma), ctrl = 1 (Resta)
  sc_out<bool> s[4], cout;

  // Señales internas
  sc_signal<bool> b_real[4]; // B corregido (directo o invertido)
  sc_signal<bool> p[4], g[4];
  sc_signal<bool> c_int[4];

  // Procesos
  void compute_inversion(); // Aplica XOR a B
  void compute_pg();        // Genera P y G
  void compute_carry();     // Lógica CLA
  void compute_sum();       // Genera S

  SC_CTOR(AdderSub4Bit) {
    SC_METHOD(compute_inversion);
    for (int i = 0; i < 4; i++)
      sensitive << b[i] << ctrl;

    SC_METHOD(compute_pg);
    for (int i = 0; i < 4; i++)
      sensitive << a[i] << b_real[i];

    SC_METHOD(compute_carry);
    sensitive << ctrl;
    for (int i = 0; i < 4; i++)
      sensitive << p[i] << g[i];

    SC_METHOD(compute_sum);
    for (int i = 0; i < 4; i++)
      sensitive << p[i] << c_int[i];
  }
};

#endif