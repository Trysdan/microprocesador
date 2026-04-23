#include "Mux16to1.hpp"

void Mux16to1::process_mux() {
  // 1. Convertir el selector binario a entero
  unsigned int index = 0;
  for (int i = 0; i < 4; i++) {
    if (sel[i].read()) {
      index |= (1 << i);
    }
  }

  // 2. Pasar la entrada seleccionada a la salida
  // index tendrá un valor de 0 a 15
  for (int i = 0; i < 4; i++) {
    out[i].write(in[index][i]->read());
  }
}