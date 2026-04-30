#include "ComparatorUnit4Bit.hpp"

void ComparatorUnit4Bit::compute_comparison() {
  bool bit_a[4], bit_b[4];
  for (int i = 0; i < 4; i++) {
    bit_a[i] = a[i].read();
    bit_b[i] = b[i].read();
  }

  // 1. Igualdad (EQ): EQ = (A3^B3)' & (A2^B2)' & (A1^B1)' & (A0^B0)'
  bool x[4]; // x_i = (Ai ^ Bi)' -> Ai == Bi
  for (int i = 0; i < 4; i++) {
    x[i] = !(bit_a[i] ^ bit_b[i]);
  }
  bool eq = x[3] & x[2] & x[1] & x[0];

  // 2. Mayor que (GT): Magnitud bit a bit
  // GT = A3 B3' + x3 A2 B2' + x3 x2 A1 B1' + x3 x2 x1 A0 B0'
  bool gt = (bit_a[3] & !bit_b[3]) |
            (x[3] & bit_a[2] & !bit_b[2]) |
            (x[3] & x[2] & bit_a[1] & !bit_b[1]) |
            (x[3] & x[2] & x[1] & bit_a[0] & !bit_b[0]);

  // 3. Menor que (LT): LT = !(GT | EQ)
  bool lt = !(gt | eq);

  // 4. Escribimos en las salidas (el bus espera 4 bits, ponemos el resultado en bit 0)
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