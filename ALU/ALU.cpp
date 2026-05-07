#include "ALU.hpp"

void ALU::process_control() {
  sc_uint<4> op;
  for (int i = 0; i < 4; i++) op[i] = opcode[i].read();

  // OpCode 0x5 es SUB (ctrl=1), OpCode 0x2 es ADD (ctrl=0)
  if (op == 0x5) {
    s_ctrl_arith.write(true);
  } else {
    s_ctrl_arith.write(false);
  }
}

void ALU::drive_bus() {
  if (alu_out_en.read() == true) {
    sc_lv<8> temp;
    for (int i = 0; i < 8; i++) {
      temp[i] = result[i].read() ? '1' : '0';
    }

    data_out.write(temp);
  } else {
    data_out.write("ZZZZZZZZ");
  }
}

void ALU::drive_zero() {
  bool is_zero = true;
  for (int i = 0; i < 8; i++) {
    if (result[i].read() == true) {
      is_zero = false;
      break;
    }
  }
  zero.write(is_zero);
}