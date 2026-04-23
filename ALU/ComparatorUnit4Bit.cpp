#include "ComparatorUnit4Bit.hpp"

void ComparatorUnit4Bit::compute_comparison() {
  // 1. Convertimos los bits individuales a un valor entero para comparar
  sc_uint<4> val_a, val_b;
  for (int i = 0; i < 4; i++) {
    val_a[i] = a[i].read();
    val_b[i] = b[i].read();
  }

  // 2. Evaluamos las condiciones
  bool eq = (val_a == val_b);
  bool gt = (val_a > val_b);
  bool lt = (val_a < val_b);

  // 3. Escribimos en las salidas de 4 bits
  // El bit 0 contendra el resultado, los bits 1, 2 y 3 seran siempre 0
  for (int i = 0; i < 4; i++) {
    if (i == 0) {
      out_equal[i].write(eq);
      out_greater[i].write(gt);
      out_less[i].write(lt);
    } else {
      out_equal[i].write(false);
      out_greater[i].write(false);
      out_less[i].write(false);
    }
  }
}