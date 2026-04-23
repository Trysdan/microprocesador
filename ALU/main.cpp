#include "ALU.hpp"
#include <systemc.h>
#include <iostream>

int sc_main(int argc, char *argv[]) {
  // Senales de entrada (Cables)
  sc_signal<bool> a[4], b[4], opcode[4];
  
  // Senales de salida
  sc_signal<bool> res[4];
  sc_signal<bool> cout;
  sc_signal<bool> alu_out_en;
  sc_signal<sc_lv<8>> bus_out;

  // Instancia del modulo ALU
  ALU alu("mi_alu");

  // Conectar senales a los puertos de la ALU
  for (int i = 0; i < 4; i++) {
    alu.a[i](a[i]);
    alu.b[i](b[i]);
    alu.opcode[i](opcode[i]);
    alu.result[i](res[i]);
  }
  alu.cout(cout);
  alu.alu_out_en(alu_out_en);
  alu.data_out(bus_out);

  // Inicializacion de entradas
  auto set_val = [&](sc_signal<bool>* port, int val) {
    for (int i = 0; i < 4; i++) port[i].write((val >> i) & 1);
  };

  std::cout << "\n--- TESTBENCH ALU ESTRUCTURAL (4-BIT) ---\n";

  // CASO 1: SUMA 5 + 3
  set_val(a, 5); // 0101
  set_val(b, 3); // 0011
  set_val(opcode, 0); // 0000 = Suma
  sc_start(10, SC_NS);
  std::cout << "5 + 3 = " << (res[3].read() << 3 | res[2].read() << 2 | res[1].read() << 1 | res[0].read()) << " (Cout: " << cout.read() << ")\n";

  // CASO 2: RESTA 10 - 4
  set_val(a, 10); // 1010
  set_val(b, 4);  // 0100
  set_val(opcode, 1); // 0001 = Resta
  sc_start(10, SC_NS);
  std::cout << "10 - 4 = " << (res[3].read() << 3 | res[2].read() << 2 | res[1].read() << 1 | res[0].read()) << "\n";

  std::cout << "\nSimulacion finalizada.\n";
  return 0;
}