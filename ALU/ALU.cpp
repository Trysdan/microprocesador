#include "ALU.hpp"

void ALU::process_control() {
  // Si opcode es 0001 (decimal 1), el AdderSub debe restar (ctrl = 1)
  // Para todos los demas casos, que sume (ctrl = 0)
  bool b0 = opcode[0].read();
  bool b1 = opcode[1].read();
  bool b2 = opcode[2].read();
  bool b3 = opcode[3].read();

  if (b0 && !b1 && !b2 && !b3) {
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