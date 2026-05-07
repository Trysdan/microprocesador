#ifndef COMPARATORUNIT4BIT_H
#define COMPARATORUNIT4BIT_H

#include <systemc.h>

SC_MODULE(ComparatorUnit4Bit) {
  // Entradas de 4 bits
  sc_in<bool> a[4], b[4];

  // Salidas de 4 bits para el Mux (0001 si es verdadero, 0000 si es falso)
  sc_out<bool> out_equal[4];
  sc_out<bool> out_greater[4];
  sc_out<bool> out_less[4];

  void compute_comparison();

  SC_CTOR(ComparatorUnit4Bit) {
    SC_METHOD(compute_comparison);
    for (int i = 0; i < 4; i++) {
      sensitive << a[i] << b[i];
    }
  }
};

#endif