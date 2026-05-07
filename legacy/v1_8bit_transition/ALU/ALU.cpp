#include "ALU.hpp"

void ALU::process_control() {
  // El AdderSub debe restar (ctrl = 1) solo si el OpCode es 0101 (0x5)
  sc_uint<4> op;
  for (int i = 0; i < 4; i++) op[i] = opcode[i].read();

  if (op == 0x5) {
    s_ctrl_arith.write(true);
  } else {
    s_ctrl_arith.write(false);
  }
}


void ALU::drive_bus() {
  if (alu_out_en.read() == true) {
    sc_lv<8> temp;
    for (int i = 0; i < 4; i++) {
      temp[i] = result[i].read() ? '1' : '0';
    }
    for (int i = 4; i < 8; i++) {
      temp[i] = '0';
    }
    data_out.write(temp);
  } else {
    data_out.write("ZZZZZZZZ");
  }
}

void ALU::drive_zero() {
  bool is_zero = true;
  for (int i = 0; i < 4; i++) {
    if (result[i].read() == true) {
      is_zero = false;
      break;
    }
  }
  zero.write(is_zero);
}