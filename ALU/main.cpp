#include "ALU.hpp"
#include <systemc.h>
#include <iostream>

int sc_main(int argc, char *argv[]) {
  // Senales de entrada (Cables)
  sc_signal<bool> a[8], b[8];
  sc_signal<sc_uint<4>> opcode_val;
  sc_signal<bool> opcode[4];
  
  // Senales de salida
  sc_signal<bool> res[8];
  sc_signal<bool> cout;
  sc_signal<bool> zero;
  sc_signal<bool> alu_out_en;
  sc_signal<sc_lv<8>> bus_out;

  // Instancia del modulo ALU
  ALU alu("mi_alu");

  // Conectar senales a los puertos de la ALU
  for (int i = 0; i < 8; i++) {
    alu.a[i](a[i]);
    alu.b[i](b[i]);
    alu.result[i](res[i]);
  }
  for (int i = 0; i < 4; i++) {
    alu.opcode[i](opcode[i]);
  }
  alu.cout(cout);
  alu.zero(zero);
  alu.alu_out_en(alu_out_en);
  alu.data_out(bus_out);

  // Inicializacion de entradas
  auto set_val = [&](sc_signal<bool>* port, int val) {
    for (int i = 0; i < 8; i++) port[i].write((val >> i) & 1);
  };
  auto set_op = [&](int val) {
    for (int i = 0; i < 4; i++) opcode[i].write((val >> i) & 1);
  };

  std::cout << "\n--- TESTBENCH ALU ESTRUCTURAL (8-BIT) ---\n";

  // CASO 1: SUMA 5 + 3
  set_val(a, 5); // 0101
  set_val(b, 3); // 0011
  set_op(0x2); // 0x2 = ADD
  sc_start(10, SC_NS);
  
  int r = 0; for(int i=0; i<8; i++) r |= (res[i].read() << i);
  std::cout << "5 + 3 = " << r << " (Cout: " << cout.read() << ")\n";

  // CASO 2: RESTA 10 - 4
  set_val(a, 10); // 1010
  set_val(b, 4);  // 0100
  set_op(0x5); // 0x5 = SUB
  sc_start(10, SC_NS);
  r = 0; for(int i=0; i<8; i++) r |= (res[i].read() << i);
  std::cout << "10 - 4 = " << r << " (Zero: " << zero.read() << ")\n";

  // CASO 3: RESTA 5 - 5 (Forzar Zero)
  set_val(a, 5);
  set_val(b, 5);
  set_op(0x5);
  sc_start(10, SC_NS);
  r = 0; for(int i=0; i<8; i++) r |= (res[i].read() << i);
  std::cout << "5 - 5 = " << r << " (Zero: " << zero.read() << ")\n";

  std::cout << "\nSimulacion finalizada.\n";
  return 0;
}